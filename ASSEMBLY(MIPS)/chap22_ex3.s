# Program to convert read string of digits into an integer

.text
.globl  main

main :	li 		$v0, 4
		la		$a0, msg	# prompt for string
		syscall

		li 		$v0, 8
		la		$a0, buf
		li		$a1, 5
		syscall

		li  	$s4, 10
		li		$s0, 0		# $s0 = value = 0
		li		$s2, 0xA	# $s2 = new line = 0xA in ascii
		lbu		$s1, 0($a0) # get first byte in $s1
# While current byte != \n
loop:	beq		$s1, $s2, exit
		subu	$s1, $s1, 0x30 #subtract 0x30 to conv to integer
		multu	$s0, $s4
		mflo	$s5
		nop
		nop
		addu	$s0, $s0, $s5
		addu	$s0, $s0, $s1
		addiu	$a0, $a0, 1		# point to next byte
		lbu		$s1, 0($a0) # get next byte in $s1
		j loop
		nop

exit :	move 	$a0, $s0
		li 		$v0, 1
		syscall

		li 		$v0, 10
		syscall

.data

buf : 	.space  5 # 5 bytes space for 5 digits	
msg  :  .asciiz "Plz give a string of digits max 65536\n"