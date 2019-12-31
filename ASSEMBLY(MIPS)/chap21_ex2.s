# Program to do pair_wise addition between two int arrays in memory

.text
.globl  main

main :		lw		$s0, size		# $s0 = size
			ori		$t0, $zero, 0	# $t0 = counter = 0

			la		$s1, array1		# $s1 points to array1 start
			la		$s2, array2		# $s2 points to array2 start
			la		$s5, result		# $s5 points to result start
# While counter < size do
loop :		beq 	$s0, $t0, exit

			lw		$s3, 0($s1)		# store array1 item in $s3
			lw		$s4, 0($s2)		# store array2 item in $s4
			addu	$s6, $s3, $s4	# $s6 = $s3 + $s4
			sw		$s6, 0($s5)		# store in corresponding spot of result

			addiu	$s1, $s1, 4		# point to next in array1
			addiu	$s2, $s2, 4		# point to next in array2
			addiu	$s5, $s5, 4		# point to next in result
			addiu	$t0, $t0, 1		# counter ++

			j		loop			
			nop						# branch delay

exit :		li 		$v0, 10
			syscall			

.data

size  :  .word       7
array1:  .word     -30, -23, 56, -43, 72, -18, 71
array2:  .word      45,  23, 21, -23, -82,  0, 69
result:  .word       0,   0,  0,   0,   0,  0,  0