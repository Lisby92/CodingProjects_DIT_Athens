# Program to Evaluate the following polynomial using Horner's method:
# ax^3 + bx^2 + cx + d
.text
.globl  main

main :		lw		$s0, x
			lw		$s1, a
			lw		$s2, bb
			lw		$s3, c
			lw		$s4, d			

			multu	$s0, $s1 		# ax
			mflo	$s5		 		# $s5 = temp
			nop
			nop

			addu	$s6, $s5, $s2	# $s6 = ax + b
			multu	$s6, $s0		# ax^2 + bx
			mflo	$s5				# $s5 = ax^2 + bx
			nop
			nop

			addu 	$s6, $s5, $s3	# ax^2 + bx + c
			multu	$s6, $s0		# ax^3 + bx^2 + cx
			mflo	$s6
			nop
			nop

			addu 	$s6, $s6, $s4	# $s6 = ax^3 + bx^2 + cx + d
			sw		$s6, result

			li 		$v0, 10
			syscall
.data


x:      .word    7
a:      .word   -3
bb:     .word    3
c:      .word    9
d:      .word  -24
result: .word    0