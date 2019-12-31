.text
.globl  main

main :  sub     $8, $0, 4
		ori 	$9, $0, 3
		multu   $9, $8
		mflo	$10
		addiu	$10, $10, 7
		ori 	$9, $0, 2
		multu   $9, $8
		mflo 	$11
		addiu   $11, $11, 8
		divu	$10, $11

		li $v0, 10
		syscall