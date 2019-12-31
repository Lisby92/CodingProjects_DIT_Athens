.text
.globl  main

main : 	ori $8, $0, 0x186A
		sll $8, $8, 8
		ori $9, $0, 0x1388
		sll $9, $9, 4
		ori $10, $0, 0x61A8
		sll $10, $10, 4

		div $8, $10
		mflo $12
		sll $0, $0, 0
		sll $0, $0, 0
	  multu $12, $9
	   mflo $12

	   li $v0, 10
	   syscall