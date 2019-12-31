.text
.globl  main

main :  addiu $8, $0, -1
		sll $10, $8, 4
	    subu $10, $10, $8
	    subu $10, $10, $8
	    subu $10, $10, $8

	    subu $11, $0, $10

	    li $v0, 10
	    syscall