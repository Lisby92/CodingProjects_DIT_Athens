#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <string.h>
#include <time.h>

#define ROWS        80                 /* y dimension of problem grid */
#define COLS        64                 /* x dimension of problem grid */
#define STEPS       1000               /* number of time steps */
#define S_TAG       1                  /* message tag */
#define N_TAG       2                  /* message tag */
#define E_TAG       3                  /* message tag */
#define W_TAG       4                  /* message tag */

static int globalFlag = 0;//for allreduce
static int reductionStep = (int) sqrt(STEPS);//convergence check intervals

static const struct heatArgs
{ 
  double factorRow;
  double factorCol;
} heatArgs = {0.1, 0.1};

static inline void heatSwap(double ** a, double ** b)
{
  /* This function swaps between the 2 grids to avoid assignmenets */
  double *temp = *a;
  *a = *b;
  *b = temp;
}

static inline int heatUpdateLocal(const double * old, double * new,
                                   const int cols, const int convCheck,
                                   const int rowStart, const int rowEnd, 
                                   const int colStart, const int colEnd)
{
  /* This function updates the grid areas which dont depend on halo rows or
     collumns received through communicating with neighboors.
     Convergence is checked here because the temperature is higher around the 
     centre of the grid resulting to longer time needed for convergence to 
     occur at it */
  int localFlag = 1;
  int convFlag = 0;
  if(convCheck == 1)//check for convergence this step
  {
    for(int i = (rowStart + 1); i <= (rowEnd - 1); ++i)
    {
      for(int j = (colStart + 1); j <= (colEnd - 1); ++j)
      {
        new[i*(cols+2)+j] = old[i*(cols+2)+j] +
        heatArgs.factorRow *
        (old[(i+1)*(cols+2)+j] + old[(i-1)*(cols+2)+j] - (2*old[i*(cols+2)+j])) +
        heatArgs.factorCol *
        (old[i*(cols+2)+j+1] + old[i*(cols+2)+j-1] - (2*old[i*(cols+2)+j]));
        if(localFlag == 1)
        {
          if(new[i*(cols+2)+j] < old[i*(cols+2)+j])//found a tile with dif val
          {
            convFlag = 1;
            localFlag = 0;//stop checking
          }
        }
      }
    }
  }
  else//do not check for convergence this step
  {
    for(int i = (rowStart + 1); i <= (rowEnd - 1); ++i)
    {
      for(int j = (colStart + 1); j <= (colEnd - 1); ++j)
      {
        new[i*(cols+2)+j] = old[i*(cols+2)+j] +
        heatArgs.factorRow *
        (old[(i+1)*(cols+2)+j] + old[(i-1)*(cols+2)+j] - (2*old[i*(cols+2)+j])) +
        heatArgs.factorCol *
        (old[i*(cols+2)+j+1] + old[i*(cols+2)+j-1] - (2*old[i*(cols+2)+j]));
      }
    }
  }
  return convFlag;
}

static inline void heatUpdateBorder(const double * old, double * new,
                                    const int cols,
                                    const int rowStart, const int rowEnd, 
                                    const int colStart, const int colEnd)
{
  /* This function updates the grid areas that depend only on halo rows and 
     collumns received through communication with neighboors */
  for(int i = rowStart; i <= rowEnd; ++i)
  {
    for(int j = colStart; j <= colEnd; ++j)
    {
      if((i == rowStart || i == rowEnd) || (j == colStart || j == colEnd))//halo area
        new[i*(cols+2)+j] = old[i*(cols+2)+j] +
        heatArgs.factorRow *
        (old[(i+1)*(cols+2)+j] + old[(i-1)*(cols+2)+j] - (2*old[i*(cols+2)+j]))+
        heatArgs.factorCol *
        (old[i*(cols+2)+j+1] + old[i*(cols+2)+j-1] - (2*old[i*(cols+2)+j]));
      else//avoid local
        continue;
    }
  }
}

static inline int heatUpdateSerial(const double * old, double * new,
                                   const int cols, const int convCheck,
                                   const int rowStart, const int rowEnd, 
                                   const int colStart, const int colEnd)
{
  /* This function updates all grid areas and is invoked on serial executions */
  int localFlag = 1;
  int convFlag = 0;
  if(convCheck == 1)//check for convergence this step
  {
    for(int i = rowStart; i < rowEnd; ++i)
    {
      for(int j = colStart; j < colEnd; ++j)
      {
        new[i*cols+j] = old[i*cols+j] +
        heatArgs.factorRow *
        (old[(i+1)*cols+j] + old[(i-1)*cols+j] - (2 * old[i*cols+j])) +
        heatArgs.factorCol *
        (old[i*cols+j+1] + old[i*cols+j-1] - (2 * old[i*cols+j]));
        if(localFlag == 1)//check
        {
          if(new[i*(cols)+j] < old[i*(cols)+j])//found a tile with dif val
          {
            convFlag = 1;
            localFlag = 0;//stop checking
          }
        }
      }
    }
  }
  else//dont check for convergence
  {
    for(int i = rowStart; i < rowEnd; ++i)
    {
      for(int j = colStart; j < colEnd; ++j)
      {
        new[i*cols+j] = old[i*cols+j] +
        heatArgs.factorRow *
        (old[(i+1)*cols+j] + old[(i-1)*cols+j] - (2 * old[i*cols+j])) +
        heatArgs.factorCol *
        (old[i*cols+j+1] + old[i*cols+j-1] - (2 * old[i*cols+j]));
      }
    }
  }
  return convFlag;
}

static inline int heatWriteSerial(const int procRank, const int rows,
                                  const double * grid, const int cols,
                                  const int rowStart, const int rowEnd, 
                                  const int colStart, const int colEnd,
                                  const int flag)
{
  /* This function writes out the input grid to a .dat file in current path */
  char filePath[70] = "";
  if(flag == 0)
    sprintf(filePath, "%d_%d_conv_serial_Initial.dat", rows, cols);
  else
    sprintf(filePath, "%d_%d_conv_serial_Final.dat", rows, cols);

  FILE * fp = fopen(filePath, "w");
  if(fp == NULL)
    return -1;
  for(int i = rowStart; i < rowEnd; ++i)
  {
    for(int j = colStart; j < colEnd; ++j)
    {
      fprintf(fp, "%-14.1lf", grid[i*cols+j]);
      if(j != (colEnd -1))
        fprintf(fp, " ");
    }
    fprintf(fp, "\n");
  }
  fclose(fp);
  return 0;//all ok
}

static inline void heatInit(double * grid, const int flag, const int cols,
                            const int rowStart, const int rowEnd, 
                            const int colStart, const int colEnd,
                            const int globalRowS, const int globalRowE, 
                            const int globalColS, const int globalColE)
{
  /* This function intialises the temperature on the given grid with higher
     temperatures at the centre, progressively lower ones until the sides 
     and 0s at the perimetre */
  if(flag == 0)//serial
    for(int i = 0; i < ROWS; ++i)//avoid halo area
      for(int j = 0; j < COLS; ++j)
        grid[i*COLS+j] = (double) (i * (ROWS - i - 1) * j * (COLS - j - 1));
  else if(flag == 1)//coop
  {
    int locI = 0;
    int locJ = 0;
    for(int i = rowStart; i < rowEnd; ++i)//avoid halo area
    {
      if(i < globalRowS || i > globalRowE)//in halo area
        continue;
      else
      {
        ++locI;
        locJ = 0;//reinit
      }
      for(int j = colStart; j < colEnd; ++j)
      {
        if(j < globalColS || j > globalColE)//in halo area
          continue;
        else
        {
          ++locJ;
          grid[locI*cols+locJ] = (double)
          ((i-1) * (ROWS - (i - 1) - 1) * (j - 1) * (COLS - (j - 1) - 1));
        }
      }
    }
  }
}

/* MAIN PROGRAM FUNCTION */
int main(int argc, char *argv[])
{
  int rank;//unique process rank
  int commSz;//total processes using the same communicator

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &commSz);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  srand(time(NULL));
  if(rank == 0)
  {
    printf("MPI_Processes = %d\n", commSz);
    printf("Total Tasks = %d\n", commSz);
  }

  if(commSz == 1)/* SERIAL CODE */
  {
    /* ALLOCATE THE OLD AND NEW BLOCKS */
    double * oldBlock = (double *) 
      malloc(ROWS * COLS * sizeof(double));
    if(oldBlock == NULL)
    {
      printf("Proc[%d]:: Error, not enough memory...\nAborting...\n", rank);
      MPI_Abort(MPI_COMM_WORLD, -1);
    }
    double * newBlock = (double *) 
      malloc(ROWS * COLS * sizeof(double));
    if(newBlock == NULL)
    {
      printf("Proc[%d]:: Error, not enough memory...\nAborting...\n", rank);
      free(oldBlock);
      oldBlock = NULL;
      MPI_Abort(MPI_COMM_WORLD, -1);
    }
    for(int i = 0; i < ROWS; ++i)
    {
      for(int j = 0; j < COLS; ++j)
      {
        oldBlock[i*COLS+j] = 0.0;
        newBlock[i*COLS+j] = 0.0;
      }
    }

    /* INITIALISE GRID TEMPERATURE */
    heatInit(oldBlock, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    /* WRITE THE INITIAL GRID TO THE CORRESPONDING FILE */
    if(heatWriteSerial(rank, ROWS, oldBlock, COLS, 0, ROWS, 0, COLS, 0) == -1)
    {
      printf("Proc[%d]:: Error, could not create the initial file..."
             "\nAborting...\n", rank);
      free(oldBlock);
      oldBlock = NULL;
      free(newBlock);
      newBlock = NULL;
      MPI_Abort(MPI_COMM_WORLD, -1);
    }

    /* SOLVE THE PROBLEM */
    double timeStart = MPI_Wtime();//timer starts
    for(int steps = 0; steps < STEPS; ++steps)//main loop
    {
      /* CHECK FOR CONVERGENCE */
      if(steps % reductionStep == 0)
      {
        /* UPDATE THE GRID */
        int convResult = heatUpdateSerial(oldBlock, newBlock, COLS, 1, 
                                          1, ROWS-1, 1, COLS-1);
        if(convResult == 0)//convergence detected
        {
          reductionStep = STEPS + 1;//stop checking
          printf("\nConvergence detected by MPI task at step %d\n", steps);
        }
      }
      else
        heatUpdateSerial(oldBlock, newBlock, COLS, 0, 1, ROWS-1, 1, COLS-1);

      /* OLDBLOCK = NEWBLOCK SWAP */
      heatSwap(&oldBlock, &newBlock);
    }
    double timeEnd = MPI_Wtime();//timer ends
    printf("\nElapsed time was %f ms\n", 1000 * (timeEnd - timeStart));

    /* WRITE THE FINAL GRID TO THE CORRESPONDING FILE */
    if(heatWriteSerial(rank, ROWS, oldBlock, COLS, 0, ROWS, 0, COLS, 1) == -1)
    {
      printf("Proc[%d]:: Error, could not create the final file..."
             "\nAborting...\n", rank);
      free(oldBlock);
      oldBlock = NULL;
      free(newBlock);
      newBlock = NULL;
      MPI_Abort(MPI_COMM_WORLD, -1);
    }
    
    /* CLEAR UP THE HEAP */
    free(oldBlock);
    oldBlock = NULL;
    free(newBlock);
    newBlock = NULL;

    MPI_Finalize();
  }
  else /* PARALLEL MPI CODE */
  {
    /* INITIAL CHECKS ON NUMBER OF PROCESSES */
    if(commSz % 2 != 0)//odd != 1 number of procs
    {
      if(rank == 0)
      {
        printf("Can't parition grid fairly with odd number of procs = %d...\n"
               "Aborting...\n", commSz);
      }
      MPI_Abort(MPI_COMM_WORLD, -1);
    }

    /* DECLARE VARIABLES */
    int globalRowS = 0, globalRowE = 0;//global border rows
    int globalColS = 0, globalColE = 0;//global border collumns
    int nn = 0, ww = 0, ss = 0, ee = 0;//neighboors
    int offRowS = 0, offRowE = 0;//local border rows
    int offColS = 0, offColE = 0;//local border collumns
    double * oldBlock = NULL;//old version of grid
    double * newBlock = NULL;//new version of grid
    int blockRows = 0;//total rows for each block
    int blockCols = 0;//total cols for each block
    int cartRank;//process cartesian rank
    int dims[2];//Cartesian topology dimensions
    int coord[2];//Process cartesian 2d coords
    int wrapAround[2] = {0, 0};//set periodicity
    int reorder = 1;
    MPI_Comm cartComm;//cartesian communicator

    /* CLASSIFY PROBLEM SIZE BASED ON TOTAL MPI PROCESSES */
    int cut = (int) sqrt(commSz);
    double cutF = sqrt(commSz);
    int procsPerRow = 0, procsPerCol = 0;//vertical and horizontal distribution
    if(commSz == 2)//Case 1 : handling 2 procs
    {
      blockRows = ROWS;
      blockCols = COLS / commSz;
      procsPerRow = ROWS / blockRows;
      procsPerCol = COLS / blockCols;
      if((procsPerRow * procsPerCol) != commSz)
      {
        if(rank == 0)
          printf("Grid partitioning results to remains...\nAborting...\n");
        MPI_Abort(MPI_COMM_WORLD, -1);
      }
    }
    else if(cutF > (double) cut)//Case 2.1: handling 6, 8, 10, ... procs
    {
      const int probSize = COLS * ROWS;
      if(probSize % commSz != 0)//can't cut it without remains
      {
        if(rank == 0)
          printf("Grid partitioning results to remains...\nAborting...\n");
        MPI_Abort(MPI_COMM_WORLD, -1);
      }
      const int localProbSize = probSize / commSz;
      int spread = probSize;
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
      procsPerRow = ROWS / blockRows;
      procsPerCol = COLS / blockCols;
      if((procsPerRow * procsPerCol) != commSz)
      {
        if(rank == 0)
          printf("Grid partitioning results to remains...\nAborting...\n");
        MPI_Abort(MPI_COMM_WORLD, -1);
      }     
    }
    else//Case 2.2: handling 4, 16, 64, ... procs
    {
      procsPerRow = cut;
      procsPerCol = cut;

      if((ROWS % cut != 0) || (COLS % cut != 0))//can't cut even blocks BLOCK, BLOCK
      {
        if(rank == 0)
          printf("Grid partitioning results to remains...\nAborting...\n");
        MPI_Abort(MPI_COMM_WORLD, -1);
      }
      blockRows = ROWS / cut;//total rows of block + 2 extra rows halo
      blockCols = COLS / cut;//total cols of block + 2 extra cols halo
    }

    if(rank == 0)
    {
      printf("Grid can be partioned without remains...\n"
             "Rows per block : %d, Columns per block : %d\n"
             "Vertical processes : %d, Horizontal processes : %d\n\n",
             blockRows, blockCols, procsPerRow, procsPerCol);
    }

    /* CARTESIAN 2D TOPOLOGY CREATION */
    dims[0] = procsPerRow;
    dims[1] = procsPerCol;
    //create the topology
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, wrapAround, reorder, &cartComm);
    //find coordinates in the cartesian group
    MPI_Cart_coords(cartComm, rank, 2, coord);
    //use the coords to find rank in cartesian group
    MPI_Cart_rank(cartComm, coord, &cartRank);
    printf("proc[%d]: cartRank [%d], my coords = (%d,%d)\n",
    rank, cartRank, coord[0], coord[1]);

    /* NEIGHBOURHOOD DISCOVERY */
    if(commSz == 2)
    {
      if(cartRank == 0)//West Proc
      {
        nn = MPI_PROC_NULL;
        ss = MPI_PROC_NULL;
        ww = MPI_PROC_NULL;
        ee = cartRank + 1;
        globalRowS = 0;
        globalRowE = blockRows - 1;
        globalColS = 0;
        globalColE = blockCols - 1;
        offRowS = 2;
        offRowE = blockRows;
        offColS = 2;
        offColE = blockCols;
      }
      else//East proc
      {
        nn = MPI_PROC_NULL;
        ss = MPI_PROC_NULL;
        ww = cartRank - 1;
        ee = MPI_PROC_NULL;
        globalRowS = 0;
        globalRowE = blockRows - 1;
        globalColS = COLS - blockCols;
        globalColE = COLS - 1;
        offRowS = 2;
        offRowE = blockRows;
        offColS = 1;
        offColE = blockCols - 1;
      }
    }
    else
    {
      if(cartRank == 0)//NW corner
      {
        nn = MPI_PROC_NULL;
        ss = cartRank + procsPerCol;
        ww = MPI_PROC_NULL;
        ee = cartRank + 1;
        globalRowS = 0;
        globalRowE = blockRows - 1;
        globalColS = 0;
        globalColE = blockCols - 1;
        offRowS = 2;
        offRowE = blockRows;
        offColS = 2;
        offColE = blockCols;
      }
      else if(cartRank == (procsPerCol - 1))//NE corner
      {
        nn = MPI_PROC_NULL;
        ss = cartRank + procsPerCol;
        ww = cartRank - 1;
        ee = MPI_PROC_NULL;
        globalRowS = 0;
        globalRowE = blockRows - 1;
        globalColS = COLS - blockCols;
        globalColE = COLS - 1;
        offRowS = 2;
        offRowE = blockRows;
        offColS = 1;
        offColE = blockCols - 1;
      }
      else if(cartRank == ((procsPerRow * procsPerCol) - procsPerCol))//SW corner
      {
        nn = cartRank - procsPerCol;
        ss = MPI_PROC_NULL;
        ww = MPI_PROC_NULL;
        ee = cartRank + 1;
        globalRowS = ROWS - blockRows;
        globalRowE = ROWS - 1;
        globalColS = 0;
        globalColE = blockCols - 1;
        offRowS = 1;
        offRowE = blockRows - 1;
        offColS = 2;
        offColE = blockCols;
      }
      else if(cartRank == ((procsPerCol * procsPerRow) - 1))//SE corner
      {
        nn = cartRank - procsPerCol;
        ss = MPI_PROC_NULL;
        ww = cartRank - 1;
        ee = MPI_PROC_NULL;
        globalRowS = ROWS - blockRows;
        globalRowE = ROWS - 1;
        globalColS = COLS - blockCols;
        globalColE = COLS - 1;
        offRowS = 1;
        offRowE = blockRows - 1;
        offColS = 1;
        offColE = blockCols - 1;
      }
      else if(cartRank < procsPerCol)//NN side
      {
        nn = MPI_PROC_NULL;
        ss = cartRank + procsPerCol;
        ww = cartRank - 1;
        ee = cartRank + 1;
        globalRowS = 0;
        globalRowE = blockRows - 1;
        globalColS = cartRank * blockCols;
        globalColE = globalColS + blockCols - 1;
        offRowS = 2;
        offRowE = blockRows;
        offColS = 1;
        offColE = blockCols;
      }
      else if((cartRank > ((procsPerCol * procsPerRow) - procsPerCol)) && 
              (cartRank < ((procsPerCol * procsPerRow) - 1)))//SS side
      {
        nn = cartRank - procsPerCol;
        ss = MPI_PROC_NULL;
        ww = cartRank - 1;
        ee = cartRank + 1;
        globalRowS = ROWS - blockRows;
        globalRowE = ROWS - 1;
        globalColS = (cartRank % procsPerCol) * blockCols;
        globalColE = globalColS + blockCols - 1;
        offRowS = 1;
        offRowE = blockRows - 1;
        offColS = 1;
        offColE = blockCols;
      }
      else if((cartRank % procsPerCol) == 0)//WW side
      {
        nn = cartRank - procsPerCol;
        ss = cartRank + procsPerCol;
        ww = MPI_PROC_NULL;
        ee = cartRank + 1;
        globalRowS = (cartRank / procsPerCol) * blockRows;
        globalRowE = globalRowS + blockRows - 1;
        globalColS = 0;
        globalColE = blockCols - 1;
        offRowS = 1;
        offRowE = blockRows;
        offColS = 2;
        offColE = blockCols;
      }
      else if((cartRank + 1) % procsPerCol == 0)//EE side
      {
        nn = cartRank - procsPerCol;
        ss = cartRank + procsPerCol;
        ww = cartRank - 1;
        ee = MPI_PROC_NULL;
        globalRowS = ((cartRank + 1 - procsPerCol) / procsPerCol) * blockRows;
        globalRowE = globalRowS + blockRows - 1;
        globalColS = COLS - blockCols;
        globalColE = COLS - 1;
        offRowS = 1;
        offRowE = blockRows;
        offColS = 1;
        offColE = blockCols - 1;
      }
      else//general case middle process
      {
        nn = cartRank - procsPerCol;
        ss = cartRank + procsPerCol;
        ww = cartRank - 1;
        ee = cartRank + 1;
        int rowMarginS = procsPerCol;
        int rowMarginE = rowMarginS + procsPerCol - 1;
        int rowOffset = 1;
        while(1)
        {
          if((cartRank > rowMarginS) && (cartRank < rowMarginE))
          {
            globalRowS = rowOffset * blockRows;
            break;
          }
          else
          {
            rowMarginS += procsPerCol;
            rowMarginE += procsPerCol;
            ++rowOffset;
          }
        }
        globalRowE = globalRowS + blockRows - 1;
        globalColS = (cartRank % procsPerCol) * blockCols;
        globalColE = globalColS + blockCols - 1;
        offRowS = 1;
        offRowE = blockRows - 1;
        offColS = 1;
        offColE = blockCols - 1;
      }
    }
    
    /* ALLOCATE THE OLD AND NEW BLOCKS */
    oldBlock = (double *) 
      malloc((blockRows + 2) * (blockCols + 2) * sizeof(double));
    if(oldBlock == NULL)
    {
      printf("Proc[%d]:: Error, not enough memory...\nAborting...\n", rank);
      MPI_Comm_free(&cartComm);
      MPI_Abort(MPI_COMM_WORLD, -1);
    }
    newBlock = (double *) 
      malloc((blockRows + 2) * (blockCols + 2) * sizeof(double));
    if(newBlock == NULL)
    {
      printf("Proc[%d]:: Error, not enough memory...\nAborting...\n", rank);
      free(oldBlock);
      oldBlock = NULL;
      MPI_Comm_free(&cartComm);
      MPI_Abort(MPI_COMM_WORLD, -1);
    }

    /* INITIALISE BOTH TO ZERO TO SUPPRESS WARNINGS */
    for(int i = 0; i < (blockRows + 2); ++i)
    {
      for(int j = 0; j < (blockCols + 2); ++j)
      {
        oldBlock[i*(blockCols+2)+j] = 0.0;
        newBlock[i*(blockCols+2)+j] = 0.0;
      }
    }

    /* INITIALISE TEMPERATURE ON GRID */
    heatInit(oldBlock, 1, (blockCols + 2), 1, (ROWS + (cut * 2) - 1),
             1, (COLS + (cut * 2) - 1), (globalRowS + 1), (globalRowE + 1),
             (globalColS + 1), (globalColE + 1));

    /* CONVERT INITIAL GRID DATA INTO TEXT FORM */
    const int stringLength = 15;
    const char * colChange = "%-14.1lf ";//value + space delimiter
    const char * rowChange = "%-14.1lf\n";//value + newLine delimiter
    char * gridAscii = (char *) malloc((blockRows) * (blockCols) *
                                       stringLength * sizeof(char));
    if(gridAscii == NULL)
    {
      printf("Proc[%d]:: Error, not enough memory...\nAborting...\n", rank);
      free(oldBlock);
      oldBlock = NULL;
      free(newBlock);
      newBlock = NULL;
      MPI_Comm_free(&cartComm);
      MPI_Abort(MPI_COMM_WORLD, -1);
    }
    int count = 0;
    for(int i = 1; i < (blockRows + 2 - 1); ++i)
    {
      for(int j = 1; j < (blockCols + 2 - 1 - 1); ++j)
      {
        sprintf(&gridAscii[count * stringLength],
                colChange, oldBlock[i*(blockCols+2)+j]);
        count++;
      }
      if(ee == MPI_PROC_NULL)//new line delimiter
      {
        sprintf(&gridAscii[count * stringLength],
                rowChange, oldBlock[i*(blockCols+2)+(blockCols)]);
      }
      else//space delimiter
      {
        sprintf(&gridAscii[count * stringLength],
                colChange, oldBlock[i*(blockCols+2)+(blockCols)]);
      }
      count++;
    }

    /* CREATE THE STRING OUTPUT DATATYPE */
    MPI_Datatype mpiString;
    MPI_Type_contiguous(stringLength, MPI_CHAR, &mpiString);
    MPI_Type_commit(&mpiString);//commit it

    /* CREATE THE DISTRIBUTED ARRAY DATATYPE */
    MPI_Datatype distribArray;
    int globalDims[2] = {ROWS, COLS};// ROWS * COLS in global grid
    int globalDistrib[2] = {MPI_DISTRIBUTE_BLOCK,
                            MPI_DISTRIBUTE_BLOCK};//(Block, Block) partition
    int distribArgs[2] = {MPI_DISTRIBUTE_DFLT_DARG,
                          MPI_DISTRIBUTE_DFLT_DARG};//default block size
    int procDims[2] = {procsPerRow, procsPerCol};//procs vertical and horizontal
    MPI_Type_create_darray(commSz, cartRank, 2,
                           globalDims, globalDistrib, distribArgs, procDims,
                           MPI_ORDER_C, mpiString, &distribArray);
    MPI_Type_commit(&distribArray);//commit it

    /* PERFORM PARALLEL COLLECTIVE WRITE FOR THE INITIAL GRID */
    MPI_Status status;
    MPI_File fh;
    char initFilePath[80] = "";
    int randTag;
    if(cartRank == 0)
      randTag = rand() % 1000;
    MPI_Bcast(&randTag, 1, MPI_INT, 0, cartComm);//distribute value to other processes
    sprintf(initFilePath, "%d_%d_conv_parallel_Initial%d.dat", ROWS, COLS, randTag);
    if(rank == 0)
      printf("\nOutput files identifier : %d\n", randTag);
    MPI_File_open(cartComm, initFilePath,
                  MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
    MPI_File_set_view(fh, 0, MPI_CHAR, distribArray, "native", MPI_INFO_NULL);
    MPI_File_write_all(fh, gridAscii, blockRows*blockCols, mpiString, &status);
    MPI_File_close(&fh);
    
    /* CREATE THE DATATYPES */
    MPI_Datatype row;//to send rows
    MPI_Datatype col;//to send collumns
    MPI_Type_contiguous(blockCols, MPI_DOUBLE, &row);//create the row type
    MPI_Type_commit(&row);//commit it
    MPI_Type_vector(blockRows, 1, (blockCols + 2), MPI_DOUBLE, &col);//and col
    MPI_Type_commit(&col);//commit it

    /* CREATE THE STATUS AND REQUESTS ARRAYS */
    MPI_Status statusArr[4];
    MPI_Request reqArrSendOld[4];
    MPI_Request reqArrRcvOld[4];
    MPI_Request reqArrSendNew[4];
    MPI_Request reqArrRcvNew[4];
    int numReqRecvOld = 0;//to count total requests from receives
    int numReqSendOld = 0;//to count total requests from sends
    int numReqRecvNew = 0;//to count total requests from receives
    int numReqSendNew = 0;//to count total requests from sends

    int localFlag = 0;//to check for local convergence
    double timeStart = 0.0, timeEnd = 0.0;//for time measurement

    /* INITIATE PERSISTENT COMMUNICATION */
    if(nn != MPI_PROC_NULL)
    {
      MPI_Recv_init(&(oldBlock[0*(blockCols+2)+1]), 1, row,
                    nn, S_TAG, MPI_COMM_WORLD, &(reqArrRcvOld[numReqRecvOld++]));
      MPI_Send_init(&(oldBlock[1*(blockCols+2)+1]), 1, row,
                    nn, N_TAG, MPI_COMM_WORLD, &(reqArrSendOld[numReqSendOld++]));
      MPI_Recv_init(&(newBlock[0*(blockCols+2)+1]), 1, row,
                    nn, S_TAG, MPI_COMM_WORLD, &(reqArrRcvNew[numReqRecvNew++]));
      MPI_Send_init(&(newBlock[1*(blockCols+2)+1]), 1, row,
                    nn, N_TAG, MPI_COMM_WORLD, &(reqArrSendNew[numReqSendNew++]));
    }
    if(ss != MPI_PROC_NULL)
    {
       MPI_Recv_init(&(oldBlock[(blockRows+1)*(blockCols+2)+1]), 1, row,
                     ss, N_TAG, MPI_COMM_WORLD, &(reqArrRcvOld[numReqRecvOld++]));
       MPI_Send_init(&(oldBlock[blockRows*(blockCols+2)+1]), 1, row,
                     ss, S_TAG, MPI_COMM_WORLD, &(reqArrSendOld[numReqSendOld++]));
       MPI_Recv_init(&(newBlock[(blockRows+1)*(blockCols+2)+1]), 1, row,
                     ss, N_TAG, MPI_COMM_WORLD, &(reqArrRcvNew[numReqRecvNew++]));
       MPI_Send_init(&(newBlock[blockRows*(blockCols+2)+1]), 1, row,
                     ss, S_TAG, MPI_COMM_WORLD, &(reqArrSendNew[numReqSendNew++]));
    }
    if(ww != MPI_PROC_NULL)
    {
      MPI_Recv_init(&(oldBlock[1*(blockCols+2)+0]), 1, col,
                    ww, E_TAG, MPI_COMM_WORLD, &(reqArrRcvOld[numReqRecvOld++]));
      MPI_Send_init(&(oldBlock[1*(blockCols+2)+1]), 1, col,
                    ww, W_TAG, MPI_COMM_WORLD, &(reqArrSendOld[numReqSendOld++]));
      MPI_Recv_init(&(newBlock[1*(blockCols+2)+0]), 1, col,
                    ww, E_TAG, MPI_COMM_WORLD, &(reqArrRcvNew[numReqRecvNew++]));
      MPI_Send_init(&(newBlock[1*(blockCols+2)+1]), 1, col,
                    ww, W_TAG, MPI_COMM_WORLD, &(reqArrSendNew[numReqSendNew++]));
    }
    if(ee != MPI_PROC_NULL)
    {
      MPI_Recv_init(&(oldBlock[1*(blockCols+2)+blockCols+1]), 1, col,
                    ee, W_TAG, MPI_COMM_WORLD, &(reqArrRcvOld[numReqRecvOld++]));
      MPI_Send_init(&(oldBlock[1*(blockCols+2)+blockCols]), 1, col,
                    ee, E_TAG, MPI_COMM_WORLD, &(reqArrSendOld[numReqSendOld++]));
      MPI_Recv_init(&(newBlock[1*(blockCols+2)+blockCols+1]), 1, col,
                    ee, W_TAG, MPI_COMM_WORLD, &(reqArrRcvNew[numReqRecvNew++]));
      MPI_Send_init(&(newBlock[1*(blockCols+2)+blockCols]), 1, col,
                    ee, E_TAG, MPI_COMM_WORLD, &(reqArrSendNew[numReqSendNew++]));
    }
    MPI_Barrier(MPI_COMM_WORLD);//all processes reach here for synchronization
    
    /* SOLVE THE PROBLEM */
    if(cartRank == 0)
      timeStart = MPI_Wtime();//timer starts
    for(int steps = 0; steps < STEPS; ++steps)//main loop
    {
      /* COMMUNICATE WITH NEIGHBOORS */
      if(steps % 2 == 0)//even phase
      {
        MPI_Startall(numReqRecvOld, reqArrRcvOld);//receive neighboor's border at halo
        MPI_Startall(numReqSendOld, reqArrSendOld);//send border of grid to neighboor
      }
      else//odd phase
      {
        MPI_Startall(numReqRecvNew, reqArrRcvNew);//receive neighboor's border at halo
        MPI_Startall(numReqSendNew, reqArrSendNew);//send border of grid to neighboor
      }

      /* DO THE LOCAL WORK */
      if(steps % reductionStep == 0)
        localFlag = heatUpdateLocal(oldBlock, newBlock, blockCols, 1,
                                    offRowS, offRowE, offColS, offColE);
      else
        localFlag = heatUpdateLocal(oldBlock, newBlock, blockCols, 0,
                                    offRowS, offRowE, offColS, offColE);
      if(steps % 2 == 0)//even phase
        MPI_Waitall(numReqRecvOld, reqArrRcvOld, statusArr);
      else//odd phase
        MPI_Waitall(numReqRecvNew, reqArrRcvNew, statusArr);

      /* DO THE BORDER WORK */
      heatUpdateBorder(oldBlock, newBlock, blockCols,
                      offRowS, offRowE, offColS, offColE);

      /* OLDBLOCK = NEWBLOCK SWAP */
      heatSwap(&oldBlock, &newBlock);

      /* CHECK FOR CONVERGENCE */
      if(steps % reductionStep == 0)
      {
        MPI_Allreduce(&localFlag, &globalFlag, commSz,
                      MPI_INT, MPI_LOR, MPI_COMM_WORLD);
        if(globalFlag == 0)
        {
          reductionStep = STEPS + 1;//stop checking
          if(cartRank == 0)
            printf("\nConvergence detected by MPI tasks at step %d...\n", steps);
        }
      }

      if(steps % 2 == 0)//even phase
        MPI_Waitall(numReqSendOld, reqArrSendOld, statusArr);
      else//odd phase
        MPI_Waitall(numReqSendNew, reqArrSendNew, statusArr);
    }

    MPI_Barrier(cartComm);//synchronization for accurate time measurement
    if(cartRank == 0)
    {
      timeEnd = MPI_Wtime();//timer ends
      printf("Elapsed time was %f ms\n", 1000 * (timeEnd - timeStart));
    }

    /* CONVERT FINAL GRID DATA INTO TEXT FORM */
    count = 0;
    for(int i = 1; i < (blockRows + 2 - 1); ++i)
    {
      for(int j = 1; j < (blockCols + 2 - 1 - 1); ++j)
      {
        sprintf(&gridAscii[count * stringLength],
                colChange, oldBlock[i*(blockCols+2)+j]);
        count++;
      }
      if(ee == MPI_PROC_NULL)//new line delimiter
      {
        sprintf(&gridAscii[count * stringLength],
                rowChange, oldBlock[i*(blockCols+2)+(blockCols)]);
      }
      else//space delimiter
      {
        sprintf(&gridAscii[count * stringLength],
                colChange, oldBlock[i*(blockCols+2)+(blockCols)]);
      }
      count++;
    }

    /* PERFORM PARALLEL COLLECTIVE WRITE FOR THE FINAL GRID */
    char finalFilePath[80] = "";
    sprintf(finalFilePath, "%d_%d_conv_parallel_Final%d.dat", ROWS, COLS, randTag);
    MPI_File_open(cartComm, finalFilePath,
                  MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
    MPI_File_set_view(fh, 0, MPI_CHAR, distribArray, "native", MPI_INFO_NULL);
    MPI_File_write_all(fh, gridAscii, blockRows*blockCols, mpiString, &status);
    MPI_File_close(&fh);

    /* CLEAR UP THE REQUEST ARRAYS, CARTESIAN COMMUNICATOR AND DATATYPES */
    MPI_Request_free(reqArrRcvNew);
    MPI_Request_free(reqArrSendNew);
    MPI_Request_free(reqArrRcvOld);
    MPI_Request_free(reqArrSendOld);
    MPI_Type_free(&row);
    MPI_Type_free(&col);
    MPI_Type_free(&mpiString);
    MPI_Type_free(&distribArray);
    MPI_Comm_free(&cartComm);
    
    /* CLEAR UP THE HEAP */
    free(gridAscii);
    gridAscii = NULL;
    free(oldBlock);
    oldBlock = NULL;
    free(newBlock);
    newBlock = NULL;


    MPI_Finalize();
  }
}