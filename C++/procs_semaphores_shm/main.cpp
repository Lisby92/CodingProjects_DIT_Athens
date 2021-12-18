#include "helpers.hpp"
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdio>
#include <ctime>
#include <cstring>

// comment in macro below to print debug output to verify the program is working as expected
// OR comment out to print only simulation specific output
//#define DEBUG_OUTPUT

using std::cout;
using std::string;

union senum {    // semaphore union as suggested to be used in sys/sem.h for semctl arguments
    int               val;      /* Value for SETVAL */
    struct semid_ds * buff; ;   /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  * array;    /* Array for GETALL, SETALL */
    struct seminfo  * _buf;		/* Buffer for IPC_INFO */
} sem_attr;     // initial lock state

int main(int argc, char **argv) {
    string X;
    int K, N;
    int filelines;
    int biggestLine;

    if (argc == 4) {
        X = argv[1];
        filelines = countFileLines(X);
        if(filelines <= 0) {
            cout << "Please provide a non empty file\n";
            return -1;
        }
        #ifdef DEBUG_OUTPUT
        cout << "Number of lines in input file: " << filelines << '\n';
        #endif
        biggestLine = countBiggestLine(X);
        if(biggestLine > 100) {
            cout << "Length of biggest line in current file(" << biggestLine << ") exceeds the maximum of 100\n";
            return -2;
        }
        #ifdef DEBUG_OUTPUT
        cout << "Biggest line in input file: " << biggestLine << '\n';
        #endif
        K = atoi(argv[2]);
        if (K < 0) {
            cout << "Please give valid number of child-processes to spawn!(K > 0)\n";
            return -3;
        }

        N = atoi(argv[3]);
        if (N < 0) {
            cout << "Please give valid number of transactions that each child-process will execute!(N > 0)\n";
            return -4;
        }
    }
    else {
        cout << "Error : Please provide 3 parametres : X, K, N!\n";
        cout << "X : The file to be parsed\n";
        cout << "K : The number of child-processes to spawn\n";
        cout << "N: The number of transactions that each child-process will execute\n";
        return -5;
    }
    #ifdef DEBUG_OUTPUT
    cout << "I received the following parametres:\nX = " << X << "\nK = " << K << "\nN = " << N << "\n";
    #endif
    run(X, K, N, filelines, biggestLine);
    return 0;
}

void run(const string &X, const int &K, const int &N, const int &file_lines, const int &biggestLine) {
    // shared memory
    int     array_shm_id;
    int     req_shm_id;
    int     res_shm_id;
    int *   array_shm_ptr; // shared memory array containing each process id to distinguish between server and clients
    req_ptr req_shm_ptr;
    res_ptr res_shm_ptr;
    // semaphores
    int    req_mux;
    int    req_empty;
    int    req_full;
    int    res_mux;
    int    res_empty;
    int    res_full;
    // simulation specific and helpers
    int    count     = K + 1; // we create one more to be the server + K processes to be clients
    int    clients   = 0;     // used to count the processes that we will spawn
    int    total_txs = 0;
    int    cur_line;
    int    client_id;
    string cur_data;
    // time related
    struct 	timespec client_req_timer{};
    struct 	timespec client_res_timer{};
    timestamp	average_total_time;
    timestamp   cur_timestamp;
    /******************************************/
    //	Create shared memory area : Array 	  //
    /******************************************/
    array_shm_id = shmget(IPC_PRIVATE, count * sizeof(int), IPC_CREAT | 0666);
    if (array_shm_id < 0) {
        perror("shmget");
        exit(-6);
    }
    array_shm_ptr = (int *) shmat(array_shm_id, nullptr, 0);
    if ((int) *array_shm_ptr == -1) {
        perror("shmat");
        exit(-6);
    }
    /**********************************************/
    //	  Create shared memory area : Request     //
    /**********************************************/
    req_shm_id = shmget(IPC_PRIVATE, sizeof(client_request), IPC_CREAT | 0666);
    if (req_shm_id < 0) {
        perror("shmget");
        exit(-6);
    }
    req_shm_ptr = (req_ptr) shmat(req_shm_id, nullptr, 0);
    if (req_shm_ptr == (void *) -1) {
        perror("shmat");
        exit(-6);
    }
    /**********************************************/
    //	  Create shared memory area : Response     //
    /**********************************************/
    res_shm_id = shmget(IPC_PRIVATE, sizeof(server_response), IPC_CREAT | 0666);
    if (res_shm_id < 0) {
        perror("shmget");
        exit(-6);
    }
    res_shm_ptr = (res_ptr) shmat(res_shm_id, nullptr, 0);
    if (res_shm_ptr == (void *) -1) {
        perror("shmat");
        exit(-6);
    }
    /******************************************/
    //			Create Semaphores 			  //
    /******************************************/
    if ((req_mux = semget(IPC_PRIVATE, 1, 0660 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(-6);
    }
    sem_attr.val = 1;        //initial state = unlocked
    if (semctl(req_mux, 0, SETVAL, sem_attr) == -1) {
        perror("semctl SETVAL");
        exit(-6);
    }
    if ((req_empty = semget(IPC_PRIVATE, 1, 0660 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(-6);
    }
    sem_attr.val = 1;        //initial state = unlocked
    if (semctl(req_empty, 0, SETVAL, sem_attr) == -1) {
        perror("semctl SETVAL");
        exit(-6);
    }
    if ((req_full = semget(IPC_PRIVATE, 1, 0660 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(-6);
    }
    sem_attr.val = 0;        //initial state = locked
    if (semctl(req_full, 0, SETVAL, sem_attr) == -1) {
        perror("semctl SETVAL");
        exit(-6);
    }
    if ((res_mux = semget(IPC_PRIVATE, 1, 0660 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(-6);
    }
    sem_attr.val = 1;        //initial state = unlocked
    if (semctl(res_mux, 0, SETVAL, sem_attr) == -1) {
        perror("semctl SETVAL");
        exit(-6);
    }
    if ((res_empty = semget(IPC_PRIVATE, 1, 0660 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(-6);
    }
    sem_attr.val = 1;        //initial state = unlocked
    if (semctl(res_empty, 0, SETVAL, sem_attr) == -1) {
        perror("semctl SETVAL");
        exit(-6);
    }
    if ((res_full = semget(IPC_PRIVATE, 1, 0660 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(-6);
    }
    sem_attr.val = 0;        //initial state = locked
    if (semctl(res_full, 0, SETVAL, sem_attr) == -1) {
        perror("semctl SETVAL");
        exit(-6);
    }
    /******************************************/
    //			Initialise Sem_Ops	 		  //
    /******************************************/
    struct sembuf req_up_mux{};
    req_up_mux.sem_num = 0;
    req_up_mux.sem_op = 1;
    req_up_mux.sem_flg = 0;
    /*******************************************/
    struct sembuf req_down_mux{};
    req_down_mux.sem_num = 0;
    req_down_mux.sem_op = -1;
    req_down_mux.sem_flg = 0;
    /*******************************************/
    struct sembuf req_up_empty{};
    req_up_empty.sem_num = 0;
    req_up_empty.sem_op = 1;
    req_up_empty.sem_flg = 0;
    /*******************************************/
    struct sembuf req_down_empty{};
    req_down_empty.sem_num = 0;
    req_down_empty.sem_op = -1;
    req_down_empty.sem_flg = 0;
    /*******************************************/
    struct sembuf req_up_full{};
    req_up_full.sem_num = 0;
    req_up_full.sem_op = 1;
    req_up_full.sem_flg = 0;
    /*******************************************/
    struct sembuf req_down_full{};
    req_down_full.sem_num = 0;
    req_down_full.sem_op = -1;
    req_down_full.sem_flg = 0;
    /*******************************************/
    struct sembuf res_up_mux{};
    res_up_mux.sem_num = 0;
    res_up_mux.sem_op = 1;
    res_up_mux.sem_flg = 0;
    /*******************************************/
    struct sembuf res_down_mux{};
    res_down_mux.sem_num = 0;
    res_down_mux.sem_op = -1;
    res_down_mux.sem_flg = 0;
    /*******************************************/
    struct sembuf res_up_empty{};
    res_up_empty.sem_num = 0;
    res_up_empty.sem_op = 1;
    res_up_empty.sem_flg = 0;
    /*******************************************/
    struct sembuf res_down_empty{};
    res_down_empty.sem_num = 0;
    res_down_empty.sem_op = -1;
    res_down_empty.sem_flg = 0;
    /*******************************************/
    struct sembuf res_up_full{};
    res_up_full.sem_num = 0;
    res_up_full.sem_op = 1;
    res_up_full.sem_flg = 0;
    /*******************************************/
    struct sembuf res_down_full{};
    res_down_full.sem_num = 0;
    res_down_full.sem_op = -1;
    res_down_full.sem_flg = 0;
    /******************************************/
    //		  Spawn Server & Clients		  //
    /******************************************/
    pid_t retpid = 1;      // Force Server initially.
    while (count-- > 0 &&  // Only fork if limit not reached
           retpid > 0)     // AND is Server (clients will exit loop with retpid == 0).
    {
        // Adjust actual count if successful.
        if ((retpid = fork()) > 0) {
            array_shm_ptr[clients] = retpid;
            clients++;
        }
    }

    if(retpid == 0) { // processes here simulating the clients and server transacting with each other
        if(getpid() == array_shm_ptr[0]) { // server
            /******************************************/
            //			    Server Code 			  //
            /******************************************/
            while(total_txs < N * K) {
                total_txs++;
                if(semop(req_full, &req_down_full, 1) == -1) {
                    perror("semop");
                    exit(-6);
                }
                if(semop(req_mux, &req_down_mux, 1) == -1) {
                    perror("semop");
                    exit(-6);
                }
                //******** SERVER READS REQUEST *********//
                client_id = req_shm_ptr->client_id;
                cur_line = req_shm_ptr->requested_line;
                cur_data = getLine(X, cur_line);
                #ifdef DEBUG_OUTPUT
                cout << "Server reads request for line " << cur_line << " from client[" << client_id <<"]\nData retrieved is \"" << cur_data << "\"\n";
                #endif
                //******** SERVER SUBMITS RESPONSE *********//
                if(semop(res_empty, &res_down_empty, 1) == -1) {
                    perror("semop");
                    exit(-6);
                }
                if(semop(res_mux, &res_down_mux, 1) == -1) {
                    perror("semop");
                    exit(-6);
                }
                // store the response
                res_shm_ptr->client_id = client_id;
                strcpy(res_shm_ptr->response_data, cur_data.c_str());
                res_shm_ptr->time_req_submit.nsec = req_shm_ptr->time_req_submit.nsec;
                res_shm_ptr->time_req_submit.sec = req_shm_ptr->time_req_submit.sec;
                if(semop(res_mux, &res_up_mux, 1) == -1) {
                    perror("semop");
                    exit(-6);
                }
                if(semop(res_full, &res_up_full, 1) == -1) {
                    perror("semop");
                    exit(-6);
                }
                if(semop(req_mux, &req_up_mux, 1) == -1) {
                    perror("semop");
                    exit(-6);
                }
                if(semop(req_empty, &req_up_empty, 1) == -1) {
                    perror("semop");
                    exit(-6);
                }
            }
        }
        else { // clients
            /******************************************/
            //		    	Client Code 			  //
            /******************************************/
            srand((unsigned int)getpid());  //genuine seed for each client
            // initialise the average time
            average_total_time.nsec = 0;
            average_total_time.sec = 0;
            while(total_txs < N) {
                total_txs++;
                if(semop(req_empty, &req_down_empty, 1) == -1) {
                    perror("semop");
                    exit(-6);
                }
                if(semop(req_mux, &req_down_mux, 1) == -1) {
                    perror("semop");
                    exit(-6);
                }
                //******** CLIENT SUBMITS REQUEST *********//
                cur_line = requestRandLineIndex(file_lines);
                req_shm_ptr->client_id = getpid();
                req_shm_ptr->requested_line = cur_line;
                if(clock_gettime(CLOCK_MONOTONIC, &client_req_timer) == -1) {
                    perror("clock_gettime");
                    exit(-6);
                }
                req_shm_ptr->time_req_submit.sec = client_req_timer.tv_sec;
                req_shm_ptr->time_req_submit.nsec = client_req_timer.tv_nsec;
                #ifdef DEBUG_OUTPUT
                cout << "Client[" << getpid() << "] submits request for line [" << cur_line << "]\n";
                #endif
                if(semop(req_mux, &req_up_mux, 1) == -1) {
                    perror("semop");
                    exit(-6);
                }
                if(semop(req_full, &req_up_full, 1) == -1) {
                    perror("semop");
                    exit(-6);
                }
                if(semop(res_full, &res_down_full, 1) == -1) {
                    perror("semop");
                    exit(-6);
                }
                if(semop(res_mux, &res_down_mux, 1) == -1) {
                    perror("semop");
                    exit(-6);
                }
                //******** CLIENT READS RESPONSE *********//
                #ifdef DEBUG_OUTPUT
                cout << "Client[" << getpid() << "] reads response for client[" << res_shm_ptr->client_id << "] for line [" << cur_line << "] with data : \""<< res_shm_ptr->response_data << "\"\n";
                #endif
                if(clock_gettime(CLOCK_MONOTONIC, &client_res_timer) == -1) {
                    perror("clock_gettime");
                    exit(-6);
                }
                // compute the average request->response time on the fly
                cur_timestamp.nsec = client_res_timer.tv_nsec - res_shm_ptr->time_req_submit.nsec;
                cur_timestamp.sec  = client_res_timer.tv_sec - res_shm_ptr->time_req_submit.sec;
                if (cur_timestamp.nsec < 0) { // we changed second, adjust
                    cur_timestamp.sec --;
                    cur_timestamp.nsec += 1000000000;
                }
                average_total_time.nsec += cur_timestamp.nsec / N;
                average_total_time.sec += cur_timestamp.sec / N;
                if(semop(res_mux, &res_up_mux, 1) == -1) {
                    perror("semop");
                    exit(-6);
                }
                if(semop(res_empty, &res_up_empty, 1) == -1) {
                    perror("semop");
                    exit(-6);
                }
            }
            // print the statistics
            cout << "Client[" << getpid() << "] average request->response time in ms : " << (average_total_time.sec * 1000) + (average_total_time.nsec / 1000) << "\n";
        }
    }
    // client and server processes die here
    if(retpid == 0)
        exit(0);

    // Master clears up
    while(clients-- > 0)
        wait(nullptr);
    cout << "Master: All client requests have been processed by server ...\n";
    if(shmdt((void *) array_shm_ptr) == -1) {
        perror("shmdt");
        exit(-6);
    }
    if(shmdt((void *) req_shm_ptr) == -1) {
        perror("shmdt");
        exit(-6);
    }
    if(shmdt((void *) res_shm_ptr) == -1) {
        perror("shmdt");
        exit(-6);
    }
    cout << "Master: Detached shared memory ...\n";
    if(shmctl(array_shm_id, IPC_RMID, nullptr) == -1) {
        perror("shmctl");
        exit(-6);
    }
    if(shmctl(req_shm_id, IPC_RMID, nullptr) == -1) {
        perror("shmctl");
        exit(-6);
    }
    if(shmctl(res_shm_id, IPC_RMID, nullptr) == -1) {
        perror("shmctl");
        exit(-6);
    }
    cout << "Master: Destroyed shared memory ...\n";
    if(semctl(req_mux, 0, IPC_RMID) == -1) {
        perror("semctl");
        exit(-6);
    }
    if(semctl(req_empty, 0, IPC_RMID) == -1) {
        perror("semctl");
        exit(-6);
    }
    if(semctl(req_full, 0, IPC_RMID) == -1) {
        perror("semctl");
        exit(-6);
    }
    if(semctl(res_mux, 0, IPC_RMID) == -1) {
        perror("semctl");
        exit(-6);
    }
    if(semctl(res_empty, 0, IPC_RMID) == -1) {
        perror("semctl");
        exit(-6);
    }
    if(semctl(res_full, 0, IPC_RMID) == -1) {
        perror("semctl");
        exit(-6);
    }
    cout << "Master: Removed the semaphores ...\n";
    cout << "Master: Exiting ...\n";
}
