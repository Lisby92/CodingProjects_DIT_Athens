# Program to reverse strings

.text
.globl  main

main :	lb		$s0, newl	# $s0 = \n not to be changed

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
		
		lb		$s1, str($t0)# read second char
l1   :	beq		$s1, $zero, init
		addiu 	$t0, $t0, 1	 # counter++
		lb		$s1, str($t0)
		j		l1
		nop
# While counter >= 0 do		
init:	addiu	$t0, $t0, -1 # we dont want last two character \n and 0
		li 		$t1, 0		 # counter to add in the reversed string
rvrs:	bltz	$t0, cont
		lb		$s1, str($t0)
		sb		$s1, str1($t1)
		addiu 	$t1, $t1, 1
		addiu	$t0, $t0, -1
		j rvrs
		nop

cont :	li 		$v0, 4
		la 		$a0, res
		syscall				# print result msg

		sb		$zero, str1($t1) # add string null char

		li		$v0, 4
		la		$a0, str1	# point to reversed string and print it
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
str1 :  .space  100 # reversed string to be hold here same size obviously
prom :	.asciiz "\nPlease give a string!\n"
res	 :	.asciiz "Reversed string is "
newl :	.word	'\n'