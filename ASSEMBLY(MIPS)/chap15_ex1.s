.text
.globl  main

#$10 = result
#$11 = tempo
main :	   lui $8, 0x1001 #base register = $8
		   lw  $9, 0($8)  #load x in $9
		   ori $11, $0, 5
		  mult $9, $11
		  mflo $10 #5x
		   ori $11, $0, 3
		  mult $9, $9
		  mflo $9 #x^2
		  addi $10, $10, -12 #5x-12
		  mult $9, $11
		  mflo $11
		   sll $0, $0, 0
		  addu $10, $10, $11 #$10= $3x^2+5x-12
		  sw $10, 4($8)

	     li $v0, 10
	     syscall

.data

x: .word 0
poly: .word 0    
