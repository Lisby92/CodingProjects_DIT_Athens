# Program to remove spaces from string
# Registers :
#  $8 -- ptr to string start
#  $9 -- holds current string character
# $10 -- holds space = 0x20
# $11 -- ptr to current character
# $12 -- ptr to dest character

.text
.globl  main

main :	ori 	$10, $0, 0x20 	# $10 = space
		lui 	$11, 0x1001		# points to current = string start
		lbu 	$9, 0($11)  	# get first character
		lui 	$12, 0x1001		# points to dest = string start
# while current != null do
loop :	beqz 	$9, exit 
# if current = space 
if   :	beq  	$9, $10, skip	#		
		sb	 	$9, 0($12)		# store it in dest
		addiu	$12, $12, 1		# point dest to next available position
		
els  :	addiu	$11, $11, 1		# point current to next character
		lbu 	$9, 0($11)  	# get next character
		j loop
		sll 	$0, $0, 0 		# jump delay

		sb		$9, 0($12)		# store end character at end
		addiu	$12, $12, 1		# point dest to next available position
end  :	bgt		$12, $11, exit
		sb		$9, 0($12)
		j end
		sll 	$0, $0, 0 		# jump delay

exit:	li 		$v0, 10
		syscall
# is space so skip it
skip  :	j els
		sll 	$0, $0, 0 		# jump delay 		

.data

str1 : .asciiz "Is  this a dagger    which I see before me"