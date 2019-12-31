.text
.globl  main

main:   lui $8, 0x1001 #base register
		lw  $9, 0($8) #x
		lw  $10, 4($8) #a
		lw  $11, 8($8) #b
		lw  $12, 12($8)#c
		lw  $13, 16($8)#d	

		multu $9, $10
		mflo $10 #=a*x
		sll  $0, $0, 0
		addu $10, $10, $11 # = a*x + b
		multu $10, $9
		mflo $10 # = a*x^2 +b*x
		sll  $0, $0, 0
		addu $10, $10, $12 # = a*x^2 +b*x + c
		multu $10, $9
		mflo $10 # = a*x^3 + b*x^2 + cx
		sll  $0, $0, 0
		addu $10 ,$10, $13

		li $v0, 10
		syscall

.data

a1:     .word    1
a2:	   .word    -3
a3:	   .word     3
a4:     .word    9
a5:     .word  -24