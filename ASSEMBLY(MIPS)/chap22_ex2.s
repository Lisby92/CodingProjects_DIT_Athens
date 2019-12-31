# Program that repeatedly prompts the user for the number of miles traveled and the gallons of gasoline consumed, and then prints out the miles per gallon floatishly. 

.text
.globl  main

main :	li 		$v0, 4		# print string
		la 		$a0, msg1 	# get 1st msg prompt
		syscall

		li 		$v0, 5		# read miles
		syscall

		li 		$s2, 100	# $s2 = 100

if	 :	beqz	$v0, exit	# if miles  = 0 exit

els  :	multu	$v0, $s2	# miles x 100
		mflo 	$s0	# miles in $s0
		nop
		nop
		
		li		$v0, 4		# print string
		la		$a0, msg2	# get 2nd msg prompt
		syscall

		li 		$v0, 5		# read galons of gasoline consumed
		syscall
		move	$s1, $v0	# galons in $s1
		
		divu	$s0, $s1	# miles/galons
		mflo	$s0			# move it to $s0
		nop					# delay slot
		nop
		divu	$s0, $s2	# divide it with 100
		mflo	$a0			# move it to $a0 to print whole gallons
		mfhi	$s0			# store hundrenths in s0 temporarily
		li		$v0, 1		# print $a0 content
		syscall

		li		$v0, 4		# print string
		la		$a0, peri	# get "."
		syscall

		move 	$a0, $s0
		li		$v0, 1		# print rest number
		syscall

		li		$v0, 4		# print msg3
		la 		$a0, msg3
		syscall

		j main
		nop

exit :	li 		$v0, 10		# exit program
		syscall

.data

msg1 : 	.asciiz	"Please give miles travelled\n"
msg2 :	.asciiz	"Please give galons of gasoline consumed\n"
msg3 :  .asciiz " miles per galon!\n"
peri :	.asciiz "."