.text
.globl  main

main :   and $8, $0, $8
	   addiu $8, $8, 4096
	   addiu $8, $8, 4096
	   addiu $8, $8, 4096
	   addiu $8, $8, 4096
	   addiu $8, $8, 4096
	   addiu $8, $8, 4096
	   addiu $8, $8, 4096
	   addiu $8, $8, 4096
	   addiu $8, $8, 4096
	   addiu $8, $8, 4096
	   addiu $8, $8, 4096
	   addiu $8, $8, 4096
	   addiu $8, $8, 4096
	   addiu $8, $8, 4096
	   addiu $8, $8, 4096
	   addiu $8, $8, 4096

	     ori $9, $0, 4096
	     sll $9, $9, 4

	     ori $10, $0, 4096
	    addu $10, $10, $10
	    addu $10, $10, $10
	    addu $10, $10, $10
	    addu $10, $10, $10
	    

	    li $v0, 10
	    syscall
