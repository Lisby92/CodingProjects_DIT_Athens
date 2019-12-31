#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct timestamp
{	//used to hold time instances
	long int 	sec;
	long int 	nsec;
} timestamp;

typedef struct part
{	//used to simulate a part
	//part id and type(1,2,3)
	int 		id;
	int 		type;
	//timestamps of entry/exit on each stage
	timestamp 	t_in_created;
	timestamp	t_out_created;
	timestamp	t_in_painted;
	timestamp	t_out_painted;
	timestamp 	t_in_checked;
	timestamp 	t_out_checked;
	timestamp 	t_in_assembled;
	timestamp 	t_out_assembled;
} part;

typedef part * part_ptr;

typedef struct product
{	//used to simulate a product
	int 		id;
	timestamp 	prod_time;
} product;

union senum
{	//semaphore union
    int val;
    struct 	semid_ds *buff;
    unsigned short *array;
} sem_attr; //initial lock value

void simulate(int Y);

int main(int argc, char ** argv)
{
	int Y;

	if(argc == 2)
	{
		Y = atoi(argv[1]);
		if(Y < 0)
		{
			printf("Please give valid number of parts!(Y > 0)\n");
			return 1;
		}
		else
			simulate(Y);
	}
	else
	{
		printf("Error : Number of parts to be produced not specified!\n");
		return 1;
	}
	return 0;	
}

void simulate(int Y)
{	//shared memory
	int    		Array_shm_id;
	int    		*Array_shm_ptr;
	int    		Paint_shm_id;
	part_ptr    Paint_shm_ptr;
	int    		Assemble_shm_id;
    part_ptr    Assemble_shm_ptr;
    //semaphores 
    int 		paint_mux;
    int 		p_empty;
    int 		p_full;
    int 		assemle_mux;
    int 		a_empty;
    int 		a_full;
    int 		check_mux;
    //time_related
	struct 		timespec paint;
	struct 		timespec check;
	struct 		timespec assemble;
	struct 		timespec timer;
	struct 		timespec pauser;
	timestamp	average_waiting_time;
    timestamp	average_total_time;
    timestamp	t_first_part;
    timestamp   wait_paint;
    //simulation_specific
    part 		new_part;
	part 		checked_part;
	product 	new_product;
	int 		pause_time;
	int    		count = 8;
	int 	    slaves = 0;
    int 		part_counter = 0;
    int 		total_items = 0;
	/******************************************/
	//	Create shared memory area : Array 	  //
	/******************************************/
	Array_shm_id = shmget(IPC_PRIVATE, 8*sizeof(int), IPC_CREAT | 0666);
    if(Array_shm_id < 0)
    {
        perror("shmget");
        exit(1);
    }
    Array_shm_ptr = (int *) shmat(Array_shm_id, NULL, 0);
    if((int ) *Array_shm_ptr == -1)
    {
        perror("shmat");
        exit(1);
    }
	/******************************************/
	//	Create shared memory area : Painter   //
	/******************************************/
    Paint_shm_id = shmget(IPC_PRIVATE, sizeof(part), IPC_CREAT | 0666);
    if(Paint_shm_id < 0)
    {
        perror("shmget");
        exit(1);
    }
    Paint_shm_ptr = (part_ptr) shmat(Paint_shm_id, NULL, 0);
    if(Paint_shm_ptr == (void *)-1)
    {
        perror("shmat");
        exit(1);
    }
	/******************************************/
	//	Create shared memory area : Assembler //
	/******************************************/
    Assemble_shm_id = shmget(IPC_PRIVATE, sizeof(part), IPC_CREAT | 0666);
    if(Assemble_shm_id < 0)
    {
        perror("shmget");
        exit(1);
    }
    Assemble_shm_ptr = (part_ptr) shmat(Assemble_shm_id, NULL, 0);
    if(Assemble_shm_ptr == (void *)-1)
    {
        perror("shmat");
        exit(1);
    }
    /******************************************/
	//			Create Semaphores 			  //
	/******************************************/
	//shared by slaves : {0,1,2,3,4,5,6}
    if((paint_mux = semget(IPC_PRIVATE, 1, 0660 | IPC_CREAT)) == -1)
    {
		perror("semget");
		exit(1);
	}
	sem_attr.val = 1;        //initial state = unlocked
	if(semctl(paint_mux, 0, SETVAL, sem_attr) == -1)
	{
		perror("semctl SETVAL");
		exit(1);
	}
	//shared by slaves : {0,1,2,4,5,6}
	if((p_empty = semget(IPC_PRIVATE, 1, 0660 | IPC_CREAT)) == -1)
    {
		perror("semget");
		exit(1);
	}
	sem_attr.val = 1;        //initial state = unlocked
	if(semctl(p_empty, 0, SETVAL, sem_attr) == -1)
	{
		perror("semctl SETVAL");
		exit(1);
	}
	//shared by slaves : {0,1,2,3}
	if((p_full = semget(IPC_PRIVATE, 1, 0660 | IPC_CREAT)) == -1)
    {
		perror("semget");
		exit(1);
	}
	sem_attr.val = 0;        //initial state = locked
	if(semctl(p_full, 0, SETVAL, sem_attr) == -1)
	{
		perror("semctl SETVAL");
		exit(1);
	}
	//shared by slaves : {4,5,6,7}
	if((assemle_mux = semget(IPC_PRIVATE, 1, 0660 | IPC_CREAT)) == -1)
    {
		perror("semget");
		exit(1);
	} 
	sem_attr.val = 1;        //initial state = unlocked
	if(semctl(assemle_mux, 0, SETVAL, sem_attr) == -1)
	{
		perror("semctl SETVAL");
		exit(1);
	}
	//shared by slaves : {4,5,6,7}
	if((a_empty = semget(IPC_PRIVATE, 1, 0660 | IPC_CREAT)) == -1)
    {
		perror("semget");
		exit(1);
	}
	sem_attr.val = 1;        //initial state = unlocked
	if(semctl(a_empty, 0, SETVAL, sem_attr) == -1)
	{
		perror("semctl SETVAL");
		exit(1);
	}
	//shared by slaves : {4,5,6,7}
	if((a_full = semget(IPC_PRIVATE, 1, 0660 | IPC_CREAT)) == -1)
    {
		perror("semget");
		exit(1);
	}
	sem_attr.val = 0;        //initial state = locked
	if(semctl(a_full, 0, SETVAL, sem_attr) == -1)
	{
		perror("semctl SETVAL");
		exit(1);
	}
	//shared by slaves : {3,4,5,6}
	if((check_mux = semget(IPC_PRIVATE, 1, 0660 | IPC_CREAT)) == -1)
    {
		perror("semget");
		exit(1);
	}
	sem_attr.val = 0;        //initial state = locked
	if(semctl(check_mux, 0, SETVAL, sem_attr) == -1)
	{
		perror("semctl SETVAL");
		exit(1);
	}
	/******************************************/
	//			Initialise Sem_Ops	 		  //
	/******************************************/
	struct sembuf up_paint_mux;
	up_paint_mux.sem_num = 0;
    up_paint_mux.sem_op = 1;
    up_paint_mux.sem_flg = 0;
    /*******************************************/
	struct sembuf down_paint_mux;
	down_paint_mux.sem_num = 0;
    down_paint_mux.sem_op = -1;
    down_paint_mux.sem_flg = 0;
    /*******************************************/
	struct sembuf up_p_empty;
	up_p_empty.sem_num = 0;
    up_p_empty.sem_op = 1;
    up_p_empty.sem_flg = 0;
    /*******************************************/
	struct sembuf down_p_empty;
	down_p_empty.sem_num = 0;
    down_p_empty.sem_op = -1;
    down_p_empty.sem_flg = 0;
    /*******************************************/
	struct sembuf up_p_full;
	up_p_full.sem_num = 0;
    up_p_full.sem_op = 1;
    up_p_full.sem_flg = 0;
    /*******************************************/
	struct sembuf down_p_full;
	down_p_full.sem_num = 0;
    down_p_full.sem_op = -1;
    down_p_full.sem_flg = 0;
    /*******************************************/
	struct sembuf up_assemble_mux;
	up_assemble_mux.sem_num = 0;
    up_assemble_mux.sem_op = 1;
    up_assemble_mux.sem_flg = 0;
    /*******************************************/
	struct sembuf down_assemble_mux;
	down_assemble_mux.sem_num = 0;
    down_assemble_mux.sem_op = -1;
    down_assemble_mux.sem_flg = 0;
    /*******************************************/
	struct sembuf up_a_empty;
	up_a_empty.sem_num = 0;
    up_a_empty.sem_op = 1;
    up_a_empty.sem_flg = 0;
    /*******************************************/
	struct sembuf down_a_empty;
	down_a_empty.sem_num = 0;
    down_a_empty.sem_op = -1;
    down_a_empty.sem_flg = 0;
    /*******************************************/
	struct sembuf up_a_full;
	up_a_full.sem_num = 0;
    up_a_full.sem_op = 1;
    up_a_full.sem_flg = 0;
    /*******************************************/
	struct sembuf down_a_full;
	down_a_full.sem_num = 0;
    down_a_full.sem_op = -1;
    down_a_full.sem_flg = 0;
    /*******************************************/
    struct sembuf up_check_mux;
    up_check_mux.sem_num = 0;
    up_check_mux.sem_op = 1;
    up_check_mux.sem_flg = 0;
    /*******************************************/
    struct sembuf down_check_mux;
    down_check_mux.sem_num = 0;
    down_check_mux.sem_op = -1;
    down_check_mux.sem_flg = 0;
	/******************************************/
	//			Spawn Slaves				  //
	/******************************************/
    pid_t  retpid = 1;// Force Master initially.
    while(count-- > 0 && retpid > 0)// Only fork if limit not reached AND is Master (slaves will exit loop with retpid == 0).
    {// Adjust actual count if successful.
        if((retpid = fork()) > 0)
        {
        	Array_shm_ptr[slaves] = retpid;
            slaves++;
        }
    }

    if(retpid == 0)
    {	//slaves here simulating all the stages of the production line
        if(getpid() == Array_shm_ptr[0] || getpid() == Array_shm_ptr[1] || getpid() == Array_shm_ptr[2])
        {
        	/******************************************/
			//			Construction Stage			  //
			/******************************************/
            srand((unsigned int)getpid());//genuine seed for each slave
            pauser.tv_sec = 0;//no second pauses , we just need nanosec field
    		while(total_items < Y)
    		{
    			pause_time = rand() % (7999999 + 1 - 3000000) + 3000000;//tweak(7.999.999ns = max delay, 3.000.000ns = min delay)
        		pauser.tv_nsec = pause_time;
        		if(clock_nanosleep(CLOCK_MONOTONIC, 0, &pauser, NULL) != 0)
        		{
        			perror("clock_nanosleep");
        			exit(1);
        		}
    			total_items ++;
        		if(clock_gettime(CLOCK_MONOTONIC, &timer) == -1)
			    {
			    	perror("clock_gettime");
			    	exit(1);
			    }
        		new_part.t_in_created.sec = timer.tv_sec;
        		new_part.t_in_created.nsec = timer.tv_nsec;
        		new_part.id = rand() % (9999 + 1 - 1000) + 1000;//get a new id in range 1000~9999
        		if(getpid() == Array_shm_ptr[0])//construct parts type1
        			new_part.type = 1;
        		else if(getpid() == Array_shm_ptr[1])//construct parts type2
        			new_part.type = 2;
        		else if(getpid() == Array_shm_ptr[2])//construct parts type3
        			new_part.type = 3;

        		if(semop(p_empty, &down_p_empty, 1) == -1)
        		{
        			perror("semop");
		        	exit(1);
        		}
			    if(semop(paint_mux, &down_paint_mux, 1) == -1)
			    {
			    	perror("semop");
		        	exit(1);
			    }
			    //CONSTRUCTOR PAINT ENTRY//
			    if(clock_gettime(CLOCK_MONOTONIC, &timer) == -1)
		    	{
		    		perror("clock_gettime");
		    		exit(1);
		    	}
        		new_part.t_out_created.sec = timer.tv_sec;
        		new_part.t_out_created.nsec = timer.tv_nsec;
        		Paint_shm_ptr->id = new_part.id;
        		Paint_shm_ptr->type = new_part.type;
        		Paint_shm_ptr->t_in_created.sec = new_part.t_in_created.sec;
        		Paint_shm_ptr->t_in_created.nsec = new_part.t_in_created.nsec;
        		Paint_shm_ptr->t_out_created.sec = new_part.t_out_created.sec;
        		Paint_shm_ptr->t_out_created.nsec = new_part.t_out_created.nsec;
        		//CONSTRUCTOR PAINT EXIT//
        		if(semop(paint_mux, &up_paint_mux, 1) == -1)
        		{
        			perror("semop");
		        	exit(1);
        		}
			    if(semop(p_full, &up_p_full, 1) == -1)
			    {
			    	perror("semop");
		        	exit(1);
			    }
        	}

        }
        else if(getpid() == Array_shm_ptr[3])
        {
        	/******************************************/
			//			Painting Stage				  //
			/******************************************/
            while(total_items < (3 * Y))
            {
				total_items ++;
				if(semop(p_full, &down_p_full, 1) == -1)
				{
					perror("semop");
		        	exit(1);
				}
			    if(semop(paint_mux, &down_paint_mux, 1) == -1)
			    {
			    	perror("semop");
		        	exit(1);
			    }
				//PAINTER PAINT ENTRY
				if(clock_gettime(CLOCK_MONOTONIC, &timer) == -1)
				{
				    perror("clock_gettime");
				    exit(1);
				}
	            Paint_shm_ptr->t_in_painted.sec = timer.tv_sec;
	            Paint_shm_ptr->t_in_painted.nsec = timer.tv_nsec;
	            paint.tv_sec = 0;//we wont use seconds
	            if(Paint_shm_ptr->type == 1)//part1
				    paint.tv_nsec = 2000000;//paint for 2.0ms = 2.000.000 * 1ns
	            else if(Paint_shm_ptr->type == 2)//part2
				    paint.tv_nsec = 3000000;//paint for 3.0ms = 3.000.000 * 1ns
	            else if(Paint_shm_ptr->type == 3)//part 3
				    paint.tv_nsec = 5000000;//paint for 5.0ms = 5.000.000 * 1ns
				if(clock_nanosleep(CLOCK_MONOTONIC, 0, &paint, NULL) != 0)
	        	{
	        		perror("clock_nanosleep");
	        		exit(1);
	        	}
	            if(clock_gettime(CLOCK_MONOTONIC, &timer) == -1)
			    {
			    	perror("clock_gettime");
			    	exit(1);
			    }
			    Paint_shm_ptr->t_out_painted.sec = timer.tv_sec;
	        	Paint_shm_ptr->t_out_painted.nsec = timer.tv_nsec;
	        	//PAINTER PAINT EXIT
	        	if(semop(paint_mux, &up_paint_mux, 1) == -1)
	        	{
	        		perror("semop");
		        	exit(1);
	        	}
	        	if(semop(check_mux, &up_check_mux, 1) == -1)
	        	{
	        		perror("semop");
		        	exit(1);
	        	}
            }
        }
        else if(getpid() == Array_shm_ptr[4] || getpid() == Array_shm_ptr[5] || getpid() == Array_shm_ptr[6])
        {
        	/******************************************/
			//			Flaw-Check Stage 			  //
			/******************************************/
        	while(total_items < Y)
        	{
        		if(semop(check_mux, &down_check_mux, 1) == -1)
        		{
        			perror("semop");
		        	exit(1);
        		}
        		if(semop(paint_mux, &down_paint_mux, 1) == -1)
        		{
        			perror("semop");
		        	exit(1);
        		}
        		//CHECKER PAINT ENTRY
        		if(getpid() == Array_shm_ptr[4])//i check parts type1
        		{
        			if(Paint_shm_ptr->type == 1)//i found my part1//changed
        			{
        				total_items++;
        				if(clock_gettime(CLOCK_MONOTONIC, &timer) == -1)
					    {
						    perror("clock_gettime");
						    exit(1);
						}
						checked_part.t_in_checked.sec = timer.tv_sec;
						checked_part.t_in_checked.nsec = timer.tv_nsec;
						check.tv_sec = 0;//we wont use seconds
        				check.tv_nsec = 1000000;//sleep for 1ms = 1.000.000 * 1ns
        				if(clock_nanosleep(CLOCK_MONOTONIC, 0, &check, NULL) != 0)
			        	{
			        		perror("clock_nanosleep");
			        		exit(1);
			        	}
			        	checked_part.id = Paint_shm_ptr->id;
						checked_part.type = Paint_shm_ptr->type;
						checked_part.t_in_created.sec = Paint_shm_ptr->t_in_created.sec;
						checked_part.t_in_created.nsec = Paint_shm_ptr->t_in_created.nsec;
						checked_part.t_out_created.sec = Paint_shm_ptr->t_out_created.sec;
						checked_part.t_out_created.nsec = Paint_shm_ptr->t_out_created.nsec;
						checked_part.t_in_painted.sec = Paint_shm_ptr->t_in_painted.sec;
						checked_part.t_in_painted.nsec = Paint_shm_ptr->t_in_painted.nsec;
						checked_part.t_out_painted.sec = Paint_shm_ptr->t_out_painted.sec;
						checked_part.t_out_painted.nsec = Paint_shm_ptr->t_out_painted.nsec;
						if(semop(a_empty, &down_a_empty, 1) == -1)
			        	{
			        		perror("semop");
				        	exit(1);
			        	}
			        	if(semop(assemle_mux, &down_assemble_mux, 1) == -1)
			        	{
			        		perror("semop");
				        	exit(1);
			        	}
			        	//CHECKER ASSEMBLE ENTRY
			        	Assemble_shm_ptr->id = checked_part.id;
			        	Assemble_shm_ptr->type = checked_part.type;
			        	Assemble_shm_ptr->t_in_created.sec = checked_part.t_in_created.sec;
			        	Assemble_shm_ptr->t_in_created.nsec = checked_part.t_in_created.nsec;
			        	Assemble_shm_ptr->t_out_created.sec = checked_part.t_out_created.sec;
			        	Assemble_shm_ptr->t_out_created.nsec = checked_part.t_out_created.nsec;
			        	Assemble_shm_ptr->t_in_painted.sec = checked_part.t_in_painted.sec;
			        	Assemble_shm_ptr->t_in_painted.nsec = checked_part.t_in_painted.nsec;
			        	Assemble_shm_ptr->t_out_painted.sec = checked_part.t_out_painted.sec;
			        	Assemble_shm_ptr->t_out_painted.nsec = checked_part.t_out_painted.nsec;
			        	Assemble_shm_ptr->t_in_checked.sec = checked_part.t_in_checked.sec;
			        	Assemble_shm_ptr->t_in_checked.nsec = checked_part.t_in_checked.nsec;
			        	if(clock_gettime(CLOCK_MONOTONIC, &timer) == -1)
					    {
						    perror("clock_gettime");
						    exit(1);
						}
			        	Assemble_shm_ptr->t_out_checked.sec = timer.tv_sec;
			        	Assemble_shm_ptr->t_out_checked.nsec = timer.tv_nsec;
			        	//CHECKER ASSEMBLE EXIT 
			        	if(semop(assemle_mux, &up_assemble_mux, 1) == -1)
			        	{
			        		perror("semop");
				        	exit(1);
			        	} 
			        	if(semop(a_full, &up_a_full, 1) == -1)
			        	{
			        		perror("semop");
				        	exit(1);
			        	}
			        	//CHECKER PAINT EXIT
			        	if(semop(paint_mux, &up_paint_mux, 1) == -1)
			        	{
			        		perror("semop");
				        	exit(1);
			        	}
			        	if(semop(p_empty, &up_p_empty, 1) == -1)
		        		{
		        			perror("semop");
				        	exit(1);
		        		}
        			}
        			else//not my part1
        			{
        				if(semop(paint_mux, &up_paint_mux, 1) == -1)
		        		{
		        			perror("semop");
				        	exit(1);
		        		}
		        		if(semop(check_mux, &up_check_mux, 1) == -1)
		        		{
		        			perror("semop");
				        	exit(1);
		        		}
        			}
        			//CHECKER PAINT EXIT
        		}
	        	else if(getpid() == Array_shm_ptr[5])//i check parts type2
	        	{
	        		if(Paint_shm_ptr->type == 2)//i found my part2
        			{
        				total_items++;
        				if(clock_gettime(CLOCK_MONOTONIC, &timer) == -1)
					    {
						    perror("clock_gettime");
						    exit(1);
						}
						checked_part.t_in_checked.sec = timer.tv_sec;
						checked_part.t_in_checked.nsec = timer.tv_nsec;
						check.tv_sec = 0;//we wont use seconds
        				check.tv_nsec = 1500000;//sleep for 1.5ms = 1.500.000 * 1ns
        				if(clock_nanosleep(CLOCK_MONOTONIC, 0, &check, NULL) != 0)
			        	{
			        		perror("clock_nanosleep");
			        		exit(1);
			        	}
			        	checked_part.id = Paint_shm_ptr->id;
						checked_part.type = Paint_shm_ptr->type;
						checked_part.t_in_created.sec = Paint_shm_ptr->t_in_created.sec;
						checked_part.t_in_created.nsec = Paint_shm_ptr->t_in_created.nsec;
						checked_part.t_out_created.sec = Paint_shm_ptr->t_out_created.sec;
						checked_part.t_out_created.nsec = Paint_shm_ptr->t_out_created.nsec;
						checked_part.t_in_painted.sec = Paint_shm_ptr->t_in_painted.sec;
						checked_part.t_in_painted.nsec = Paint_shm_ptr->t_in_painted.nsec;
						checked_part.t_out_painted.sec = Paint_shm_ptr->t_out_painted.sec;
						checked_part.t_out_painted.nsec = Paint_shm_ptr->t_out_painted.nsec;
						if(semop(a_empty, &down_a_empty, 1) == -1)
			        	{
			        		perror("semop");
				        	exit(1);
			        	}
			        	if(semop(assemle_mux, &down_assemble_mux, 1) == -1)
			        	{
			        		perror("semop");
				        	exit(1);
			        	}
			        	//CHECKER ASSEMBLE ENTRY
			        	Assemble_shm_ptr->id = checked_part.id;
			        	Assemble_shm_ptr->type = checked_part.type;
			        	Assemble_shm_ptr->t_in_created.sec = checked_part.t_in_created.sec;
			        	Assemble_shm_ptr->t_in_created.nsec = checked_part.t_in_created.nsec;
			        	Assemble_shm_ptr->t_out_created.sec = checked_part.t_out_created.sec;
			        	Assemble_shm_ptr->t_out_created.nsec = checked_part.t_out_created.nsec;
			        	Assemble_shm_ptr->t_in_painted.sec = checked_part.t_in_painted.sec;
			        	Assemble_shm_ptr->t_in_painted.nsec = checked_part.t_in_painted.nsec;
			        	Assemble_shm_ptr->t_out_painted.sec = checked_part.t_out_painted.sec;
			        	Assemble_shm_ptr->t_out_painted.nsec = checked_part.t_out_painted.nsec;
			        	Assemble_shm_ptr->t_in_checked.sec = checked_part.t_in_checked.sec;
			        	Assemble_shm_ptr->t_in_checked.nsec = checked_part.t_in_checked.nsec;
			        	if(clock_gettime(CLOCK_MONOTONIC, &timer) == -1)
					    {
						    perror("clock_gettime");
						    exit(1);
						}
			        	Assemble_shm_ptr->t_out_checked.sec = timer.tv_sec;
			        	Assemble_shm_ptr->t_out_checked.nsec = timer.tv_nsec;
			        	//CHECKER ASSEMBLE EXIT 
			        	if(semop(assemle_mux, &up_assemble_mux, 1) == -1)
			        	{
			        		perror("semop");
				        	exit(1);
			        	} 
			        	if(semop(a_full, &up_a_full, 1) == -1)
			        	{
			        		perror("semop");
				        	exit(1);
			        	}
			        	//CHECKER PAINT EXIT
			        	if(semop(paint_mux, &up_paint_mux, 1) == -1)
			        	{
			        		perror("semop");
				        	exit(1);
			        	}
			        	if(semop(p_empty, &up_p_empty, 1) == -1)
		        		{
		        			perror("semop");
				        	exit(1);
		        		}
        			}
        			else//not my part2
        			{
        				if(semop(paint_mux, &up_paint_mux, 1) == -1)
		        		{
		        			perror("semop");
				        	exit(1);
		        		}
		        		if(semop(check_mux, &up_check_mux, 1) == -1)
		        		{
		        			perror("semop");
				        	exit(1);
		        		}
        			}
        			//CHECKER PAINT EXIT
	        	}
	        	else if(getpid() == Array_shm_ptr[6])// i check parts type3
	        	{
	        		if(Paint_shm_ptr->type == 3)//i found my part3
        			{
        				total_items++;
        				if(clock_gettime(CLOCK_MONOTONIC, &timer) == -1)
					    {
						    perror("clock_gettime");
						    exit(1);
						}
						checked_part.t_in_checked.sec = timer.tv_sec;
						checked_part.t_in_checked.nsec = timer.tv_nsec;
						check.tv_sec = 0;//we wont use seconds
        				check.tv_nsec = 2500000;//sleep for 2.5ms = 2.500.000 * 1ns
        				if(clock_nanosleep(CLOCK_MONOTONIC, 0, &check, NULL) != 0)
			        	{
			        		perror("clock_nanosleep");
			        		exit(1);
			        	}
			        	checked_part.id = Paint_shm_ptr->id;
						checked_part.type = Paint_shm_ptr->type;
						checked_part.t_in_created.sec = Paint_shm_ptr->t_in_created.sec;
						checked_part.t_in_created.nsec = Paint_shm_ptr->t_in_created.nsec;
						checked_part.t_out_created.sec = Paint_shm_ptr->t_out_created.sec;
						checked_part.t_out_created.nsec = Paint_shm_ptr->t_out_created.nsec;
						checked_part.t_in_painted.sec = Paint_shm_ptr->t_in_painted.sec;
						checked_part.t_in_painted.nsec = Paint_shm_ptr->t_in_painted.nsec;
						checked_part.t_out_painted.sec = Paint_shm_ptr->t_out_painted.sec;
						checked_part.t_out_painted.nsec = Paint_shm_ptr->t_out_painted.nsec;
						if(semop(a_empty, &down_a_empty, 1) == -1)
			        	{
			        		perror("semop");
				        	exit(1);
			        	}
			        	if(semop(assemle_mux, &down_assemble_mux, 1) == -1)
			        	{
			        		perror("semop");
				        	exit(1);
			        	}
			        	//CHECKER ASSEMBLE ENTRY
			        	Assemble_shm_ptr->id = checked_part.id;
			        	Assemble_shm_ptr->type = checked_part.type;
			        	Assemble_shm_ptr->t_in_created.sec = checked_part.t_in_created.sec;
			        	Assemble_shm_ptr->t_in_created.nsec = checked_part.t_in_created.nsec;
			        	Assemble_shm_ptr->t_out_created.sec = checked_part.t_out_created.sec;
			        	Assemble_shm_ptr->t_out_created.nsec = checked_part.t_out_created.nsec;
			        	Assemble_shm_ptr->t_in_painted.sec = checked_part.t_in_painted.sec;
			        	Assemble_shm_ptr->t_in_painted.nsec = checked_part.t_in_painted.nsec;
			        	Assemble_shm_ptr->t_out_painted.sec = checked_part.t_out_painted.sec;
			        	Assemble_shm_ptr->t_out_painted.nsec = checked_part.t_out_painted.nsec;
			        	Assemble_shm_ptr->t_in_checked.sec = checked_part.t_in_checked.sec;
			        	Assemble_shm_ptr->t_in_checked.nsec = checked_part.t_in_checked.nsec;
			        	if(clock_gettime(CLOCK_MONOTONIC, &timer) == -1)
					    {
						    perror("clock_gettime");
						    exit(1);
						}
			        	Assemble_shm_ptr->t_out_checked.sec = timer.tv_sec;
			        	Assemble_shm_ptr->t_out_checked.nsec = timer.tv_nsec;
			        	//CHECKER ASSEMBLE EXIT 
			        	if(semop(assemle_mux, &up_assemble_mux, 1) == -1)
			        	{
			        		perror("semop");
				        	exit(1);
			        	} 
			        	if(semop(a_full, &up_a_full, 1) == -1)
			        	{
			        		perror("semop");
				        	exit(1);
			        	}
			        	//CHECKER PAINT EXIT
			        	if(semop(paint_mux, &up_paint_mux, 1) == -1)
			        	{
			        		perror("semop");
				        	exit(1);
			        	}
			        	if(semop(p_empty, &up_p_empty, 1) == -1)
		        		{
		        			perror("semop");
				        	exit(1);
		        		}
        			}
        			else//not my part3
        			{
        				if(semop(paint_mux, &up_paint_mux, 1) == -1)
		        		{
		        			perror("semop");
				        	exit(1);
		        		}
		        		if(semop(check_mux, &up_check_mux, 1) == -1)
		        		{
		        			perror("semop");
				        	exit(1);
		        		}
        			}
        			//CHECKER PAINT EXIT
	        	}
        	}
        }
        else if(getpid() == Array_shm_ptr[7])
        {
        	/******************************************/
			//			Assembling Stage 			  //
			/******************************************/
			//Initialise time variables
        	average_waiting_time.sec = 0;
	        average_waiting_time.nsec = 0;
	        average_total_time.sec = 0;
	        average_total_time.nsec = 0;
        	while(total_items < Y)//counts products
        	{	
	        	part_counter = 0;
	        	new_product.id = 0;
	        	while(part_counter < 3)//get a new part
	        	{
	        		if(semop(a_full, &down_a_full, 1) == -1)
	        		{
	        			perror("semop");
		        		exit(1);
	        		}
	        		if(semop(assemle_mux, &down_assemble_mux, 1))
	        		{
	        			perror("semop");
		        		exit(1);
	        		}
	        		//ASSEMBLER ASSEMBLE ENTRY
	        		if(clock_gettime(CLOCK_MONOTONIC, &timer) == -1)
				    {
					    perror("clock_gettime");
					    exit(1);
					}
					Assemble_shm_ptr->t_in_assembled.sec = timer.tv_sec;
					Assemble_shm_ptr->t_in_assembled.nsec = timer.tv_nsec;
	        		if(part_counter == 0)//is the first part of the product
	        		{
	        			t_first_part.sec = Assemble_shm_ptr->t_in_created.sec;
	        			t_first_part.nsec = Assemble_shm_ptr->t_in_created.nsec;
	        		}
	        		new_product.id += Assemble_shm_ptr->id;
	        		//calculate average waiting time to enter for painting for all the parts
	        		wait_paint.sec = (Assemble_shm_ptr->t_in_painted.sec - Assemble_shm_ptr->t_in_created.sec);
	        		wait_paint.nsec = (Assemble_shm_ptr->t_in_painted.nsec - Assemble_shm_ptr->t_in_created.nsec);
	        		if(wait_paint.nsec < 0)
	        		{
	        			wait_paint.sec --;
	        			wait_paint.nsec += 1000000000;
	        		}
	        		average_waiting_time.sec += (wait_paint.sec / (3 * Y));
	        		average_waiting_time.nsec += (wait_paint.nsec / (3 * Y));
	        		part_counter ++;
		        	//ASSEMBLY EXIT
		        	if(semop(assemle_mux, &up_assemble_mux, 1) == -1)
		        	{
		        		perror("semop");
		        		exit(1);
		        	}
		        	if(semop(a_empty, &up_a_empty, 1) == -1)
		        	{
		        		perror("semop");
		        		exit(1);
		        	}
	        	}
	        	assemble.tv_sec = 0;
	        	assemble.tv_nsec = 15000000;//sleep for 15 ms = 15.000.000 * 1ns
	        	if(clock_nanosleep(CLOCK_MONOTONIC, 0, &assemble, NULL) != 0)
	            {
	            	perror("clock_nanosleep");
	            	exit(1);
	            }
	        	if(clock_gettime(CLOCK_MONOTONIC, &timer) == -1)
			    {
				    perror("clock_gettime");
				    exit(1);
				}
				total_items ++;
				//calculate each product finish time
				new_product.prod_time.sec = (timer.tv_sec - t_first_part.sec);
				new_product.prod_time.nsec = (timer.tv_nsec - t_first_part.nsec);
				if(new_product.prod_time.nsec < 0)
				{
					new_product.prod_time.sec --;
					new_product.prod_time.nsec += 1000000000;
				}
				//calculate the average time to produce each product starting from the creation of its first part
				average_total_time.sec += (new_product.prod_time.sec / Y);
	        	average_total_time.nsec += (new_product.prod_time.nsec / Y);
        	}
        	//simulation finished
        	printf("Product line simulation finished!\nResults below :\n");
        	printf("Average waiting time for each part to be painted : %ld ns ~= %ld ms\n", average_waiting_time.nsec, average_waiting_time.nsec / 1000000);
        	printf("Average time for each product to be completed : %ld ns ~= %ld ms\n", average_total_time.nsec, average_total_time.nsec / 1000000);	
        }
	}
	//slave dies here
	if(retpid == 0)
		exit(0);
	//Master clears up
    if(retpid != 0)
    {//Waiting for the slaves to finish the simulation and clears up shared memory (after detatching it) and semaphores.
    	printf("Master: Waiting...\n");
        while(slaves-- > 0)
        	wait(NULL);
        printf("Master: All slaves are done...\n");
        if(shmdt((void *) Array_shm_ptr) == -1)
        {
        	perror("shmdt");
        	exit(1);
        }
        if(shmdt((void *) Paint_shm_ptr) == -1)
        {
        	perror("shmdt");
        	exit(1);
        }
        if(shmdt((void *) Assemble_shm_ptr) == -1)
        {
        	perror("shmdt");
        	exit(1);
        }
        printf("Master: Detached shared memory...\n");
        if(shmctl(Array_shm_id, IPC_RMID, NULL) == -1)
        {
        	perror("shmctl");
        	exit(1);
        }
        if(shmctl(Paint_shm_id, IPC_RMID, NULL) == -1)
        {
        	perror("shmctl");
        	exit(1);
        }
        if(shmctl(Assemble_shm_id, IPC_RMID, NULL) == -1)
        {
        	perror("shmctl");
        	exit(1);
        }
        printf("Master: Destroyed shared memory...\n");
        if(semctl(paint_mux, 0, IPC_RMID) == -1)
        {
        	perror("semctl");
        	exit(1);
        }
        if(semctl(p_empty, 0, IPC_RMID) == -1)
        {
        	perror("semctl");
        	exit(1);
        }
        if(semctl(p_full, 0, IPC_RMID) == -1)
        {
        	perror("semctl");
        	exit(1);
        }
        if(semctl(assemle_mux, 0, IPC_RMID) == -1)
        {
        	perror("semctl");
        	exit(1);
        }
        if(semctl(a_empty, 0, IPC_RMID) == -1)
        {
        	perror("semctl");
        	exit(1);
        }
        if(semctl(a_full, 0, IPC_RMID) == -1)
        {
        	perror("semctl");
        	exit(1);
        }
        if(semctl(check_mux, 0, IPC_RMID) == -1)
        {
        	perror("semctl");
        	exit(1);
        }
       	printf("Master: Removed the semaphores...\n");
        printf("Master: Exiting...\n");
        //control back to main
    }
}