# Program to calculate lottery odds (N ana k)

.text
.globl  main

main :  li 		$v0, 4
		la 		$a0, prompt1
		syscall 			# print 1st prompt

		li  	$v0, 5
		syscall				# read N
		move 	$s0, $v0		# $s0 = N

		li 		$v0, 4
		la		$a0, prompt2	# print 2nd prompt
		syscall

		li  	$v0, 5
		syscall				# read K
		move 	$s1, $v0		# $s1 = K
# We have N and K
		subu 	$s3, $s0, $s1	# $s3 = N - K
		addiu	$s3, $s3, 1		# $s3 = N - K + 1
# We have N - K + 1 to build upper part of fraction
# First we will build the lower part
		li 		$t0, 1			# $t0 = 1
		li		$t1, 1			# sum in $t1
# While count = $t0 != K		
low  :	beq		$t0, $s1, cont		
		addiu	$t0, $t0, 1		# counter ++
		mul		$t1, $t0, $t1	# $t1 *= counter
		nop
		nop
		j		low
		nop
# Now to calculate upper part
cont :	move 	$s4, $s3		# copy $s3 to $s4 = sum

high :	beq		$s3, $s0 , exit
		addiu	$s3, $s3, 1		# $s3 ++
		mul		$s4, $s4, $s3	# sum in $s4
		nop
		nop
		j 		high
		nop		
# We have calculated upper and lower 
exit :  li  	$v0, 4
		la		$a0, msg1		# print msg1
		syscall

		divu	$s2, $s4, $t1	# $s2 = { N * (N-1) * (N-2) * (N-3) * (N-4) * ... * (N-K+1) } / (1  *   2   *   3   *  4    * ... * K)
		nop
		nop

		move 	$a0, $s2		# move odds fraction to print
		li		$v0, 1
		syscall

		li 		$v0, 10
	    syscall

.data

prompt1:	.asciiz "Please give N ( numbers the lottery has starting from 1 )\n"
prompt2:	.asciiz	"Please give K ( choises between N numbers one can pick )\n"
msg1: 		.asciiz "Your odds of winning this lottery are 1 in "	   

# N * (N-1) * (N-2) * (N-3) * (N-4) * ... * (N-K+1)
# -------------------------------------------------
#        1  *   2   *   3   *  4    * ... * K