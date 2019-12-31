.text
.globl  main

main: ori $7, $0, 0xD
	  sll $7, $7, 4
	  ori $7, $7, 0xE
	  sll $7, $7, 4
	  ori $7, $7, 0xA
	  sll $7, $7, 4
	  ori $7, $7, 0xD
	  sll $7, $7, 4



	  li $v0, 10
	  syscall