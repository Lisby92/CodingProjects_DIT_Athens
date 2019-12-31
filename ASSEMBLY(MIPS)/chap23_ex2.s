# Program to calculate integer square root of given number

.text
.globl  main

main :	li 		$v0, 4
		la 		$a0, prompt1
		syscall					# print prompt

		li 		$v0, 5
		syscall					# read integer
		move	$s0, $v0		# save it in $s0

		li		$t0, 0			# $t0 used as counter 
		mul		$t1, $t0, $t0	# $t1 used for squares

loop :	bgt		$t1, $s0, exit		

		addiu	$t0, $t0, 1		# counter ++
		mul		$t1, $t0, $t0	# continue finding squares
		j		loop
		nop

exit : li 		$v0, 4
	   la 		$a0, msg1
	   syscall				# print 1st msg

	   move 	$a0, $s0 	# move integer read to $a0 for printing
	   li 		$v0, 1
	   syscall

	   li 		$v0, 4		# print rest of msg
	   la 		$a0, msg2
	   syscall

	   addiu	$t0, $t0, -1	# we have the larger so need to go back 1 integer
	   move		$a0, $t0		# move sqrt to $a0 for printing
	   li 		$v0, 1
	   syscall

	   li 		$v0, 10
	   syscall

.data

prompt1 : .asciiz "Please give  a positive integer!\n"
msg1	: .asciiz "iSqrt("
msg2	: .asciiz ") == "