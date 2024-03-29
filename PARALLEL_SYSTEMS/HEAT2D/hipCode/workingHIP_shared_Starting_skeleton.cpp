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
#define GRID_SIZE          (ROWS * COLS)      /* grid data size */
#define STEPS              100               /* number of time steps */
#define THREADS_PER_BLOCK  80
#define BLOCKS_PER_GRID    1
#define TOTAL_THREADS      (THREADS_PER_BLOCK * BLOCKS_PER_GRID) /* total hip threads*/

/* error check on hip macro */
#define HIP_CHECK(command) \
{ \
  hipError_t status = command; \
  if(status != hipSuccess) \
  { \
    cerr << "Error : HIP reports " << hipGetErrorString(status) << endl; \
    heatCleanExit(-6); \
  } \
}

/* Device(GPU) FUNCTIONS TO BE LAUNCHED AS KERNELS FROM HOST(CPU) */
/* This function is used to discover the local working area of each thread */
__global__ void heatDiscover(int * __restrict__ globalRowS,
                             int * __restrict__ globalRowE,
                             int * __restrict__ globalColS,
                             int * __restrict__ globalColE,
                             int * __restrict__ workRowS,
                             int * __restrict__ workRowE,
                             int * __restrict__ workColS,
                             int * __restrict__ workColE,
                             const int blockRows,
                             const int blockCols,
                             const int threadsPerRow,
                             const int threadsPerCol,
                             const int probCase)
{
  const int threadIdx = hipThreadIdx_x + hipBlockDim_x * hipBlockIdx_x;
  if(probCase == 1)//2 threads
  {
    if(threadIdx == 0)//west thread
    {
      globalRowS[threadIdx] = 0;
      globalRowE[threadIdx] = blockRows - 1;
      globalColS[threadIdx] = 0;
      globalColE[threadIdx] = blockCols - 1;
      workRowS[threadIdx] = 1;
      workRowE[threadIdx] = blockRows - 2;
      workColS[threadIdx] = 1;
      workColE[threadIdx] = blockCols - 1;
    }
    else//east thread
    {
      globalRowS[threadIdx] = 0;
      globalRowE[threadIdx] = blockRows - 1;
      globalColS[threadIdx] = COLS - blockCols;
      globalColE[threadIdx] = COLS - 1;
      workRowS[threadIdx] = 1;
      workRowE[threadIdx] = blockRows - 2;
      workColS[threadIdx] = blockCols;
      workColE[threadIdx] = COLS - 2;
    } 
  }
  else if(probCase == 2)//6, 8, 10, ... OR 4, 16, 64 ... threads
  {
    if(threadIdx == 0)//NW corner
    {
      globalRowS[threadIdx] = 0;
      globalRowE[threadIdx] = blockRows - 1;
      globalColS[threadIdx] = 0;
      globalColE[threadIdx] = blockCols - 1;
      workRowS[threadIdx] = 1;
      workRowE[threadIdx] = blockRows - 1;
      workColS[threadIdx] = 1;
      workColE[threadIdx] = blockCols - 1;
    }
    else if(threadIdx == (threadsPerCol - 1))//NE corner
    {
      globalRowS[threadIdx] = 0;
      globalRowE[threadIdx] = blockRows - 1;
      globalColS[threadIdx] = COLS - blockCols;
      globalColE[threadIdx] = COLS - 1;
      workRowS[threadIdx] = 1;
      workRowE[threadIdx] = blockRows - 1;
      workColS[threadIdx] = COLS - blockCols;
      workColE[threadIdx] = COLS - 2;
    }
    else if(threadIdx == ((threadsPerRow * threadsPerCol) - threadsPerCol))//SW corner
    {
      globalRowS[threadIdx] = ROWS - blockRows;
      globalRowE[threadIdx] = ROWS - 1;
      globalColS[threadIdx] = 0;
      globalColE[threadIdx] = blockCols - 1;
      workRowS[threadIdx] = ROWS - blockRows;
      workRowE[threadIdx] = ROWS - 2;
      workColS[threadIdx] = 1;
      workColE[threadIdx] = blockCols - 1;
    }
    else if(threadIdx == ((threadsPerCol * threadsPerRow) - 1))//SE corner
    {
      globalRowS[threadIdx] = ROWS - blockRows;
      globalRowE[threadIdx] = ROWS - 1;
      globalColS[threadIdx] = COLS - blockCols;
      globalColE[threadIdx] = COLS - 1;
      workRowS[threadIdx] = ROWS - blockRows;
      workRowE[threadIdx] = ROWS - 2;
      workColS[threadIdx] = COLS - blockCols;
      workColE[threadIdx] = COLS - 2;
    }
    else if(threadIdx < threadsPerCol)//NN side
    {
      globalRowS[threadIdx] = 0;
      globalRowE[threadIdx] = blockRows - 1;
      globalColS[threadIdx] = threadIdx * blockCols;
      globalColE[threadIdx] = globalColS[threadIdx] + blockCols - 1;
      workRowS[threadIdx] = 1;
      workRowE[threadIdx] = blockRows - 1;
      workColS[threadIdx] = globalColS[threadIdx];
      workColE[threadIdx] = globalColE[threadIdx];
    }
    else if((threadIdx > ((threadsPerCol * threadsPerRow) - threadsPerCol)) && 
            (threadIdx < ((threadsPerCol * threadsPerRow) - 1)))//SS side
    {
      globalRowS[threadIdx] = ROWS - blockRows;
      globalRowE[threadIdx] = ROWS - 1;
      globalColS[threadIdx] = (threadIdx % threadsPerCol) * blockCols;
      globalColE[threadIdx] = globalColS[threadIdx] + blockCols - 1;
      workRowS[threadIdx] = ROWS - blockRows;
      workRowE[threadIdx] = ROWS - 2;
      workColS[threadIdx] = globalColS[threadIdx];
      workColE[threadIdx] = globalColE[threadIdx];
    }
    else if((threadIdx % threadsPerCol) == 0)//WW side
    {
      globalRowS[threadIdx] = (threadIdx / threadsPerCol) * blockRows;
      globalRowE[threadIdx] = globalRowS[threadIdx] + blockRows - 1;
      globalColS[threadIdx] = 0;
      globalColE[threadIdx] = blockCols - 1;
      workRowS[threadIdx] = globalRowS[threadIdx];
      workRowE[threadIdx] = globalRowE[threadIdx];
      workColS[threadIdx] = 1;
      workColE[threadIdx] = blockCols - 1;
    }
    else if((threadIdx + 1) % threadsPerCol == 0)//EE side
    {
      globalRowS[threadIdx] = ((threadIdx + 1 - threadsPerCol) / threadsPerCol) *
                              blockRows;
      globalRowE[threadIdx] = globalRowS[threadIdx] + blockRows - 1;
      globalColS[threadIdx] = COLS - blockCols;
      globalColE[threadIdx] = COLS - 1;
      workRowS[threadIdx] = globalRowS[threadIdx];
      workRowE[threadIdx] = globalRowE[threadIdx];
      workColS[threadIdx] = COLS - blockCols;
      workColE[threadIdx] = COLS - 2;
    }
    else//general case middle location
    {
      int rowMarginS = threadsPerCol;
      int rowMarginE = rowMarginS + threadsPerCol - 1;
      int rowOffset = 1;
      while(1)
      {
        if((threadIdx > rowMarginS) && (threadIdx < rowMarginE))
        {
          globalRowS[threadIdx] = rowOffset * blockRows;
          break;
        }
        else
        {
          rowMarginS += threadsPerCol;
          rowMarginE += threadsPerCol;
          ++rowOffset;
        }
      }
      globalColS[threadIdx] = (threadIdx % threadsPerCol) * blockCols;
      globalRowE[threadIdx] = globalRowS[threadIdx] + blockRows - 1;
      globalColE[threadIdx] = globalColS[threadIdx] + blockCols - 1;
      workRowS[threadIdx] = globalRowS[threadIdx];
      workRowE[threadIdx] = globalRowE[threadIdx];
      workColS[threadIdx] = globalColS[threadIdx];
      workColE[threadIdx] = globalColE[threadIdx];
    }
  }
}

/* This function updates the grid and is invoked on serial executions */
__global__ void heatUpdateSerial(double * __restrict__ devHeatGrid)
{
  //declare dynamic shared memory
  HIP_DYNAMIC_SHARED(double, sharedHeatGrid)

  //copy global to shared memory once for faster access
  for(int i = 0; i < ROWS; ++i)
    for(int j = 0; j < COLS; ++j)
      sharedHeatGrid[i*COLS+j] = devHeatGrid[i*COLS+j];

  //solve the problem
  for(int steps = 0; steps < STEPS; ++steps)
  {
    for(int i = 1; i < ROWS - 1; ++i)
    {
      for(int j = 1; j < COLS - 1; ++j)
      {
        sharedHeatGrid[i*COLS+j+GRID_SIZE] = sharedHeatGrid[i*COLS+j] +
                                      DECLINE_HORIZONTAL *
                                      (sharedHeatGrid[(i+1)*COLS+j] +
                                      sharedHeatGrid[(i-1)*COLS+j] -
                                      (2 * sharedHeatGrid[i*COLS+j])) +
                                      DECLINE_VERTICAL *
                                      (sharedHeatGrid[i*COLS+j+1] +
                                      sharedHeatGrid[i*COLS+j-1] -
                                      (2 * sharedHeatGrid[i*COLS+j]));
      }
    }
    //old = new swap
    for(int i = 1; i < ROWS - 1; ++i)
      for(int j = 1; j < COLS - 1; ++j)
        sharedHeatGrid[i*COLS+j] = sharedHeatGrid[i*COLS+j+GRID_SIZE];
  }

  //copy shared memory back to global to access from host(CPU)
  for(int i = 0; i < (ROWS - 1); ++i)
    for(int j = 0; j < (COLS - 1); ++j)
      devHeatGrid[i*COLS+j] = sharedHeatGrid[i*COLS+j];    
}

/* This function updates the grid and is invoked on parallel executions */
__global__ void heatUpdateParallel(double       * __restrict__ devHeatGrid,
                                   const int    * __restrict__ globalRowS,
                                   const int    * __restrict__ globalRowE,
                                   const int    * __restrict__ globalColS,
                                   const int    * __restrict__ globalColE,
                                   const int    * __restrict__ workRowS,
                                   const int    * __restrict__ workRowE,
                                   const int    * __restrict__ workColS,
                                   const int    * __restrict__ workColE)
{
  //declare dynamic shared memory
  HIP_DYNAMIC_SHARED(double, sharedHeatGrid)

  const int threadIdx = hipThreadIdx_x + hipBlockDim_x * hipBlockIdx_x;
  //get the borders in registers for 1 cycle memory access
  const int glRowS = globalRowS[threadIdx];
  const int glRowE = globalRowE[threadIdx];
  const int glColS = globalColS[threadIdx];
  const int glColE = globalColE[threadIdx];
  const int wRowS = workRowS[threadIdx];
  const int wRowE = workRowE[threadIdx];
  const int wColS = workColS[threadIdx];
  const int wColE = workColE[threadIdx];

  //each thread copies its own part of data from the device global memory
  //to the shared block memory for 5 cycle memory accesses
  for(int i = glRowS; i <= glRowE; ++i)
    for(int j = glColS; j <= glColE; ++j)
      sharedHeatGrid[i*COLS+j] = devHeatGrid[i*COLS+j];

  //solve the problem
  for(int steps = 0; steps < STEPS; ++steps)
  {
    __syncthreads();
    for(int i = wRowS; i <= wRowE; ++i)
    {
      for(int j = wColS; j <= wColE; ++j)
      {
        sharedHeatGrid[i*COLS+j+GRID_SIZE] = sharedHeatGrid[i*COLS+j] +
                                             DECLINE_HORIZONTAL *
                                             (sharedHeatGrid[(i+1)*COLS+j] +
                                             sharedHeatGrid[(i-1)*COLS+j] -
                                             (2*sharedHeatGrid[i*COLS+j])) +
                                             DECLINE_VERTICAL *
                                             (sharedHeatGrid[i*COLS+j+1] +
                                             sharedHeatGrid[i*COLS+j-1] -
                                             (2*sharedHeatGrid[i*COLS+j]));
      }
    }
    //old = new
    __syncthreads();
    for(int i = glRowS; i <= glRowE; ++i)
      for(int j = glColS; j <= glColE; ++j)
        sharedHeatGrid[i*COLS+j] = sharedHeatGrid[i*COLS+j+GRID_SIZE];
  }

  //copy shared memory back to global to access from host(CPU)
  for(int i = glRowS; i <= glRowE; ++i)
    for(int j = glColS; j <= glColE; ++j)
      devHeatGrid[i*COLS+j] = sharedHeatGrid[i*COLS+j+GRID_SIZE];
  __syncthreads();
}                                      

//declare dynamic variables
double * heatGrid = nullptr;
double * devHeatGrid = nullptr;

int * globalRowS = nullptr;
int * globalRowE = nullptr;
int * globalColS = nullptr;
int * globalColE = nullptr;
int * devGlobalRowS = nullptr;
int * devGlobalRowE = nullptr;
int * devGlobalColS = nullptr;
int * devGlobalColE = nullptr;

int * workRowS = nullptr;
int * workRowE = nullptr;
int * workColS = nullptr;
int * workColE = nullptr;
int * devWorkRowS = nullptr;
int * devWorkRowE = nullptr;
int * devWorkColS = nullptr;
int * devWorkColE = nullptr;

/* HOST(CPU) FUNCTIONS */
/* This function intialises the temperature on the given grid with higher
   temperatures at the centre, progressively lower ones until the sides 
   and 0s at the perimetre 
*/
static inline void heatInit(double * heatGrid)
{
  for(int i = 0; i < ROWS; ++i)//avoid halo area
    for(int j = 0; j < COLS; ++j)
      heatGrid[i*COLS+j] = (double) (i * (ROWS - i - 1) * j * (COLS - j - 1));
}

/* This function writes out the input grid to a .dat file in current path */
static inline int heatWrite(const double * heatGrid, const int flag)
{
  char filePath[70] = "";
  if(flag == 0)
  {
    sprintf(filePath, "%d_%d_hip_%d_%d_Initial.dat", ROWS, COLS,
            THREADS_PER_BLOCK, BLOCKS_PER_GRID);
  }
  else
  {
    sprintf(filePath, "%d_%d_hip_%d_%d_Final.dat", ROWS, COLS,
            THREADS_PER_BLOCK, BLOCKS_PER_GRID);
  }

  FILE * fp = fopen(filePath, "w");
  if(fp == nullptr)
    return -1;
  for(int i = 0; i < ROWS; ++i)
  {
    for(int j = 0; j < COLS; ++j)
    {
      fprintf(fp, "%-.1lf", fabs(heatGrid[i*COLS+j]));//some 0.0s appear as -0.0s
      if(j != (COLS - 1))
        fprintf(fp, " ");
    }
    fprintf(fp, "\n");
  }
  fclose(fp);
  return 0;//all ok
}

/* This function cleans up memory to prevent leaks on any exit error */
static inline void heatCleanExit(const int errorCode)
{
  if(heatGrid != nullptr)
  {
    free(heatGrid);
    heatGrid = nullptr;
  }
  if(devHeatGrid != nullptr)
  {
    HIP_CHECK(hipFree(devHeatGrid));
    devHeatGrid = nullptr;
  }
  if(globalRowS != nullptr)
  {
    free(globalRowS);
    globalRowS = nullptr;
  }
  if(devGlobalRowS != nullptr)
  {
    HIP_CHECK(hipFree(devGlobalRowS));
    devGlobalRowS = nullptr;
  }
  if(globalRowE != nullptr)
  {
    free(globalRowE);
    globalRowE = nullptr;
  }
  if(devGlobalRowE != nullptr)
  {
    HIP_CHECK(hipFree(devGlobalRowE));
    devGlobalRowE = nullptr;
  }
  if(globalColS != nullptr)
  {
    free(globalColS);
    globalColS = nullptr;
  }
  if(devGlobalColS != nullptr)
  {
    HIP_CHECK(hipFree(devGlobalColS));
    devGlobalColS = nullptr;
  }
  if(globalColE != nullptr)
  {
    free(globalColE);
    globalColE = nullptr;
  }
  if(devGlobalColE != nullptr)
  {
    HIP_CHECK(hipFree(devGlobalColE));
    devGlobalColE = nullptr;
  }
  if(workRowS != nullptr)
  {
    free(workRowS);
    workRowS = nullptr;
  }
  if(devWorkRowS != nullptr)
  {
    HIP_CHECK(hipFree(devWorkRowS));
    devWorkRowS = nullptr;
  }
  if(workRowE != nullptr)
  {
    free(workRowE);
    workRowE = nullptr;
  }
  if(devWorkRowE != nullptr)
  {
    HIP_CHECK(hipFree(devWorkRowE));
    devWorkRowE = nullptr;
  }
  if(workColS != nullptr)
  {
    free(workColS);
    workColS = nullptr;
  }
  if(devWorkColS != nullptr)
  {
    HIP_CHECK(hipFree(devWorkColS));
    devWorkColS = nullptr;
  }
  if(workColE != nullptr)
  {
    free(workColE);
    workColE = nullptr;
  }
  if(devWorkColE != nullptr)
  {
    HIP_CHECK(hipFree(devWorkColE));
    devWorkColE = nullptr;
  }

  exit(errorCode);
}

/* Main program function */
int main(int argc, char *argv[])
{
  //get the properties
  hipDeviceProp_t devProp;
  hipGetDeviceProperties(&devProp, 0);
  cout << "GPU PROPERTIES\n";
  cout << "******************************************************************\n";
  cout << "Hip Device prop succeeded" << endl;
  cout << "System minor " << devProp.minor << endl;
  cout << "System major " << devProp.major << endl;
  cout << "Agent Prop Name " << devProp.name << endl;
  cout << "Total Global Memory " << devProp.totalGlobalMem << " bytes\n";
  cout << "Shared Memory Per Block " << devProp.sharedMemPerBlock << " bytes\n";
  cout << "Registers per block " << devProp.regsPerBlock << endl;
  cout << "Warp size " << devProp.warpSize << endl;
  cout << "Max Threads Per Block " << devProp.maxThreadsPerBlock << endl;
  cout << "Max clock frequency of the multiProcessors " << devProp.clockRate << " kHz\n";
  cout << "Max global memory clock frequency " << devProp.memoryClockRate << " kHz\n";
  cout << "Global memory bus width " << devProp.memoryBusWidth << " bits\n";
  cout << "Size of shared memory region " << devProp.totalConstMem << " bytes\n";
  cout << "Number of multi-processors (compute units) " << devProp.multiProcessorCount << endl;
  cout << "L2 cache size " << devProp.l2CacheSize << " bytes\n";
  cout << "Maximum resident threads per multi-processor " << devProp.maxThreadsPerMultiProcessor << endl;
  cout << "Maximum Shared Memory Per Multiprocessor " << devProp.maxSharedMemoryPerMultiProcessor << " bytes\n";
  cout << "******************************************************************\n\n";

  //allocate host(CPU) memory
  heatGrid = (double *) malloc(GRID_SIZE * sizeof(double));
  if(heatGrid == nullptr)
  {
    cerr << "Error, not enough memory...\nAborting...\n";
    heatCleanExit(1);
  }

  //initialise the heat grid
  heatInit(heatGrid);

  //allocate device(GPU) global memory
  HIP_CHECK(hipMalloc((void **)&devHeatGrid, GRID_SIZE * sizeof(double)));

  //transfer data from host(CPU) to device(GPU) memory
  HIP_CHECK(hipMemcpy(devHeatGrid, heatGrid, GRID_SIZE * sizeof(double),
                      hipMemcpyHostToDevice));

  //write out the initial grid to the corresponding file
  if(heatWrite(heatGrid, 0) == -1)
  {
    cerr << "Error, could not create the initial file...\nAborting...\n";
    heatCleanExit(2);
  }

  //calculate the kernel dimensions (x,y,z) threads/block and blocks/grid
  //serial 1 thread X / 1 block X
  dim3 hipThreads(THREADS_PER_BLOCK, 1, 1);
  dim3 hipBlocks(BLOCKS_PER_GRID, 1 ,1);

  //define the timer structs to be used
  struct timespec start;
  struct timespec end;
  double totalTime = 0.0;

  if(TOTAL_THREADS == 1)//serial execution
  {
    cout << "SERIAL EXECUTION WITH 1 HIP THREAD\n";

    //start the timer
    clock_gettime(CLOCK_MONOTONIC, &start);

    //launch the kernel(kernel overhead??)
    hipLaunchKernelGGL(heatUpdateSerial, hipBlocks, hipThreads,
                       GRID_SIZE * 2 * sizeof(double), 0,
                       devHeatGrid);

    //wait for device(GPU) to finish it's work
    HIP_CHECK(hipDeviceSynchronize());

    //stop the timer and print the result
    clock_gettime(CLOCK_MONOTONIC, &end);
    totalTime = ((end.tv_sec - start.tv_sec) * 1000.0) +
                ((end.tv_nsec - start.tv_nsec) / 1000000.0); 
    cout << "\nElapsed time was " << totalTime << " ms\n";
  }
  else//parallel execution
  {
    //initial check on number of threads
    if(TOTAL_THREADS % 2 != 0)
    {
      cout << "Can't parition grid fairly with odd number of threads = "
           << TOTAL_THREADS
           << "\nAborting...\n";
      heatCleanExit(4);
    }
    cout << "PARALLEL EXECUTION WITH " << TOTAL_THREADS << " HIP THREADS\n";

    //allocate arrays for neighbour discovery 
    //global border rows
    globalRowS = (int *) malloc(TOTAL_THREADS * sizeof(int));
    if(globalRowS == nullptr)
    {
      cerr << "Error, not enough memory...\nAborting...\n";
      heatCleanExit(5);
    }
    globalRowE = (int *) malloc(TOTAL_THREADS * sizeof(int));
    if(globalRowE == nullptr)
    {
      cerr << "Error, not enough memory...\nAborting...\n";
      heatCleanExit(5);
    }
    //global border collumns
    globalColS = (int *) malloc(TOTAL_THREADS * sizeof(int));
    if(globalColS == nullptr)
    {
      cerr << "Error, not enough memory...\nAborting...\n";
      heatCleanExit(5);
    }
    globalColE = (int *) malloc(TOTAL_THREADS * sizeof(int));
    if(globalColE == nullptr)
    {
      cerr << "Error, not enough memory...\nAborting...\n";
      heatCleanExit(5);
    }
    //working border rows
    workRowS = (int *) malloc(TOTAL_THREADS * sizeof(int));
    if(workRowS == nullptr)
    {
      cerr << "Error, not enough memory...\nAborting...\n";
      heatCleanExit(5);
    }
    workRowE = (int *) malloc(TOTAL_THREADS * sizeof(int));
    if(workRowE == nullptr)
    {
      cerr << "Error, not enough memory...\nAborting...\n";
      heatCleanExit(5);
    }
    //working border collumns
    workColS = (int *) malloc(TOTAL_THREADS * sizeof(int));
    if(workColS == nullptr)
    {
      cerr << "Error, not enough memory...\nAborting...\n";
      heatCleanExit(5);
    }
    workColE = (int *) malloc(TOTAL_THREADS * sizeof(int));
    if(workColE == nullptr)
    {
      cerr << "Error, not enough memory...\nAborting...\n";
      heatCleanExit(5);
    }
    
    int blockRows = 0;//total rows for each block of threads data block
    int blockCols = 0;//total cols for each block of threads data block
    int threadsPerRow = 0, threadsPerCol = 0;//vertical and horizontal distrib

    //classify problem cases based on total threads
    int cut = (int) sqrt(TOTAL_THREADS);
    double cutF = sqrt(TOTAL_THREADS);
    int probCase;
    if(TOTAL_THREADS == 2)//case 1 : handling 2 threads
    {
      blockRows = ROWS;
      blockCols = COLS / TOTAL_THREADS;
      threadsPerRow = ROWS / blockRows;
      threadsPerCol = COLS / blockCols;
      if((threadsPerRow * threadsPerCol) != TOTAL_THREADS)
      {
        cout << "Grid partitioning results to remains...\nAborting...\n";
        heatCleanExit(6);
      }    
      printf("Grid can be partioned without remains...\n"
             "Rows per block : %d, Columns per block : %d\n"
             "Vertical threads : %d, Horizontal threads : %d\n\n",
             blockRows, blockCols, threadsPerRow, threadsPerCol);
      probCase = 1;
    }
    else if(cutF > (double) cut)//case 2.1 : handling 6, 8, 10, ... threads
    {
      if(GRID_SIZE % TOTAL_THREADS != 0)//can't cut it without remains
      {
        cout << "Grid partitioning results to remains...\nAborting...\n";
        heatCleanExit(7);
      }
      const int localProbSize = GRID_SIZE / TOTAL_THREADS;
      int spread = GRID_SIZE;
      //find the best possible partition
      for(int i = ROWS; i > 0; --i)//priority to rows
      {
        for(int j = COLS; j > 0; --j)
        {
          if((i * j) == localProbSize)
          {
            if(ROWS % i != 0 || COLS % j != 0)
              continue;
            if(abs(i -j) < spread)
            {
              spread = abs(i - j);
              blockRows = i;
              blockCols = j;
            }
          }
        }
      }
      threadsPerRow = ROWS / blockRows;
      threadsPerCol = COLS / blockCols;
      if((threadsPerRow * threadsPerCol) != TOTAL_THREADS)
      {
        cout << "Grid partitioning results to remains...\nAborting...\n";
        heatCleanExit(7);
      }
      printf("Grid can be partioned without remains...\n"
             "Rows per block : %d, Columns per block : %d\n"
             "Vertical threads : %d, Horizontal threads : %d\n\n",
             blockRows, blockCols, threadsPerRow, threadsPerCol);
      probCase = 2;
    }
    else//case 2.2 : handling 4, 9, 16, ... threads
    {
      threadsPerRow = cut;
      threadsPerCol = cut;
      if((ROWS % cut != 0) || (COLS % cut != 0))//can't cut even blocks
      {
        cout << "Grid partitioning results to remains...\nAborting...\n";
        heatCleanExit(8);
      }
      blockRows = ROWS / cut;
      blockCols = COLS / cut;
      printf("Grid can be partioned without remains...\n"
             "Rows per block : %d, Columns per block : %d\n"
             "Vertical threads : %d, Horizontal threads : %d\n\n",
             blockRows, blockCols, threadsPerRow, threadsPerCol);
      probCase = 2;
    }

    //working and global discovery phase
    //allocate device(GPU) global memory
    HIP_CHECK(hipMalloc((void **)&devGlobalRowS, TOTAL_THREADS * sizeof(int)));
    HIP_CHECK(hipMalloc((void **)&devGlobalRowE, TOTAL_THREADS * sizeof(int)));
    HIP_CHECK(hipMalloc((void **)&devGlobalColS, TOTAL_THREADS * sizeof(int)));
    HIP_CHECK(hipMalloc((void **)&devGlobalColE, TOTAL_THREADS * sizeof(int)));
    HIP_CHECK(hipMalloc((void **)&devWorkRowS, TOTAL_THREADS * sizeof(int)));
    HIP_CHECK(hipMalloc((void **)&devWorkRowE, TOTAL_THREADS * sizeof(int)));
    HIP_CHECK(hipMalloc((void **)&devWorkColS, TOTAL_THREADS * sizeof(int)));
    HIP_CHECK(hipMalloc((void **)&devWorkColE, TOTAL_THREADS * sizeof(int)));
    //transfer data from host(CPU) to device(GPU) memory
    HIP_CHECK(hipMemcpy(devGlobalRowS, globalRowS, TOTAL_THREADS * sizeof(int),
                        hipMemcpyHostToDevice));
    HIP_CHECK(hipMemcpy(devGlobalRowE, globalRowE, TOTAL_THREADS * sizeof(int),
                        hipMemcpyHostToDevice));
    HIP_CHECK(hipMemcpy(devGlobalColS, globalColS, TOTAL_THREADS * sizeof(int),
                        hipMemcpyHostToDevice));
    HIP_CHECK(hipMemcpy(devGlobalColE, globalColE, TOTAL_THREADS * sizeof(int),
                        hipMemcpyHostToDevice));
    HIP_CHECK(hipMemcpy(devWorkRowS, workRowS, TOTAL_THREADS * sizeof(int),
                        hipMemcpyHostToDevice));
    HIP_CHECK(hipMemcpy(devWorkRowE, workRowE, TOTAL_THREADS * sizeof(int),
                        hipMemcpyHostToDevice));
    HIP_CHECK(hipMemcpy(devWorkColS, workColS, TOTAL_THREADS * sizeof(int),
                        hipMemcpyHostToDevice));
    HIP_CHECK(hipMemcpy(devWorkColE, workColE, TOTAL_THREADS * sizeof(int),
                        hipMemcpyHostToDevice));

    //mas endiaferei to global pou einai to meros tou pinaka
    //pou tha vlepei to kathe thread, isos mono to global
    hipLaunchKernelGGL(heatDiscover, hipBlocks, hipThreads, 0, 0,
                       devGlobalRowS, devGlobalRowE,
                       devGlobalColS, devGlobalColE,
                       devWorkRowS, devWorkRowE,
                       devWorkColS, devWorkColE,
                       blockRows, blockCols,
                       threadsPerRow, threadsPerCol, probCase);

    //wait for device(GPU) to finish it's work
    HIP_CHECK(hipDeviceSynchronize());

    //start the timer
    clock_gettime(CLOCK_MONOTONIC, &start);

    //launch the kernel
    hipLaunchKernelGGL(heatUpdateParallel, hipBlocks, hipThreads,
                       GRID_SIZE * 2 * sizeof(double), 0,
                       devHeatGrid,
                       devGlobalRowS, devGlobalRowE,
                       devGlobalColS, devGlobalColE,
                       devWorkRowS, devWorkRowE,
                       devWorkColS, devWorkColE);

    //wait for device(GPU) to finish it's work
    HIP_CHECK(hipDeviceSynchronize());

    //stop the timer and print the result
    clock_gettime(CLOCK_MONOTONIC, &end);
    totalTime = ((end.tv_sec - start.tv_sec) * 1000.0) +
                ((end.tv_nsec - start.tv_nsec) / 1000000.0); 
    cout << "\nElapsed time was " << totalTime << " ms\n";
  }

  HIP_CHECK(hipMemcpy(heatGrid, devHeatGrid,
                      GRID_SIZE * sizeof(double), hipMemcpyDeviceToHost));
  
  //write out the final grid to the corresponding file
  if(heatWrite(heatGrid, 1) == -1)
  {
    cerr << "Error, could not create the initial file...\nAborting...\n";
    heatCleanExit(3);
  }

  //clear memory and exit
  heatCleanExit(0);
}