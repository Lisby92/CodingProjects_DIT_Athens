# Program that repeatedly asks for string and prints out it's length.

.text
.globl  main

main :	lb		$s0, newl

		li		$v0, 4
		la		$a0, prom
		syscall				# prints out the prompt

		li		$v0, 8
		la		$a0, str
		li 		$a1, 100
		syscall				# reads the string

		lb		$s1, str 	# get first character to check if \n we exit
		li 		$t0, 1 		# start counter from 1 we have already read one char
# While read first char !- '\n' do		
loop :	beq		$s1, $s0, exit
		
		lb		$s1, str($t0)
l1   :	beq		$s1, $s0, print
		lb		$s1, str($t0)
		addiu 	$t0, $t0, 1	# counter++
		j		l1
		nop
		
print:	li 		$v0, 4
		la 		$a0, res
		syscall				# print result msg

		beq		$t0, 1, cont

		addiu 	$t0, $t0, -1 #else 

cont :	li		$v0, 1
		move	$a0, $t0	# move result and print it
		syscall

		li 		$t0, 1		# initialise counter for next string

		li		$v0, 4
		la		$a0, prom
		syscall				# prints out the prompt

		li		$v0, 8
		la		$a0, str
		li 		$a1, 100
		syscall				# reads the string

		lb		$s1, str 	# get next character to check if \n we exit
		j		loop
		nop					# repeat

exit :	li 		$v0, 10
		syscall

.data

str  :	.space	100 # max is 99 chars string
prom :	.asciiz "\nPlease give a string!\n"
res	 :	.asciiz "String's length is "
newl :	.word	'\n'