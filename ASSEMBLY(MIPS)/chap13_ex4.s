.text
.globl  main

main:  	ori $8, $0, 0xFF
		ori $9, $0, 0xAA
		sll $10, $8, 2
	   subu $8, $10, $8

	   sll $10, $9, 2
	  addu $9, $10, $9

	  subu $10, $8, $9

	  li $v0, 10
	  syscall
