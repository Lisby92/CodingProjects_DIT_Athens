#program to calculate GCD (MKD) between 2 integers.
.text
.globl  main

main :		lui $8, 0x1001 # $8 = base register
			lw  $9, 0($8)  # $9 = M
			lw  $10, 4($8) # $10 = N

while:		beq $9, $10, set #while N!=M
if1 :		bgt $10, $9, endif1
els1:		subu $9, $9, $10  # M = M - N
			j while
			sll $0, $0, 0 #nop

set : 		sw $9, 8($8) #store in result

Exit:		li $v0, 10
			syscall			

endif1:		subu $10, $10, $9 # N = N - M
			j while
			sll $0, $0, 0 #nop

.data
M : 	 .word 40
N : 	 .word 100
result : .word 0

#get N
#get M

#while (N != M)
    #if ( N > M )
     #   N = N - M;
    #else
   #     M = M - N;

#GCD =  N;