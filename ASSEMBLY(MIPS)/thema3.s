.text
.globl  main

main :	li 		$s0 , 0xAAAAAA12
		li 		$s1,  0x55555512
		li 		$s2, 0
		li 		$t0, 0
		li 		$t1, 0
		li 		$t2, 0
loop :	beq		$t2, 32, exit
		beqz	$s0, exit
		beqz	$s1, exit
		sll 	$t0, $s0, 31
		srl		$t0, $t0, 31
		sll		$t1, $s1, 31
		srl		$t1, $t1, 31
		add		$t2, 1
		srl		$s0, $s0, 1
		srl		$s1, $s1, 1
		
if  :	beq		$t0, $t1, fnd		
		j 		loop
		nop

fnd :	add 	$s2, 1
		j 		loop
		nop

exit :	li 		$v0, 1
		move 	$a0, $s2
		syscall

		li 		$v0, 10
		syscall

.data			