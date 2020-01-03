#!/bin/bash

mpicc -O3 -g -Wall mpi_heat2Dn.c -o heat
mpicc -O3 -g -Wall mpi_Simple_heat2D.c -o simple -lm
mpicc -O3 -g -Wall mpi_convergence_heat2D.c -o conv -lm
mpicc -O3 -g -Wall mpi+Openmp_convergence_heat2D.c -o hybrid -fopenmp -lm
hcc -g -Wall hip_heat2D.cpp -o hipHeat -lm
