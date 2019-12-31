.text
.globl  main
## to calculate 17xy - 12x - 6y + 12 = answer
## Register usage :
## $8 = base register
## $9 = answer 
## $10 = x
## $11 = y
## $12 = answer
## $13 = tempo
## $14 = tempo

main:		lui $8, 0x1001
			lw  $10, 0($8)  
			ori $12, $0, 12
			lw	$11, 4($8)
		   mult $12, $10
		   mflo $13  # = 12x
		   addi $14, $0, -6
		   mult $14, $11
		   subu $12, $12, $13 ## = -12x +12
		   mflo $13 # = -6y
		   ori  $14, $0, 17
		   mult $14, $10
		   addu $12, $12, $13 ## = -12x +12 -6y
		   mflo $14
		   sll $0, $0, 0
		   sll $0, $0, 0
		  mult $14, $11
		  mflo $14
		  sll $0, $0, 0
		  addu $12, $14, $12
		  sw $12, 8($8)


		   


			li $v0, 10
			syscall

.data

x:		.word -24
y:		.word -3
answer: .word 0			