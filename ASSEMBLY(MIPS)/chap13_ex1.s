.text
.globl  main

main: 	ori $8, $0, 456
	  addiu $8, $8, -229
	  addiu $8, $8, 325
	  addiu $8, $8, -552

	  li $v0, 10
	  syscall