# Program to compute the dot product of two vectors
.text
.globl  main

main :	
# print 1st prompt
		li 		$v0, 4
		la		$a0, prompt0
		syscall
# get length of vectors in $s0
		li		$v0, 5
		syscall
		move	$s0, $v0
		sw		$s0, length
# initialise 2 vector indexes $t1,$t2 and loop counter $t3		
		li 		$t1, 0
		li		$t2, 0
		li		$t3, 0
# for($t3 = 0;$t3 < $s0;$t3 ++)
l1	:	beq		$t3, $s0, con1
# print prompt for integer
		li		$v0, 4
		la 		$a0, prompt1
		syscall
# get 1st vectors integers
		li		$v0, 5
		syscall
		move	$t4, $v0
# store and increment index1 to point next word by 4 , and loop counter ++
		sw		$t4, vectorA($t1)
		addiu	$t1, $t1, 4
		addiu	$t3, $t3, 1
		j		l1
		nop
# do the same for vectorB
con1 :	li		$t3, 0  
l2	 :	beq		$t3, $s0, con2
# print prompt for integers
		li		$v0, 4
		la 		$a0, prompt2
		syscall
# get 2nd vector's integers
		li		$v0, 5
		syscall
		move	$t4, $v0
# store and increment index2 to point next word by 4, and loop counter ++		
		sw		$t4, vectorB($t2)
		addiu	$t2, $t2, 4
		addiu	$t3, $t3, 1
		j		l2
		nop
# vectors filled now to calculate the DOT product		
con2 :  li		$t1, 0
		li		$t2, 0
		li		$t3, 0
# we need 2 more registers for total sum and 2 temporary thats $s1, $t6, $t7
		li		$s1, 0
		li		$t6, 0
		li		$t7, 0
# for ($t3 = 0;$t3 < $s0;$t3 ++)
l3	:	beq		$t3, $s0, con3
# get words in temps from vectors indexes
		lw		$t6, vectorA($t1)
		lw		$t7, vectorB($t2)
# increment indexes and loop counter
		addiu	$t3, $t3, 1
		addiu	$t1, $t1, 4
		addiu	$t2, $t2, 4
# calculate the product and add to sum
		mul		$t6, $t6, $t7
		addu 	$s1, $s1, $t6
		j 		l3
		nop
# print the result prompt
con3 :	li		$v0, 4
		la 		$a0, prompt3
		syscall
# print the result dot product
		li		$v0, 1
		move	$a0, $s1
		syscall

exit : 	li 		$v0, 10
		syscall

.data

length :  .word   0	   # length of vector
vectorA:  .space  40    # space for 10 integers
vectorB:  .space  40    # space for 10 integers
prompt0:  .asciiz "Enter length of vectors\n"
prompt1:  .asciiz "Give the values of 1st vector's elements!\n"
prompt2:  .asciiz "Give the values of 2nd vector's elements!\n"	
prompt3:  .asciiz "The dot product of given vector's is : "	