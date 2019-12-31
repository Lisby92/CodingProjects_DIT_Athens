# Program that repeatedly prompts the user for the number of miles traveled and the gallons of gasoline consumed, and then prints out the miles per gallon. 

.text
.globl  main

main :	li 		$v0, 4		# print string
		la 		$a0, msg1 	# get 1st msg prompt
		syscall

		li 		$v0, 5		# read miles
		syscall

		move 	$s0, $v0	# miles in $s0
if	 :	beqz	$s0, exit	# if miles  = 0 exit

els  :	li		$v0, 4		# print string
		la		$a0, msg2	# get 2nd msg prompt
		syscall

		li 		$v0, 5		# read galons of gasoline consumed
		syscall

		move	$s1, $v0	# galons in $s1
		divu	$s0, $s1	# miles/galons
		mflo	$a0			# move it to $a0 for printing
		nop					# delay slot

		li		$v0, 1
		syscall

		li		$v0, 4		# print string
		la		$a0, msg3	# get msg3
		syscall

		j main
		nop

exit :	li 		$v0, 10		# exit program
		syscall

.data

msg1 : 	.asciiz	"Please give miles travelled\n"
msg2 :	.asciiz	"Please give galons of gasoline consumed\n"
msg3 :  .asciiz " miles per galon!\n"