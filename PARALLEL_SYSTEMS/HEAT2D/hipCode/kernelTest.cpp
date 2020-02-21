#include <assert.h>
#include <sys/time.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <algorithm>

#include "hip/hip_runtime.h"

using std::cout;
using std::endl;
using std::cerr;

#define ROWS               20                 /* y dimension of problem grid */
#define COLS               20                 /* x dimension of problem grid */
#define DECLINE_HORIZONTAL 0.1
#define DECLINE_VERTICAL   0.1
#define PROB_SIZE          (ROWS * COLS)      /* grid data size */
#define STEPS              100               /* number of time steps */
#define THREADS_PER_BLOCK  1
#define BLOCKS_PER_GRID    1
#define TOTAL_THREADS      (THREADS_PER_BLOCK * BLOCKS_PER_GRID) /* total hip threads*/

/* error check on hip macro */
#define HIP_CHECK(command) \
{ \
  hipError_t status = command; \
  if(status != hipSuccess) \
  { \
    cerr << "Error : HIP reports " << hipGetErrorString(status) << endl; \
    exit(-6); \
  } \
}

/* device(GPU) functions to be called from __global__ or other
   __device__ functions (ignored during host compilation) */

__device__ void heatSwap(double ** __restrict__ a, double ** __restrict__ b)
{
  double * temp = *a;
  *a = *b;
  *b = temp;
}

/* device(GPU) functions to be launched as a kernel from host(CPU) */

/* This function updates all grid areas and is invoked on serial executions */
__global__ void heatUpdateSerial(double * __restrict__ oldBlock,
                                 double * __restrict__ newBlock)
{
  for(int k = 0; k < STEPS; ++k)
  {
    for(int i = 1; i < ROWS - 1; ++i)
    {
      for(int j = 1; j < COLS - 1; ++j)
      {
        newBlock[i*COLS+j] = oldBlock[i*COLS+j] +
        DECLINE_HORIZONTAL *
        (oldBlock[(i+1)*COLS+j] + oldBlock[(i-1)*COLS+j] - (2 * oldBlock[i*COLS+j])) +
        DECLINE_VERTICAL *
        (oldBlock[i*COLS+j+1] + oldBlock[i*COLS+j-1] - (2 * oldBlock[i*COLS+j]));
      }
    }
    /*
    //heatSwap(oldBlock, newBlock);
    for(int i = 1; i < ROWS - 1; ++i)
      for(int j = 1; j < COLS - 1; ++j)
        oldBlock[i*COLS+j] = newBlock[i*COLS+j];
    */
    double * temp = oldBlock;
    oldBlock = newBlock;
    newBlock = temp;
    
  }
}

/* host(CPU) functions */

/* This function intialises the temperature on the given grid with higher
   temperatures at the centre, progressively lower ones until the sides 
   and 0s at the perimetre 
*/
static inline void heatInit(double * oldBlock, double * newBlock)
{
  for(int i = 0; i < ROWS; ++i)//avoid halo area
  {
    for(int j = 0; j < COLS; ++j)
    {
      oldBlock[i*COLS+j] = (double) (i * (ROWS - i - 1) * j * (COLS - j - 1));
      newBlock[i*COLS+j] = 0.0;
    }
  }
}

/* This function swaps the 2 grid's pointers to avoid assignmenets */
/*
static inline void heatSwap(double ** a, double ** b)
{
  double * temp = *a;
  *a = *b;
  *b = temp;
}*/

/* This function writes out the input grid to a .dat file in current path */
static inline int heatWriteSerial(const double * grid, const int flag)
{
  char filePath[70] = "";
  if(flag == 0)
    sprintf(filePath, "%d_%d_hip_sk_Initial.dat", ROWS, COLS);
  else
    sprintf(filePath, "%d_%d_hip_sk_Final.dat", ROWS, COLS);

  FILE * fp = fopen(filePath, "w");
  if(fp == nullptr)
    return -1;
  for(int i = 0; i < ROWS; ++i)
  {
    for(int j = 0; j < COLS; ++j)
    {
      fprintf(fp, "%-.1lf", grid[i*COLS+j]);
      if(j != (COLS - 1))
        fprintf(fp, " ");
    }
    fprintf(fp, "\n");
  }
  fclose(fp);
  return 0;//all ok
}

/* Main program function */
int main(int argc, char *argv[])
{
  //get the properties
  hipDeviceProp_t devProp;
  hipGetDeviceProperties(&devProp, 0);
  cout << "System minor " << devProp.minor << endl;
  cout << "System major " << devProp.major << endl;
  cout << "Agent Prop Name " << devProp.name << endl;
  cout << "Hip Device prop succeeded" << endl ;

  //allocate and initialise host(CPU) memory
  if(TOTAL_THREADS == 1)//serial execution
  {
    cout << "SERIAL EXECUTION WITH 1 HIP THREAD\n";
    double * oldBlock = (double *) malloc(ROWS * COLS * sizeof(double));
    if(oldBlock == nullptr)
    {
      cerr << "Error, not enough memory...\nAborting...\n";
      exit(1);
    }
    double * newBlock = (double *) malloc(ROWS * COLS * sizeof(double));
    if(newBlock == nullptr)
    {
      cerr << "Error, not enough memory...\nAborting...\n";
      free(oldBlock);
      oldBlock = nullptr;
      exit(2);
    }
    heatInit(oldBlock, newBlock);

    //write out the initial grid to the corresponding file
    if(heatWriteSerial(oldBlock, 0) == -1)
    {
      cerr << "Error, could not create the initial file...\nAborting...\n";
      free(oldBlock);
      oldBlock = nullptr;
      free(newBlock);
      newBlock = nullptr;
      exit(3);
    }

    //allocate and initialise device(GPU) memory
    double * devOldBlock = nullptr;
    double * devNewBlock = nullptr;
    HIP_CHECK(hipMalloc((void **)&devOldBlock, PROB_SIZE * sizeof(double)));
    HIP_CHECK(hipMalloc((void **)&devNewBlock, PROB_SIZE * sizeof(double)));

    //transfer data from host(CPU) to device(GPU) memory
    HIP_CHECK(hipMemcpy(devOldBlock, oldBlock, PROB_SIZE * sizeof(double),
                        hipMemcpyHostToDevice));
    HIP_CHECK(hipMemcpy(devNewBlock, newBlock, PROB_SIZE * sizeof(double),
                        hipMemcpyHostToDevice));

    //calculate the kernel dimensions (x,y,z) threads/block and blocks/grid
    //serial 1 thread X / 1 block X
    dim3 hipThreads(THREADS_PER_BLOCK, 1, 1);
    dim3 hipBlocks(BLOCKS_PER_GRID, 1 ,1);

    //start the timer
    struct timespec start;
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    //solve the problem
    //launch the kernel(kernel overhead??)
    hipLaunchKernelGGL(heatUpdateSerial, hipBlocks, hipThreads, 0, 0,
                       devOldBlock, devNewBlock);
    HIP_CHECK(hipDeviceSynchronize());
    //stop the timer
    clock_gettime(CLOCK_MONOTONIC, &end);
    const double totalTime = ((end.tv_sec - start.tv_sec) * 1000.0) +
                             ((end.tv_nsec - start.tv_nsec) / 1000000.0); 
    cout << "\nElapsed time was " << totalTime << " ms\n";

    //transfer data back to host(CPU) from device(GPU)
    if(STEPS % 2 == 0)//get the correct version
    {
      HIP_CHECK(hipMemcpy(newBlock, devOldBlock, PROB_SIZE * sizeof(double),
                          hipMemcpyDeviceToHost));
    }
    else
    {
      HIP_CHECK(hipMemcpy(newBlock, devNewBlock, PROB_SIZE * sizeof(double),
                          hipMemcpyDeviceToHost));
    }
    
    //write out the final grid to the corresponding file
    if(heatWriteSerial(newBlock, 1) == -1)
    {
      cerr << "Error, could not create the initial file...\nAborting...\n";
      HIP_CHECK(hipFree(devOldBlock));
      devOldBlock = nullptr;
      HIP_CHECK(hipFree(devNewBlock));
      devNewBlock = nullptr;
      free(oldBlock);
      oldBlock = nullptr;
      free(newBlock);
      newBlock = nullptr;
      exit(4);
    }
  }
  else//parallel execution
  {
    cout << "PARALLEL EXECUTION WITH " << TOTAL_THREADS << " HIP THREADS\n";
  }
  
  return 0;
}