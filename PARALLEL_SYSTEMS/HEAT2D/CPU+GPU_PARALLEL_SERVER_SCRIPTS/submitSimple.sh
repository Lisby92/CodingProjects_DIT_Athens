#!/bin/bash

#1 node 1 task
qsub -l nodes=1:ppn=1 -N simple_n1_p1 runSimple.sh

#1 node 4 tasks
qsub -l nodes=1:ppn=4 -N simple_n1_p4 runSimple.sh

#1 node 8 tasks
qsub -l nodes=1:ppn=8 -N simple_n1_p8 runSimple.sh

#2 nodes 16 tasks
qsub -l nodes=2:ppn=8 -N simple_n2_p8 runSimple.sh

#4 nodes 32 tasks
qsub -l nodes=4:ppn=8 -N simple_n4_p8 runSimple.sh

#8 nodes 64 tasks
qsub -l nodes=8:ppn=8 -N simple_n8_p8 runSimple.sh

#10 nodes 80 tasks
qsub -l nodes=10:ppn=8 -N simple_n10_p8 runSimple.sh