.text
.globl  main

main:	lui $8, 0x1000 #base register = $8
		ori $9, $0, 0  #counter $9 , start = 0
		ori $10, $0, 25 #end of loop 25 times

loop:	beq $9, $10, exit #exit after 25 times
		sw  $9, 0($8)
		addiu $8, $8, 0x4 #offset +=0x4 to go to next 4 bytes
		addiu $9, $9, 1	#counter +=1
		j loop
		sll $0, $0, 0 #no-op

exit: 	li $v0, 10
		syscall

.data

array : .space 100