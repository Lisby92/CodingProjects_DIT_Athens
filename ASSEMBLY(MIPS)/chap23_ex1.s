# Program to test if given number is prime

.text
.globl  main

main :	li 		$v0, 4
		la 		$a0, prompt
		syscall				# print prompt for input

		li 		$v0, 5
		syscall				# read integer
		move 	$s0, $v0	# save it in $s0

		li		$t0, 2		# $t0 = 2
		divu	$t1, $s0, $t0	# $t1 = upper limit is N/2
# While $t0 < $t1 do
loop :	bge		$t0, $t1, p
		remu	$s2, $s0, $t0	# $s2 = $s0 % $t0
if   :	beqz	$s2, np			# if rem = 0 not prime		
		addiu	$t0, $t0, 1	# counter ++ to check next div
		j 		loop
		nop
		
exit :	li 		$v0, 10
		syscall

np	:	li 		$v0, 4
		la		$a0, res2
		syscall
		j 		exit
		nop

p   :	li 		$v0, 4
		la 		$a0, res1
		syscall
		j 		exit
		nop		

.data

res1 	:	.asciiz "The number you gave is prime!\n"
res2 	:	.asciiz "The number you gave is not prime!\n"
prompt	:	.asciiz	"Please give a positive integer!\n"