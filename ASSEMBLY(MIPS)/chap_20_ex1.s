#program to convert a upper case string of known size to lower
# registers :
#  $8 -- ptr to string start
# $10 -- holds current string character

.text	
.globl  main

# Initialize
main :	lui 	$8, 0x1001 		# ptr points to string start
		lbu		$10, 0($8)		# get first character
# While we havent reached string end do
loop :	beqz 	$10, exit 		# found null character exit			
		addiu	$10, $10, 0x20  # convert it to upper
		sb		$10, 0($8)		# store it at same position
		addiu 	$8, $8, 1		# point to next character
		lbu		$10, 0($8)		# get next character
		j loop
		sll $0, $0, 0 			# nop delay branch

exit:	li $v0, 10
		syscall


.data

str : .asciiz "SALAMSALUT"