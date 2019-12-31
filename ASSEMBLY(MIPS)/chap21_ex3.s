# Program to compare 2 strings
.text
.globl  main

main  :		la		$s1, string1	# $s1 points to start of string1
			la		$s2, string2	# $s2 points to start of string2
			lw		$s0, result		# $s0 holds result
			ori		$t1, $zero, 0	# counter of 1st string
			ori		$t2, $zero, 0	# counter of 2nd string
# While counter1 = counter2 (same size) do
while :		bne		$t1, $t2, exit
			lbu		$t3, 0($s1)		# $t3 gets character from 1st string
			lbu		$t4, 0($s2)		# $t4 gets character from 2nd string
# If 	char1 != char2 exit				
if    :		bne		$t3, $t4, exit
els   :		addiu	$t1, $t1, 1		# counter1++
			addiu	$t2, $t2, 1		# counter2++
			addiu	$s1, $s1, 1		# point to next character of string1
			addiu	$s2, $s2, 1		# point to next character of string2
			j 		while
			nop			

set   :		addiu	$s0, $s0, 1		# result = 1
			sw		$s0, result		# store it in memory

exit  :		li 		$v0, 10
			syscall

.data

result	:  	.word     0
string1	: 	.asciiz   "c"
string2	: 	.asciiz   "c"