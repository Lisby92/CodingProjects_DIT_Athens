.text
.globl  main

main: ori $7, $0, 0x0FA5
	  ori $8, $7, 0x368F
	  andi $9, $7, 0x368F
	  xori $10, $7, 0x368F

	  li $v0, 10
	  syscall 