# Program that repeatedly asks the user for two strings. Print out the new string that is the concatenation of the two strings.
.text
.globl  main

main :	li 		$t0, 0			# $t0 = counter1
		li 		$t1, 0  		# $t1 = counter2
# Prompt for 1st string
		li 		$v0, 4
		la 		$a0, prm1
		syscall
# Get 1st string
		li 		$v0, 8
		la 		$a0, str1
		li 		$a1, 100
		syscall
# Count first string's length without \n\0
		lb		$s1, str1($t0)	# get 1st char
		addiu	$t0, $t0, 1		# counter1 ++
# While read char != \n do
l1	 :	beq		$s1, $zero, con1
		lb		$s1, str1($t0)	# get 1st char
		addiu	$t0, $t0, 1		# counter1 ++
		j 		l1
		nop
# Prompt for 2nd string
con1 :	li 		$v0, 4
		la 		$a0, prm2
		syscall		
		li 		$v0, 8
		la 		$a0, str2
		li 		$a1, 100
		syscall
# Count second string's length with \n
		lb		$s1, str2($t1)	# get 1st char
		addiu	$t1, $t1, 1		# counter2 ++
# While read char != \n do	
l2   :	beq		$s1, $zero, con2
		lb		$s1, str2($t1)	# get 1st char
		addiu	$t1, $t1, 1		# counter2 ++
		j 		l2
		nop
# Create concatenation of the two strings 
con2 :	li 		$t2, 0			# $t2 = counter to add 1st string
		li		$t3, 0			# $t2 = counter to add 2nd string		
		addiu 	$t0, $t0, -2 	# skip last character
# While we havent reached 1st strings \n do
l3	 :	beq		$t2, $t0, con3
		lb		$s1, str1($t2)  # load 1st strings chars
		sb		$s1, str3($t2)	# store the chars in corresponding position of counter $t2
		addiu	$t2, $t2, 1		# point to next char
		j		l3
		nop
# While we havent reached 		
con3 :	beq		$t3, $t1, prnt
		lb		$s1, str2($t3)	# load 2nd string's chars
		sb		$s1, str3($t2)	# continue storing after 1st string's end
		addiu	$t2, $t2, 1		# correct counter's position
		addiu 	$t3, $t3, 1		# point to next char of str2
		j		con3
		nop

prnt :  li 		$v0, 4
		la 		$a0, msg1
		syscall					# print result msg

		addiu	$t2, $t2, 1
		lb		$zero, str3($t2)
		li 		$v0, 4			# print concatenation string
		la 		$a0, str3
		syscall

		b 		main

.data

str1 :	.space 100
str2 :	.space 100
str3 :	.space 200
prm1 :	.asciiz "\nPlease give 1st string!\n"
prm2 :	.asciiz "Please give 2nd string!\n"
msg1 :	.asciiz	"Concatenation of the two strings is : "