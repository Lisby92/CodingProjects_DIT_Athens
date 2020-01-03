#!/bin/bash

#1 node 4 tasks
qsub -l nodes=1:ppn=4 -N heat_n1_p4 runHeat.sh

#1 node 8 tasks
qsub -l nodes=1:ppn=8 -N heat_n1_p8 runHeat.sh

#2 nodes 16 tasks
qsub -l nodes=2:ppn=8 -N heat_n2_p8 runHeat.sh

#4 nodes 32 tasks
qsub -l nodes=4:ppn=8 -N heat_n4_p8 runHeat.sh

#8 nodes 64 tasks
qsub -l nodes=8:ppn=8 -N heat_n8_p8 runHeat.sh

#10 nodes 80 tasks
qsub -l nodes=10:ppn=8 -N heat_n10_p8 runHeat.sh