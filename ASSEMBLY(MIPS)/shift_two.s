.text
.globl  main

main: 	ori $8, $0, 0x6F
		sll $9, $8, 2

		li $v0, 10
		syscall