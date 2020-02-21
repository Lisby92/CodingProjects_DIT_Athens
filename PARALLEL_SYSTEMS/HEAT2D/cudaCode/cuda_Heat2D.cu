#include <assert.h>
//#include <sys/time.h>
#include <time.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <algorithm>

using std::cout;
using std::endl;
using std::cerr;

#define DECLINE_HORIZONTAL 0.1
#define DECLINE_VERTICAL   0.1
#define STEPS              1000               /* number of time steps */

/* error check on hip macro */
#define CUDA_CHECK(command) \
{ \
  cudaError_t status = command; \
  if(status != cudaSuccess) \
  { \
    cerr << "Error : Cuda reports " << cudaGetErrorString(status) << endl; \
    heatCleanExit(-6); \
  } \
}

/* Device(GPU) FUNCTIONS TO BE LAUNCHED AS KERNELS FROM HOST(CPU) */
/* This function is used to discover the local working area of each thread */
__global__ void heatDiscover(int * __restrict__ workRowS,
                             int * __restrict__ workRowE,
                             int * __restrict__ workColS,
                             int * __restrict__ workColE,
                             const int blockRows, const int blockCols,
                             const int threadsPerRow, const int threadsPerCol,
                             const int probCase,
                             const int gridRows, const int gridCols)
{
  const int thread_Id = blockIdx.x * blockDim.x + threadIdx.x;
  if(probCase == 1)//2 threads
  {
    if(thread_Id == 0)//west thread
    {
      workRowS[thread_Id] = 1;
      workRowE[thread_Id] = blockRows - 2;
      workColS[thread_Id] = 1;
      workColE[thread_Id] = blockCols - 1;
    }
    else//east thread
    {
      workRowS[thread_Id] = 1;
      workRowE[thread_Id] = blockRows - 2;
      workColS[thread_Id] = blockCols;
      workColE[thread_Id] = gridCols - 2;
    } 
  }
  else if(probCase == 2)//6, 8, 10, ... OR 4, 16, 64 ... threads
  {
    if(thread_Id == 0)//NW corner
    {
      workRowS[thread_Id] = 1;
      workRowE[thread_Id] = blockRows - 1;
      workColS[thread_Id] = 1;
      workColE[thread_Id] = blockCols - 1;
    }
    else if(thread_Id == (threadsPerCol - 1))//NE corner
    {
      workRowS[thread_Id] = 1;
      workRowE[thread_Id] = blockRows - 1;
      workColS[thread_Id] = gridCols - blockCols;
      workColE[thread_Id] = gridCols - 2;
    }
    else if(thread_Id == ((threadsPerRow * threadsPerCol) - threadsPerCol))//SW corner
    {
      workRowS[thread_Id] = gridRows - blockRows;
      workRowE[thread_Id] = gridRows - 2;
      workColS[thread_Id] = 1;
      workColE[thread_Id] = blockCols - 1;
    }
    else if(thread_Id == ((threadsPerCol * threadsPerRow) - 1))//SE corner
    {
      workRowS[thread_Id] = gridRows - blockRows;
      workRowE[thread_Id] = gridRows - 2;
      workColS[thread_Id] = gridCols - blockCols;
      workColE[thread_Id] = gridCols - 2;
    }
    else if(thread_Id < threadsPerCol)//NN side
    {
      workRowS[thread_Id] = 1;
      workRowE[thread_Id] = blockRows - 1;
      workColS[thread_Id] = thread_Id * blockCols;
      workColE[thread_Id] = workColS[thread_Id] + blockCols - 1;
    }
    else if((thread_Id > ((threadsPerCol * threadsPerRow) - threadsPerCol)) && 
            (thread_Id < ((threadsPerCol * threadsPerRow) - 1)))//SS side
    {
      workRowS[thread_Id] = gridRows - blockRows;
      workRowE[thread_Id] = gridRows - 2;
      workColS[thread_Id] = (thread_Id % threadsPerCol) * blockCols;
      workColE[thread_Id] =  workColS[thread_Id] + blockCols - 1;
    }
    else if((thread_Id % threadsPerCol) == 0)//WW side
    {
      workRowS[thread_Id] = (thread_Id / threadsPerCol) * blockRows;
      workRowE[thread_Id] = workRowS[thread_Id] + blockRows - 1;
      workColS[thread_Id] = 1;
      workColE[thread_Id] = blockCols - 1;
    }
    else if((thread_Id + 1) % threadsPerCol == 0)//EE side
    {
      workRowS[thread_Id] = ((thread_Id + 1 - threadsPerCol) / threadsPerCol) *
                              blockRows;
      workRowE[thread_Id] = workRowS[thread_Id] + blockRows - 1;
      workColS[thread_Id] = gridCols - blockCols;
      workColE[thread_Id] = gridCols - 2;
    }
    else//general case middle location
    {
      int rowMarginS = threadsPerCol;
      int rowMarginE = rowMarginS + threadsPerCol - 1;
      int rowOffset = 1;
      while(1)
      {
        if((thread_Id > rowMarginS) && (thread_Id < rowMarginE))
        {
          workRowS[thread_Id] = rowOffset * blockRows;
          break;
        }
        else
        {
          rowMarginS += threadsPerCol;
          rowMarginE += threadsPerCol;
          ++rowOffset;
        }
      }
      workRowE[thread_Id] = workRowS[thread_Id] + blockRows - 1;
      workColS[thread_Id] = (thread_Id % threadsPerCol) * blockCols;
      workColE[thread_Id] = workColS[thread_Id] + blockCols - 1;
    }
  }
}

/* This function updates the grid and is invoked on serial executions */
__global__ void heatUpdateSerial(const double * __restrict__ devOldHeatGrid,
                                       double * __restrict__ devNewHeatGrid,
                                 const int gridRows, const int gridCols)
{
  for(int i = 1; i < gridRows - 1; ++i)
  {
    for(int j = 1; j < gridCols - 1; ++j)
    {
      devNewHeatGrid[i*gridCols+j] = devOldHeatGrid[i*gridCols+j] +
                                    DECLINE_HORIZONTAL *
                                    (devOldHeatGrid[(i+1)*gridCols+j] +
                                    devOldHeatGrid[(i-1)*gridCols+j] -
                                    (2 * devOldHeatGrid[i*gridCols+j])) +
                                    DECLINE_VERTICAL *
                                    (devOldHeatGrid[i*gridCols+j+1] +
                                    devOldHeatGrid[i*gridCols+j-1] -
                                    (2 * devOldHeatGrid[i*gridCols+j]));
    }
  }    
}

/* This function updates the grid and is invoked on parallel executions */
__global__ void heatUpdateParallel(const double * __restrict__ devOldHeatGrid,
                                   double       * __restrict__ devNewHeatGrid,
                                   const int    * __restrict__ workRowS,
                                   const int    * __restrict__ workRowE,
                                   const int    * __restrict__ workColS,
                                   const int    * __restrict__ workColE,
                                   const int                   gridCols)
{
  const int thread_Id = blockIdx.x * blockDim.x + threadIdx.x;
  //get the borders in registers for 1 cycle memory access
  const int wRowS = workRowS[thread_Id];
  const int wRowE = workRowE[thread_Id];
  const int wColS = workColS[thread_Id];
  const int wColE = workColE[thread_Id];

  for(int i = wRowS; i <= wRowE; ++i)
  {
    for(int j = wColS; j <= wColE; ++j)
    {
      devNewHeatGrid[i*gridCols+j] = devOldHeatGrid[i*gridCols+j] +
                                 DECLINE_HORIZONTAL *
                                 (devOldHeatGrid[(i+1)*gridCols+j] +
                                 devOldHeatGrid[(i-1)*gridCols+j] -
                                 (2*devOldHeatGrid[i*gridCols+j])) +
                                 DECLINE_VERTICAL *
                                 (devOldHeatGrid[i*gridCols+j+1] +
                                 devOldHeatGrid[i*gridCols+j-1] -
                                 (2*devOldHeatGrid[i*gridCols+j]));
    }
  }
}                                      

//declare dynamic variables
double * heatGrid = nullptr;
double * devNewHeatGrid = nullptr;
double * devOldHeatGrid = nullptr;
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
static inline void heatInit(double * heatGrid,
                            const int gridRows, const int gridCols)
{
  for(int i = 0; i < gridRows; ++i)//avoid halo area
    for(int j = 0; j < gridCols; ++j)
      heatGrid[i*gridCols+j] = (double) (i * (gridRows - i - 1) * j * (gridCols - j - 1));
}

/* This function writes out the input grid to a .dat file in current path */
static inline int heatWrite(const double * heatGrid, const int flag,
                            const int gridRows, const int gridCols,
                            const int threadsPerBlock, const int blocksPerGrid)
{
  char filePath[70] = "";
  if(flag == 0)
  {
    sprintf(filePath, "%d_%d_cuda_%d_%d_Initial.dat", gridRows, gridCols,
            threadsPerBlock, blocksPerGrid);
  }
  else
  {
    sprintf(filePath, "%d_%d_cuda_%d_%d_Final.dat", gridRows, gridCols,
            threadsPerBlock, blocksPerGrid);
  }

  FILE * fp = fopen(filePath, "w");
  if(fp == nullptr)
    return -1;
  for(int i = 0; i < gridRows; ++i)
  {
    for(int j = 0; j < gridCols; ++j)
    {
      fprintf(fp, "%-.1lf", fabs(heatGrid[i*gridCols+j]));//some 0.0s appear as -0.0s
      if(j != (gridCols - 1))
        fprintf(fp, " ");
    }
    fprintf(fp, "\n");
  }
  fclose(fp);
  return 0;//all ok
}

/* This function swaps between the 2 grids to avoid assignmenets */
static inline void heatSwap(double ** a, double ** b)
{
  double *temp = *a;
  *a = *b;
  *b = temp;
}

/* This function cleans up memory to prevent leaks on any exit error */
static inline void heatCleanExit(const int errorCode)
{
  if(heatGrid != nullptr)
  {
    free(heatGrid);
    heatGrid = nullptr;
  }
  if(devOldHeatGrid != nullptr)
  {
    CUDA_CHECK(cudaFree(devOldHeatGrid));
    devOldHeatGrid = nullptr;
  }
  if(devNewHeatGrid != nullptr)
  {
    CUDA_CHECK(cudaFree(devNewHeatGrid));
    devNewHeatGrid = nullptr;
  }
  if(workRowS != nullptr)
  {
    free(workRowS);
    workRowS = nullptr;
  }
  if(devWorkRowS != nullptr)
  {
    CUDA_CHECK(cudaFree(devWorkRowS));
    devWorkRowS = nullptr;
  }
  if(workRowE != nullptr)
  {
    free(workRowE);
    workRowE = nullptr;
  }
  if(devWorkRowE != nullptr)
  {
    CUDA_CHECK(cudaFree(devWorkRowE));
    devWorkRowE = nullptr;
  }
  if(workColS != nullptr)
  {
    free(workColS);
    workColS = nullptr;
  }
  if(devWorkColS != nullptr)
  {
    CUDA_CHECK(cudaFree(devWorkColS));
    devWorkColS = nullptr;
  }
  if(workColE != nullptr)
  {
    free(workColE);
    workColE = nullptr;
  }
  if(devWorkColE != nullptr)
  {
    CUDA_CHECK(cudaFree(devWorkColE));
    devWorkColE = nullptr;
  }
  exit(errorCode);
}

/* Main program function */
int main(int argc, char *argv[])
{
  //get the properties
  cudaDeviceProp deviceProp;
  CUDA_CHECK(cudaGetDeviceProperties(&deviceProp, 0));
  cout << "GPU PROPERTIES\n";
  cout << "******************************************************************\n";
  cout << "Cuda Device prop succeeded" << endl;
  cout << "System minor " << deviceProp.minor << endl;
  cout << "System major " << deviceProp.major << endl;
  cout << "Agent Prop Name " << deviceProp.name << endl;
  cout << "Total Global Memory " << deviceProp.totalGlobalMem << " bytes\n";
  cout << "Shared Memory Per Block " << deviceProp.sharedMemPerBlock << " bytes\n";
  cout << "Registers per block " << deviceProp.regsPerBlock << endl;
  cout << "Warp size " << deviceProp.warpSize << endl;
  cout << "Max Threads Per Block " << deviceProp.maxThreadsPerBlock << endl;
  cout << "Max clock frequency of the multiProcessors " << deviceProp.clockRate << " kHz\n";
  cout << "Size of shared memory region " << deviceProp.totalConstMem << " bytes\n";
  cout << "Number of multi-processors (compute units) " << deviceProp.multiProcessorCount << endl;
  cout << "******************************************************************\n\n";
  //get properties to check on input data possible run scenarios
  const int devMaxThreadsPerBlock = deviceProp.maxThreadsPerBlock;
  const int devMaxConcurrentThreads = deviceProp.multiProcessorCount *
                                      deviceProp.warpSize;

  //get the command line input data and do initial checks
  if(argc != 5)
  {
    cerr << "Not enough input data, need 4\n";
    cerr << "Grid_Rows Grid_Collumns Threads_Per_Block Blocks_Per_Grid\n";
    cerr << "Aborting...\n";
    heatCleanExit(-1);
  }
  const int gridRows = atoi(argv[1]);
  const int gridCols = atoi(argv[2]);
  const int threadsPerBlock = atoi(argv[3]);
  const int blocksPerGrid = atoi(argv[4]);
  const int gridSize = gridRows * gridCols;
  const int totalThreads = threadsPerBlock * blocksPerGrid;
  if(gridRows < 0 || gridCols < 0 || threadsPerBlock < 1 || blocksPerGrid < 1)
  {
    cerr << "Invalid Input Data\n";
    cerr << "Grid Rows = " << gridRows << endl;
    cerr << "Grid Cols = " << gridCols << endl;
    cerr << "Threads Per Block = " << threadsPerBlock << endl;
    cerr << "Blocks Per Grid = " << blocksPerGrid << endl;
    cerr << "Aborting...\n";
    heatCleanExit(-1);
  }
  //do checks based on device(GPU) capabilities
  if(threadsPerBlock > devMaxThreadsPerBlock)
  {
    cerr << "Maximum threads per block exceeded for current device\n";
    cerr << "Aborting...\n";
    heatCleanExit(-2);
  }
  if(totalThreads > devMaxConcurrentThreads)
  {
    cerr << "Maximum concurrent threads exceeded for current device\n";
    cerr << "Aborting...\n";
    heatCleanExit(-2);
  }

  //allocate host(CPU) memory
  heatGrid = (double *) malloc(gridSize * sizeof(double));
  if(heatGrid == nullptr)
  {
    cerr << "Error, not enough memory...\nAborting...\n";
    heatCleanExit(1);
  }

  //initialise with 0.0s the heat grid
  for(int i = 0; i < gridRows; ++i)
    for(int j = 0; j < gridCols; ++j)
      heatGrid[i*gridCols+j] = 0.0;

  //allocate device(GPU) global memory
  CUDA_CHECK(cudaMalloc((void **)&devNewHeatGrid, gridSize * sizeof(double)));

  //transfer data from host(CPU) to device(GPU) memory
  CUDA_CHECK(cudaMemcpy(devNewHeatGrid, heatGrid, gridSize * sizeof(double),
                      cudaMemcpyHostToDevice));

  //initialise the heat grid with actual data
  heatInit(heatGrid, gridRows, gridCols);

  //allocate device(GPU) global memory
  CUDA_CHECK(cudaMalloc((void **)&devOldHeatGrid, gridSize * sizeof(double)));

  //transfer data from host(CPU) to device(GPU) memory
  CUDA_CHECK(cudaMemcpy(devOldHeatGrid, heatGrid, gridSize * sizeof(double),
                      cudaMemcpyHostToDevice));

  //write out the initial grid to the corresponding file
  // if(heatWrite(heatGrid, 0, gridRows, gridCols,
               // threadsPerBlock, blocksPerGrid) == -1)
  // {
    // cerr << "Error, could not create the initial file...\nAborting...\n";
    // heatCleanExit(2);
  // }

  //calculate the kernel dimensions (x,y,z) threads/block and blocks/grid
  dim3 cudaThreads(threadsPerBlock, 1, 1);
  dim3 cudaBlocks(blocksPerGrid, 1 ,1);

  //define the timer structs to be used
  // struct timespec start;
  // struct timespec end;
  // double totalTime = 0.0;

  if(totalThreads == 1)//serial execution
  {
    cout << "Serial execution with 1 cuda thread\n";

    //start the timer
    //clock_gettime(CLOCK_MONOTONIC, &start);
	clock_t begin = clock();

    //solve the problem
    for(int steps = 0; steps < STEPS; ++steps)
    {
      //launch the kernel
      heatUpdateSerial<<<cudaBlocks, cudaThreads>>>(devOldHeatGrid, devNewHeatGrid,
                                                    gridRows, gridCols);

      //wait for device(GPU) to finish it's work
      CUDA_CHECK(cudaDeviceSynchronize());

      //old = new
      heatSwap(&devNewHeatGrid, &devOldHeatGrid);
    }

    //stop the timer and print the result
    //clock_gettime(CLOCK_MONOTONIC, &end);
	clock_t end = clock();
	double totalTime = (double)(end - begin) / CLOCKS_PER_SEC;
    //totalTime = ((end.tv_sec - start.tv_sec) * 1000.0) +
    //            ((end.tv_nsec - start.tv_nsec) / 1000000.0); 
    cout << "\nElapsed time was " << totalTime << " ms\n";
  }
  else//parallel execution
  {
    //initial check on number of threads
    if(totalThreads % 2 != 0)
    {
      cout << "Can't parition grid fairly with odd number of threads = "
           << totalThreads
           << "\nAborting...\n";
      heatCleanExit(4);
    }
    cout << "Parallel execution with ";
    cout << "Threads Per Block : " << threadsPerBlock << endl;
    cout << "Blocks Per Grid : " << blocksPerGrid << endl;
    cout << "Total Cuda Threads : " << totalThreads << endl;

    //allocate arrays for neighbour discovery 
    //working border rows
    workRowS = (int *) malloc(totalThreads * sizeof(int));
    if(workRowS == nullptr)
    {
      cerr << "Error, not enough memory...\nAborting...\n";
      heatCleanExit(5);
    }
    workRowE = (int *) malloc(totalThreads * sizeof(int));
    if(workRowE == nullptr)
    {
      cerr << "Error, not enough memory...\nAborting...\n";
      heatCleanExit(5);
    }
    //working border collumns
    workColS = (int *) malloc(totalThreads * sizeof(int));
    if(workColS == nullptr)
    {
      cerr << "Error, not enough memory...\nAborting...\n";
      heatCleanExit(5);
    }
    workColE = (int *) malloc(totalThreads * sizeof(int));
    if(workColE == nullptr)
    {
      cerr << "Error, not enough memory...\nAborting...\n";
      heatCleanExit(5);
    }
    
    int blockRows = 0;//total rows for each block of threads data block
    int blockCols = 0;//total cols for each block of threads data block
    int threadsPerRow = 0, threadsPerCol = 0;//vertical and horizontal distrib

    //classify problem cases based on total threads
    int cut = (int) sqrt(totalThreads);
    double cutF = sqrt(totalThreads);
    int probCase;
    if(totalThreads == 2)//case 1 : handling 2 threads
    {
      blockRows = gridRows;
      blockCols = gridCols / totalThreads;
      threadsPerRow = gridRows / blockRows;
      threadsPerCol = gridCols / blockCols;
      if((threadsPerRow * threadsPerCol) != totalThreads)
      {
        cout << "Grid partitioning results to remains...\nAborting...\n";
        heatCleanExit(6);
      }
      probCase = 1;
    }
    else if(cutF > (double) cut)//case 2.1 : handling 6, 8, 10, ... threads
    {
      if(gridSize % totalThreads != 0)//can't cut it without remains
      {
        cout << "Grid partitioning results to remains...\nAborting...\n";
        heatCleanExit(7);
      }
      const int localProbSize = gridSize / totalThreads;
      int spread = gridSize;
      //find the best possible partition
      for(int i = gridRows; i > 0; --i)//priority to rows
      {
        for(int j = gridCols; j > 0; --j)
        {
          if((i * j) == localProbSize)
          {
            if(gridRows % i != 0 || gridCols % j != 0)
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
      threadsPerRow = gridRows / blockRows;
      threadsPerCol = gridCols / blockCols;
      if((threadsPerRow * threadsPerCol) != totalThreads)
      {
        cout << "Grid partitioning results to remains...\nAborting...\n";
        heatCleanExit(7);
      }
      probCase = 2;
    }
    else//case 2.2 : handling 4, 9, 16, ... threads
    {
      threadsPerRow = cut;
      threadsPerCol = cut;
      if((gridRows % cut != 0) || (gridCols % cut != 0))//can't cut even blocks
      {
        cout << "Grid partitioning results to remains...\nAborting...\n";
        heatCleanExit(8);
      }
      blockRows = gridRows / cut;
      blockCols = gridCols / cut;
      probCase = 2;
    }

    printf("Grid can be partioned without remains...\n"
             "Rows per block : %d, Columns per block : %d\n"
             "Vertical threads : %d, Horizontal threads : %d\n\n",
             blockRows, blockCols, threadsPerRow, threadsPerCol);

    //working and global discovery phase
    //allocate device(GPU) global memory
    CUDA_CHECK(cudaMalloc((void **)&devWorkRowS, totalThreads * sizeof(int)));
    CUDA_CHECK(cudaMalloc((void **)&devWorkRowE, totalThreads * sizeof(int)));
    CUDA_CHECK(cudaMalloc((void **)&devWorkColS, totalThreads * sizeof(int)));
    CUDA_CHECK(cudaMalloc((void **)&devWorkColE, totalThreads * sizeof(int)));
    //transfer data from host(CPU) to device(GPU) memory
    CUDA_CHECK(cudaMemcpy(devWorkRowS, workRowS, totalThreads * sizeof(int),
                          cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(devWorkRowE, workRowE, totalThreads * sizeof(int),
                          cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(devWorkColS, workColS, totalThreads * sizeof(int),
                          cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(devWorkColE, workColE, totalThreads * sizeof(int),
                          cudaMemcpyHostToDevice));

    heatDiscover<<<cudaBlocks, cudaThreads>>>
                (devWorkRowS, devWorkRowE, devWorkColS, devWorkColE,
                 blockRows, blockCols,
                 threadsPerRow, threadsPerCol, probCase,
                 gridRows, gridCols);

    //wait for device(GPU) to finish it's work
    CUDA_CHECK(cudaDeviceSynchronize());

    //start the timer
    // clock_gettime(CLOCK_MONOTONIC, &start);
	clock_t begin = clock();

    //launch the kernel
    for(int steps = 0; steps < STEPS; ++steps)
    {
      heatUpdateParallel<<<cudaBlocks, cudaThreads>>>
                        (devOldHeatGrid, devNewHeatGrid,
                         devWorkRowS, devWorkRowE, devWorkColS, devWorkColE,
                         gridCols);

      //wait for device(GPU) to finish it's work
      CUDA_CHECK(cudaDeviceSynchronize());

      //old = new
      heatSwap(&devNewHeatGrid, &devOldHeatGrid);
    }
    
    //stop the timer and print the result
	clock_t end = clock();
	double totalTime = (double)(end - begin) / CLOCKS_PER_SEC;
    // clock_gettime(CLOCK_MONOTONIC, &end);
    // totalTime = ((end.tv_sec - start.tv_sec) * 1000.0) +
                // ((end.tv_nsec - start.tv_nsec) / 1000000.0); 
    cout << "\nElapsed time was " << totalTime << " ms\n";
  }

  if(STEPS % 2 == 0)//get the correct version
  {
    CUDA_CHECK(cudaMemcpy(heatGrid, devOldHeatGrid, gridSize * sizeof(double),
                        cudaMemcpyDeviceToHost));
  }
  else
  {
    CUDA_CHECK(cudaMemcpy(heatGrid, devNewHeatGrid, gridSize * sizeof(double),
                        cudaMemcpyDeviceToHost));
  }
  
  //write out the final grid to the corresponding file
  // if(heatWrite(heatGrid, 1, gridRows, gridCols,
               // threadsPerBlock, blocksPerGrid) == -1)
  // {
    // cerr << "Error, could not create the initial file...\nAborting...\n";
    // heatCleanExit(3);
  // }

  //clear memory and exit
  heatCleanExit(0);
}