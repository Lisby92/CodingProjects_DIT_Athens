		.text
		.globl main

main:	ori $8, $0, 0x0FA5
		ori $10, $8, 0x3687

		li $v0, 10
		syscall