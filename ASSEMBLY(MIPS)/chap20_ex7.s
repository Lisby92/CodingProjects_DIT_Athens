# Program that reverses the order of elements in the array. #for odd 
# Registers : 

.text
.globl  main

main :	lui 	$8, 0x1001
		lw 		$14, 0($8)		# get size
		addiu	$8, $8, 0x4		# $8 points to array start
		lui		$9, 0x1001
		ori		$10, $0, 4		#load 4 in temp
		multu	$14, $10		# size x 4
		mflo	$10				#store result in temp
		addu	$9, $9, $10		# $9 points 1 word after end of array
		ori		$11, $0, 0		#counter = 0
# While counter < size do
loop :	beq		$8, $9, exit
		lw		$12, 0($9)		#load last item in temp1
		lw		$13, 0($8)		#load first item in temp2
		sw		$12, 0($8)		#store first item in new position
		sw		$13, 0($9)		#store last item in new position

		addiu	$8, $8, 4		#move start pointer to next
		addiu	$9, $9, -4		#move end pointer to prev

		addiu	$10, $10, 1		#counter ++
		j		loop
		sll 	$0, $0, 0 		#noop branch delay

exit :	li 		$v0, 10
		syscall

.data

size :	.word 9                     # number of elements
array:	.word 1, 2, 3, 4, 5, 6, 7, 8, 9		