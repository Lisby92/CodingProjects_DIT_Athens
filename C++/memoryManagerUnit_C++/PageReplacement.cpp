/******************************************************************************
Implementation File : PageReplacement.cpp
Author Date     : V. Mavrommatis 27/12/2018
Purpose       : Simulates a MMU with a hash PT handling process memory requests
Revision      : < Vasileios_Mavrommatis - 2/1/2019 >
******************************************************************************/
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include "MemoryManagerUnit.h"
#define MAX_REFS 1000000   //10000 references to be send by each process we have 1.000.000 on each trace file
#define PAGE_SIZE 4096 // = page/frame size in bytes
using namespace std;
//this will be the shared memory struct used for IPC
typedef struct Requests
{
  int * address;//holds the virtual address created by the CPU
  int * offset;//holds the offsets
  char * type;//'R' = read, 'W' = write
  int source;//1 for process 1, 2 for process 2
  bool valid1;//valid data for process 1
  bool valid2;//valid data for process 2
  char * data1;//data for process 1
  char * data2;//data for process 2
}Requests;
//semaphore union
union senum
{ 
    int val;
    struct  semid_ds *buff;
    unsigned short *array;
} sem_attr; //initial lock value
//pointer to the requests shared struct
typedef Requests * requestsPtr;
//process uses the data it requested
void useData(char * fetchedData)
{
  //cout << "Process uses the data fetched from MMU!\n";
}
//main function
int main(int argc, char ** argv)
{
  int frames;//frames given
  int q;//blocks of references
  int k;//max PFs before flush
  int blocksize;//size of references block
  int count = 3;//slave processes to be spawned
  int slaves = 0;//starting by 0
  int mode;//read or write
  string line;//a line from the trace file
  string virtualPage;//the vpn part as string
  string pageOffset;//the page offset as string
  int key;//the calculated key from the string
  int reference;//counts references of each block
  int offset;//the calculated offset from the string
  int client;//the process that will send the requests
  //shared memory area handles
  int requestsShmId;
  requestsPtr reqShmPtr;
  int addressShmId;
  int offsetShmId;
  int typeShmId;
  int dataShmId1;
  int dataShmId2;
  int slaveShmId;
  int * slaveShmPtr;
  //semaphore handles
  int reqMux;
  int reqEmpty;
  int reqFull;
  srand(time(NULL));//initialise rand
  if(argc == 4)//arguments OK
  {
    k = atoi(argv[1]);
    frames = atoi(argv[2]);
    q = atoi(argv[3]);
    if(k <= 0)
    {
      cout << "Please give valid number of max PFs!(positive integer)\n";
      return 1;
    }
    else if(frames <= 0)
    {
      cout << "Please give valid number of available frames!(positive integer)\n";
      return 1;
    }
    else if(q <= 0)
    {
      cout << "Please give valid number of reference block size!(positive integer)\n";
      return 1;
    }
  }//reaching here input is ok
  else
  {
    cout << "Error : Simulation parametres are false!\n";
    return 1;
  }
  blocksize = MAX_REFS / q;
  //create shared memory area to pass requests to the MMU
  requestsShmId = shmget(IPC_PRIVATE, sizeof(Requests), IPC_CREAT | 0666);
  if(requestsShmId < 0)
  {
      perror("shmget");
      exit(1);
  }
  reqShmPtr = (requestsPtr) shmat(requestsShmId, NULL, 0);
  if(reqShmPtr == (void *)-1)
  {
      perror("shmat");
      exit(1);
  }
  //create shared memory area for adresses
  addressShmId = shmget(IPC_PRIVATE, blocksize*sizeof(int), IPC_CREAT | 0666);
  if(addressShmId < 0)
  {
      perror("shmget");
      exit(1);
  }
  reqShmPtr->address = (int *) shmat(addressShmId, NULL, 0);
  if((int ) *(reqShmPtr->address) == -1)
  {
      perror("shmat");
      exit(1);
  }
  //create shared memory area for offsets
  offsetShmId = shmget(IPC_PRIVATE, blocksize*sizeof(int), IPC_CREAT | 0666);
  if(offsetShmId < 0)
  {
      perror("shmget");
      exit(1);
  }
  reqShmPtr->offset = (int *) shmat(offsetShmId, NULL, 0);
  if((int ) *(reqShmPtr->offset) == -1)
  {
      perror("shmat");
      exit(1);
  }
  //create shared memory area for type
  typeShmId = shmget(IPC_PRIVATE, blocksize*sizeof(char), IPC_CREAT | 0666);
  if(typeShmId < 0)
  {
      perror("shmget");
      exit(1);
  }
  reqShmPtr->type = (char *) shmat(typeShmId, NULL, 0);
  if((char ) *(reqShmPtr->type) == -1)
  {
      perror("shmat");
      exit(1);
  }
  //create shared memory area for data1
  dataShmId1 = shmget(IPC_PRIVATE, blocksize*sizeof(char), IPC_CREAT | 0666);
  if(dataShmId1 < 0)
  {
      perror("shmget");
      exit(1);
  }
  reqShmPtr->data1 = (char *) shmat(dataShmId1, NULL, 0);
  if((char ) *(reqShmPtr->data1) == -1)
  {
      perror("shmat");
      exit(1);
  }
  //create shared memory area for data2
  dataShmId2 = shmget(IPC_PRIVATE, blocksize*sizeof(char), IPC_CREAT | 0666);
  if(dataShmId2 < 0)
  {
      perror("shmget");
      exit(1);
  }
  reqShmPtr->data2 = (char *) shmat(dataShmId2, NULL, 0);
  if((char ) *(reqShmPtr->data2) == -1)
  {
      perror("shmat");
      exit(1);
  }
  //create shared memory area to distinguish between the 2 processes
  slaveShmId = shmget(IPC_PRIVATE, 3*sizeof(int), IPC_CREAT | 0666);
  if(slaveShmId < 0)
  {
      perror("shmget");
      exit(1);
  }
  slaveShmPtr = (int *) shmat(slaveShmId, NULL, 0);
  if((int ) *slaveShmPtr == -1)
  {
      perror("shmat");
      exit(1);
  }
  //create the semaphores shared by slaves : {0,1,2}
  if((reqMux = semget(IPC_PRIVATE, 1, 0660 | IPC_CREAT)) == -1)
  {
    perror("semget");
    exit(1);
  }
  sem_attr.val = 1;        //initial state = unlocked
  if(semctl(reqMux, 0, SETVAL, sem_attr) == -1)
  {
    perror("semctl SETVAL");
    exit(1);
  }
  if((reqEmpty = semget(IPC_PRIVATE, 1, 0660 | IPC_CREAT)) == -1)
  {
    perror("semget");
    exit(1);
  }
  sem_attr.val = 1;        //initial state = unlocked
  if(semctl(reqEmpty, 0, SETVAL, sem_attr) == -1)
  {
    perror("semctl SETVAL");
    exit(1);
  }
  if((reqFull = semget(IPC_PRIVATE, 1, 0660 | IPC_CREAT)) == -1)
  {
    perror("semget");
    exit(1);
  }
  sem_attr.val = 0;        //initial state = locked
  if(semctl(reqFull, 0, SETVAL, sem_attr) == -1)
  {
    perror("semctl SETVAL");
    exit(1);
  }
  //initialise sem_ops
  struct sembuf upReqMux;
  upReqMux.sem_num = 0;
  upReqMux.sem_op = 1;
  upReqMux.sem_flg = 0;
  /*******************************************/
  struct sembuf downReqMux;
  downReqMux.sem_num = 0;
  downReqMux.sem_op = -1;
  downReqMux.sem_flg = 0;
  /*******************************************/
  struct sembuf upReqEmpty;
  upReqEmpty.sem_num = 0;
  upReqEmpty.sem_op = 1;
  upReqEmpty.sem_flg = 0;
  /*******************************************/
  struct sembuf downReqEmpty;
  downReqEmpty.sem_num = 0;
  downReqEmpty.sem_op = -1;
  downReqEmpty.sem_flg = 0;
  /*******************************************/
  struct sembuf upReqFull;
  upReqFull.sem_num = 0;
  upReqFull.sem_op = 1;
  upReqFull.sem_flg = 0;
  /*******************************************/
  struct sembuf downReqFull;
  downReqFull.sem_num = 0;
  downReqFull.sem_op = -1;
  downReqFull.sem_flg = 0;
  /*******************************************/
  //spawn the slaves
  pid_t  retpid = 1;// Force Master initially.
  while(count-- > 0 && retpid > 0)// Only fork if limit not reached AND is Master
  {// Adjust actual count if successful.
    if((retpid = fork()) > 0)
    {
      slaveShmPtr[slaves] = retpid;//add pid in array
      slaves++;
    }
  }
  //simulation starts
  count = 0;
  reqShmPtr->valid1 = false;
  reqShmPtr->valid2 = false;
  if(retpid == 0)//slaves code here
  {
    if(getpid() == slaveShmPtr[0] || getpid() == slaveShmPtr[1])
    { //producer processes
      if(getpid() == slaveShmPtr[0])//process 1
      {
        ifstream in("bzip.trace");//open stream
        //cout << "hello from process 1 with id " << getpid() << "\n";
        while(count < q)
        {
          count++;//set next reference block
          if(semop(reqEmpty, &downReqEmpty, 1) == -1)
          {
            perror("semop");
            exit(1);
          }
          if(semop(reqMux, &downReqMux, 1) == -1)
          {
            perror("semop");
              exit(1);
          }
          //Start of CS
          //cout << "\nprocess 1 got in\n\n";
          reference = 0;
          reqShmPtr->source = 1;//sign it
          if(reqShmPtr->valid1 == true)
          {
            useData(reqShmPtr->data1);
            reqShmPtr->valid1 = false;
          }
          while(reference < blocksize)//more requests to be filled
          {
            getline(in, line);
            mode = line[9];//get request type as ascii, 82 = R, 87 = W
            virtualPage = line.substr(0,5);//get virtual page number
            pageOffset = line.substr(5,3);//get offset
            key = stoi(virtualPage, nullptr, 16);//convert to decimal integer
            offset = stoi(pageOffset, nullptr, 16);//convert to decimal integer
            reqShmPtr->address[reference] = key;
            reqShmPtr->type[reference] = mode;
            reqShmPtr->offset[reference] = offset;
            reference++;
          }
          //End of CS
          if(semop(reqMux, &upReqMux, 1) == -1)
          {
            perror("semop");
            exit(1);
          }
          if(semop(reqFull, &upReqFull, 1) == -1)
          {
            perror("semop");
              exit(1);
          }
        }
      }
      else if(getpid() == slaveShmPtr[1])//process 2
      {
        ifstream in("gcc.trace");//open stream
        //cout << "hello from process 2 with id " << getpid() << "\n";
        while(count < q)
        {
          count++;//set next reference block
          if(semop(reqEmpty, &downReqEmpty, 1) == -1)
          {
            perror("semop");
            exit(1);
          }
          if(semop(reqMux, &downReqMux, 1) == -1)
          {
            perror("semop");
              exit(1);
          }
          //Start of CS
          //cout << "process 2 got in\n";
          reference = 0;
          reqShmPtr->source = 2;//sign it
          if(reqShmPtr->valid2 == true)
          {
            useData(reqShmPtr->data2);
            reqShmPtr->valid2 = false;
          }
          while(reference < blocksize)
          {
            getline(in, line);
            mode = line[9];//get request type as ascii, 82 = R, 87 = W
            virtualPage = line.substr(0,5);//get virtual page number
            pageOffset = line.substr(5,3);//get offset
            key = stoi(virtualPage, nullptr, 16);//convert to decimal integer
            offset = stoi(pageOffset, nullptr, 16);;//convert to decimal integer
            reqShmPtr->address[reference] = key;
            reqShmPtr->type[reference] = mode;
            reqShmPtr->offset[reference] = offset;
            reference++;
          }
          //End of CS
          if(semop(reqMux, &upReqMux, 1) == -1)
          {
            perror("semop");
            exit(1);
          }
          if(semop(reqFull, &upReqFull, 1) == -1)
          {
            perror("semop");
              exit(1);
          }
        }
      }
    }
    else if(getpid() == slaveShmPtr[2])//MM process (consumer)
    {
      char byte;
      MemoryManagerUnit mmu(PAGE_SIZE, frames, k); //create the MMU
      while(count < q * 2)//handle q request blocks from each process
      {
        if(semop(reqFull, &downReqFull, 1) == -1)
        {
          perror("semop");
              exit(1);
        }
        if(semop(reqMux, &downReqMux, 1) == -1)
        {
          perror("semop");
            exit(1);
        }
        //Start of CS
        //cout << "\nMMU will read your requests now !\n";
        reference = 0;
        client = reqShmPtr->source;
        //cout << "SOURCE == PROCESS[" << client << "]!\n\n";
        while(reference < blocksize)
        {
          key = reqShmPtr->address[reference];
          mode = reqShmPtr->type[reference];
          offset = reqShmPtr->offset[reference];
          byte = mmu.operate(key, mode, offset, client);
          if(client == 1)
            reqShmPtr->data1[reference] = byte;
          else if(client == 2)
            reqShmPtr->data2[reference] = byte;
          reference++;//get next
        }
        if(client == 1)
          reqShmPtr->valid1 = true;
        else if(client == 2)
          reqShmPtr->valid2 = true;
        //End of CS
        if(semop(reqMux, &upReqMux, 1) == -1)
        {
          perror("semop");
          exit(1);
        }
        if(semop(reqEmpty, &upReqEmpty, 1) == -1)
        {
          perror("semop");
          exit(1);
        }
        count++;//read next reference block
      }
      mmu.find();
      mmu.print();
    }
  }
  //simulation ends
  if(retpid == 0)//slaves die here
    exit(0);

  if(retpid != 0)//master here
  {
    cout << "Master: Waiting...\n";
      while(slaves-- > 0)
        wait(NULL);
    cout << "Master: All slaves are done...\n";
    //remove memory and semaphores
    if(shmdt((void *) reqShmPtr->type) == -1)
    {
      perror("shmdt");
      exit(1);
    }
    if(shmdt((void *) reqShmPtr->address) == -1)
    {
      perror("shmdt");
      exit(1);
    }
    if(shmdt((void *) reqShmPtr->offset) == -1)
    {
      perror("shmdt");
      exit(1);
    }
    if(shmdt((void *) reqShmPtr->data1) == -1)
    {
      perror("shmdt");
      exit(1);
    }
    if(shmdt((void *) reqShmPtr->data2) == -1)
    {
      perror("shmdt");
      exit(1);
    }
    if(shmdt((void *) reqShmPtr) == -1)
    {
      perror("shmdt");
      exit(1);
    }
    if(shmdt((void *) slaveShmPtr) == -1)
    {
      perror("shmdt");
      exit(1);
    }
    cout << "Master: Detached shared memory...\n";
    if(shmctl(typeShmId, IPC_RMID, NULL) == -1)
    {
      perror("shmctl");
      exit(1);
    }
    if(shmctl(addressShmId, IPC_RMID, NULL) == -1)
    {
      perror("shmctl");
      exit(1);
    }
    if(shmctl(offsetShmId, IPC_RMID, NULL) == -1)
    {
      perror("shmctl");
      exit(1);
    }
    if(shmctl(dataShmId1, IPC_RMID, NULL) == -1)
    {
      perror("shmctl");
      exit(1);
    }
    if(shmctl(dataShmId2, IPC_RMID, NULL) == -1)
    {
      perror("shmctl");
      exit(1);
    }
    if(shmctl(requestsShmId, IPC_RMID, NULL) == -1)
    {
      perror("shmctl");
      exit(1);
    }
    if(shmctl(slaveShmId, IPC_RMID, NULL) == -1)
    {
      perror("shmctl");
      exit(1);
    }
    cout << "Master: Destroyed shared memory...\n";
    if(semctl(reqMux, 0, IPC_RMID) == -1)
    {
      perror("semctl");
      exit(1);
    }
    if(semctl(reqFull, 0, IPC_RMID) == -1)
    {
      perror("semctl");
      exit(1);
    }
    if(semctl(reqEmpty, 0, IPC_RMID) == -1)
    {
      perror("semctl");
      exit(1);
    }
    cout << "Master: Destroyed the semaphores...\n";
    cout << "Master: Exiting...\n";
  }//master dies
  return 0; 
}