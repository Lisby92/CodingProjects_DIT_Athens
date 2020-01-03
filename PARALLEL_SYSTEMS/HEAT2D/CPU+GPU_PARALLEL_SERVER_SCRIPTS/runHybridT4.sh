#!/bin/bash

#PBS -l pvmem=2G
#PBS -l walltime=02:00:00
#PBS -q workq
#PBS -k oe

cd $PBS_O_WORKDIR
export OMP_NUM_THREADS=4

module load openmpi3
mpirun hybrid