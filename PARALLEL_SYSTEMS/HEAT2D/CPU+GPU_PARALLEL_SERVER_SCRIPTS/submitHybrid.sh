#!/bin/bash

#1 node 4 tasks
qsub -l nodes=1:ppn=1 -N hybrid_n1_p1_t4 runHybridT4.sh
qsub -l nodes=1:ppn=2 -N hybrid_n1_p2_t2 runHybridT2.sh

#1 node 8 tasks
qsub -l nodes=1:ppn=1 -N hybrid_n1_p1_t8 runHybridT8.sh
qsub -l nodes=1:ppn=2 -N hybrid_n1_p2_t4 runHybridT4.sh
qsub -l nodes=1:ppn=4 -N hybrid_n1_p4_t2 runHybridT2.sh

#2 nodes 16 tasks
qsub -l nodes=2:ppn=1 -N hybrid_n2_p1_t8 runHybridT8.sh
qsub -l nodes=2:ppn=2 -N hybrid_n2_p2_t4 runHybridT4.sh
qsub -l nodes=2:ppn=4 -N hybrid_n2_p4_t2 runHybridT2.sh

#4 nodes 32 tasks
qsub -l nodes=4:ppn=1 -N hybrid_n4_p1_t8 runHybridT8.sh
qsub -l nodes=4:ppn=2 -N hybrid_n4_p2_t4 runHybridT4.sh
qsub -l nodes=4:ppn=4 -N hybrid_n4_p4_t2 runHybridT2.sh

#8 nodes 64 tasks
qsub -l nodes=8:ppn=1 -N hybrid_n8_p1_t8 runHybridT8.sh
qsub -l nodes=8:ppn=2 -N hybrid_n8_p2_t4 runHybridT4.sh
qsub -l nodes=8:ppn=4 -N hybrid_n8_p4_t2 runHybridT2.sh

#10 nodes 80 tasks
qsub -l nodes=10:ppn=1 -N hybrid_n10_p1_t8 runHybridT8.sh
qsub -l nodes=10:ppn=2 -N hybrid_n10_p2_t4 runHybridT4.sh
qsub -l nodes=10:ppn=4 -N hybrid_n10_p4_t2 runHybridT2.sh