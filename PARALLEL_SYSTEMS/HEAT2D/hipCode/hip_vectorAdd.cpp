#include <assert.h>
#include <cstdio>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
#include "hip/hip_runtime.h"

using std::cout;
using std::endl;
using std::cerr;
using std::abort;

#define WIDTH     1024
#define HEIGHT    1024
#define NUM       (WIDTH*HEIGHT)
#define THREADS_PER_BLOCK_X  16
#define THREADS_PER_BLOCK_Y  1
#define THREADS_PER_BLOCK_Z  1
#define BLOCKS_PER_GRID_X 1
#define BLOCKS_PER_GRID_Y 1
#define BLOCKS_PER_GRID_Z 1

//error checks on hip functions
#define HIP_CHECK(command) \
{ \
  hipError_t status = command; \
  if(status != hipSuccess) \
  { \
    cerr << "Error : HIP reports " << hipGetErrorString(status) << endl; \
    abort(); \
  } \
}

//device functions
__global__ void vectoradd_float(float * __restrict__ a,
                                const float * __restrict__ b,
                                const float * __restrict__ c,
                                int * __restrict__ out1,
                                int * __restrict__ out2,
                                int * __restrict__ out3,
                                int * __restrict__ out4,
                                int * __restrict__ out5,
                                int * __restrict__ out6,
                                int * __restrict__ out7,
                                const int totalThreads) 

{
  
  int x = hipBlockDim_x * hipBlockIdx_x + hipThreadIdx_x;
  int y = hipBlockDim_y * hipBlockIdx_y + hipThreadIdx_y;
  int globTiD = y * width + x;
  
  
  if(globTiD < totalThreads)
  {
    a[globTiD] = b[globTiD] + c[globTiD];
    out1[globTiD] = globTiD;
    out2[globTiD] = hipBlockDim_x;
    out3[globTiD] = hipBlockIdx_x;
    out4[globTiD] = hipThreadIdx_x;
    out5[globTiD] = hipBlockDim_y;
    out6[globTiD] = hipBlockIdx_y;
    out7[globTiD] = hipThreadIdx_y;
  }
}

#if 0
__kernel__ void vectoradd_float(float* a, const float* b, const float* c, int width, int height) {

  
  int x = blockDimX * blockIdx.x + threadIdx.x;
  int y = blockDimY * blockIdy.y + threadIdx.y;

  int i = y * width + x;
  if ( i < (width * height)) {
    a[i] = b[i] + c[i];
  }
}
#endif

//host functions

int main(int argc, char **argv)
{
  float * hostA;
  float * hostB;
  float * hostC;

  float * deviceA;
  float * deviceB;
  float * deviceC;

  hipDeviceProp_t devProp;
  hipGetDeviceProperties(&devProp, 0);
  cout << "System minor " << devProp.minor << endl;
  cout << "System major " << devProp.major << endl;
  cout << "Agent Prop Name " << devProp.name << endl;
  cout << "Hip Device prop succeeded" << endl ;

  hostA = (float *)malloc(NUM * sizeof(float));
  hostB = (float *)malloc(NUM * sizeof(float));
  hostC = (float *)malloc(NUM * sizeof(float));
  
  int * hostOut1 = (int *)malloc(NUM * sizeof(int));
  int * hostOut2 = (int *)malloc(NUM * sizeof(int));
  int * hostOut3 = (int *)malloc(NUM * sizeof(int));
  int * hostOut4 = (int *)malloc(NUM * sizeof(int));
  int * hostOut5 = (int *)malloc(NUM * sizeof(int));
  int * hostOut6 = (int *)malloc(NUM * sizeof(int));
  int * hostOut7 = (int *)malloc(NUM * sizeof(int));

  int * deviceOut1 = nullptr;
  int * deviceOut2 = nullptr;
  int * deviceOut3 = nullptr;
  int * deviceOut4 = nullptr;
  int * deviceOut5 = nullptr;
  int * deviceOut6 = nullptr;
  int * deviceOut7 = nullptr;
  
  // initialize the input data
  for(int i = 0; i < NUM; i++)
  {
    hostB[i] = (float) i;
    hostC[i] = (float) i * 100.0f;
    hostOut1[i] = 0;
    hostOut2[i] = 0;
    hostOut3[i] = 0;
    hostOut4[i] = 0;
    hostOut5[i] = 0;
    hostOut6[i] = 0;
    hostOut7[i] = 0;
  }
  //allocate memory in device
  HIP_CHECK(hipMalloc((void **)&deviceA, NUM * sizeof(float)));
  HIP_CHECK(hipMalloc((void **)&deviceB, NUM * sizeof(float)));
  HIP_CHECK(hipMalloc((void **)&deviceC, NUM * sizeof(float)));
  HIP_CHECK(hipMalloc((void **)&deviceOut1, NUM * sizeof(int)));
  HIP_CHECK(hipMalloc((void **)&deviceOut2, NUM * sizeof(int)));
  HIP_CHECK(hipMalloc((void **)&deviceOut3, NUM * sizeof(int)));
  HIP_CHECK(hipMalloc((void **)&deviceOut4, NUM * sizeof(int)));
  HIP_CHECK(hipMalloc((void **)&deviceOut5, NUM * sizeof(int)));
  HIP_CHECK(hipMalloc((void **)&deviceOut6, NUM * sizeof(int)));
  HIP_CHECK(hipMalloc((void **)&deviceOut7, NUM * sizeof(int)));
  //transfer data to device from host
  HIP_CHECK(hipMemcpy(deviceB, hostB, NUM * sizeof(float), hipMemcpyHostToDevice));
  HIP_CHECK(hipMemcpy(deviceC, hostC, NUM * sizeof(float), hipMemcpyHostToDevice));
  HIP_CHECK(hipMemcpy(deviceOut1, hostOut1, NUM * sizeof(int), hipMemcpyHostToDevice));
  HIP_CHECK(hipMemcpy(deviceOut2, hostOut2, NUM * sizeof(int), hipMemcpyHostToDevice));
  HIP_CHECK(hipMemcpy(deviceOut3, hostOut3, NUM * sizeof(int), hipMemcpyHostToDevice));
  HIP_CHECK(hipMemcpy(deviceOut4, hostOut4, NUM * sizeof(int), hipMemcpyHostToDevice));
  HIP_CHECK(hipMemcpy(deviceOut5, hostOut5, NUM * sizeof(int), hipMemcpyHostToDevice));
  HIP_CHECK(hipMemcpy(deviceOut6, hostOut6, NUM * sizeof(int), hipMemcpyHostToDevice));
  HIP_CHECK(hipMemcpy(deviceOut7, hostOut7, NUM * sizeof(int), hipMemcpyHostToDevice));

  //calculate the kernel dimensions
  dim3 hipThreads(THREADS_PER_BLOCK_X, THREADS_PER_BLOCK_Y, THREADS_PER_BLOCK_Z);
  dim3 hipBlocks(BLOCKS_PER_GRID_X, BLOCKS_PER_GRID_Y, BLOCKS_PER_GRID_Z);
  const int totalThreads = THREADS_PER_BLOCK_X * THREADS_PER_BLOCK_Y * THREADS_PER_BLOCK_Z *
                           BLOCKS_PER_GRID_X * BLOCKS_PER_GRID_Y * BLOCKS_PER_GRID_Z;

  //launch the kernel
  hipLaunchKernelGGL(vectoradd_float, 
                     hipBlocks, hipThreads,
                     0, 0,
                     deviceA, deviceB, deviceC,
                     deviceOut1, deviceOut2, deviceOut3, deviceOut4,
                     deviceOut5, deviceOut6, deviceOut7, totalThreads);
  //transfer data back to host from device
  HIP_CHECK(hipMemcpy(hostA, deviceA, NUM * sizeof(float), hipMemcpyDeviceToHost));
  HIP_CHECK(hipMemcpy(hostOut1, deviceOut1, NUM * sizeof(int), hipMemcpyDeviceToHost));
  HIP_CHECK(hipMemcpy(hostOut2, deviceOut2, NUM * sizeof(int), hipMemcpyDeviceToHost));
  HIP_CHECK(hipMemcpy(hostOut3, deviceOut3, NUM * sizeof(int), hipMemcpyDeviceToHost));
  HIP_CHECK(hipMemcpy(hostOut4, deviceOut4, NUM * sizeof(int), hipMemcpyDeviceToHost));
  HIP_CHECK(hipMemcpy(hostOut5, deviceOut5, NUM * sizeof(int), hipMemcpyDeviceToHost));
  HIP_CHECK(hipMemcpy(hostOut6, deviceOut6, NUM * sizeof(int), hipMemcpyDeviceToHost));
  HIP_CHECK(hipMemcpy(hostOut7, deviceOut7, NUM * sizeof(int), hipMemcpyDeviceToHost));

  // verify the results
  /*
  int errors = 0;
  for(int i = 0; i < NUM; ++i)
    if(hostA[i] != (hostB[i] + hostC[i]))
      ++errors;
  if(errors != 0)
   cout << "FAILED: " << errors << " errors!\n";
  else
    cout << "PASSED!\n";
  */

  cout << "printing results" << endl;
  for(int i = 0; i < totalThreads; ++i)
  {
    printf("Hello from thread globalId : %d\n\
            BlockThreadIdX = hipBlockDim_x * hipBlockIdx_x + hipThreadIdx_x = %d * %d + %d = %d\n\
            BlockThreadIdY = hipBlockDim_y * hipBlockIdx_y + hipThreadIdx_y =  %d * %d + %d = %d\n", 
            hostOut1[i],
            hostOut2[i], hostOut3[i], hostOut4[i],
            (hostOut2[i] * hostOut3[i] + hostOut4[i]),
            hostOut5[i], hostOut6[i], hostOut7[i],
            (hostOut5[i] * hostOut6[i] + hostOut7[i]));
  }

  //free the device memory
  HIP_CHECK(hipFree(deviceA));
  HIP_CHECK(hipFree(deviceB));
  HIP_CHECK(hipFree(deviceC));
  HIP_CHECK(hipFree(deviceOut1));
  HIP_CHECK(hipFree(deviceOut2));
  HIP_CHECK(hipFree(deviceOut3));
  HIP_CHECK(hipFree(deviceOut4));
  HIP_CHECK(hipFree(deviceOut5));
  HIP_CHECK(hipFree(deviceOut6));
  HIP_CHECK(hipFree(deviceOut7));

  deviceA = nullptr;
  deviceB = nullptr;
  deviceC = nullptr;
  deviceOut1 = nullptr;
  deviceOut2 = nullptr;
  deviceOut3 = nullptr;
  deviceOut4 = nullptr;
  deviceOut5 = nullptr;
  deviceOut6 = nullptr;
  deviceOut7 = nullptr;

  free(hostA);
  free(hostB);
  free(hostC);
  free(hostOut1);
  free(hostOut2);
  free(hostOut3);
  free(hostOut4);
  free(hostOut5);
  free(hostOut6);
  free(hostOut7);
  hostA = nullptr;
  hostB = nullptr;
  hostC = nullptr;
  hostOut1 = nullptr;
  hostOut2 = nullptr;
  hostOut3 = nullptr;
  hostOut4 = nullptr;
  hostOut5 = nullptr;
  hostOut6 = nullptr;
  hostOut7 = nullptr;

  //hipResetDefaultAccelerator();
  return 0;
}