#!/bin/bash

#1 node 1 task
qsub -l nodes=1:ppn=1 -N conv_n1_p1 runConv.sh

#1 node 4 task
qsub -l nodes=1:ppn=4 -N conv_n1_p4 runConv.sh

#1 node 8 tasks
qsub -l nodes=1:ppn=8 -N conv_n1_p8 runConv.sh

#2 nodes 16 tasks
qsub -l nodes=2:ppn=8 -N conv_n2_p8 runConv.sh

#4 nodes 32 tasks
qsub -l nodes=4:ppn=8 -N conv_n4_p8 runConv.sh

#8 nodes 64 tasks
qsub -l nodes=8:ppn=8 -N conv_n8_p8 runConv.sh

#10 nodes 80 tasks
qsub -l nodes=10:ppn=8 -N conv_n10_p8 runConv.sh