# Fahrenheit/Celsius Converter

.text
.globl  main

main :	li		$v0, 4
		la		$a0, msg1	# print prompt for scale
		syscall
		li		$v0, 8
		la		$a0, code	# read the scale
		li		$a1, 4
		syscall		

		lw		$s1, c1		# load the codes in $s1,$s2,$s3
		lw		$s2, c2
		lw		$s3, c3
		lb		$s0, 0($a0) # load code given in $s0

		li		$s4, 5	# values we want for our calcs
		li		$s5, 9	

loop :	beq		$s0, $s3, exit

		li		$v0, 4
		la 		$a0, msg2 # prompt to read temperature
		syscall
		li 		$v0, 5	# get temperature
		syscall

if_c :	beq		$s0, $s1, cels
		# else we want Farhenteit
		move	$t0, $v0	# F
		addiu	$t0, $t0, -32 # F -32
		multu	$t0, $s4	# 5x(F-32)
		mflo	$t0			# $t0 = 9xC
		nop
		nop
		divu	$t0, $s5  # 5x(F-32)/9
		mflo	$t0		  # $t0 = 5x(F-32)/9
		nop
		nop
		j cnt_f
		nop
		# continue cels
cnt_c:	la 		$a0, msg4 # print msg4
		li		$v0, 4
		syscall
		move	$a0, $t0 # get result
		li 		$v0, 1
		syscall			# print it
		la 		$a0, F
		li 		$v0, 4
		syscall			# print F and newline
		j cont
		nop
		# continue farh
cnt_f:	la 		$a0, msg3 # print msg3
		li		$v0, 4
		syscall
		move	$a0, $t0 # get result
		li 		$v0, 1
		syscall			# print it
		la 		$a0, C
		li 		$v0, 4
		syscall			# print C and newline

cont :	li		$v0, 4
		la		$a0, msg1	# print prompt for scale
		syscall
		li		$v0, 8
		la		$a0, code	# read the scale
		li		$a1, 4
		syscall		
		lb		$s0, 0($a0) # load code given in $s0

		j loop
		nop		

exit : 	li		$v0, 4
		la		$a0, msg5
		syscall
		
		li 		$v0, 10
		syscall

cels :	move	$t0, $v0	# C
		multu	$t0, $s5	# 9xC
		mflo	$t0			# $t0 = 9xC
		nop
		nop
		divu	$t0, $s4  # 9xC / 5
		mflo	$t0		  # $t0 = 9xC/5
		nop
		nop
		addiu	$t0, $t0, 32	# $t0 = 9xC/5 + 32
		j		cnt_c
		nop

.data

msg1:	.asciiz "Enter scale :\n"
msg2:	.asciiz "Enter Temperature :\n"
msg3:	.asciiz "Celsius Temperature : "
msg4:	.asciiz "Fahrenheit Temperature : "
msg5:	.asciiz "Done!\n"
code:	.space	4 # 4 byte for 1 character
c1	:	.word 	'c'
c2	:	.word	'f'
c3	:	.word	'q'
C   :	.asciiz "C\n"
F 	:	.asciiz "F\n"
#F = (9/5)C + 32

#C = (5/9)(F - 32)