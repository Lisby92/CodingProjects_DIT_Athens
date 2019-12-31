# Program to print Star christmas tree up to down of defined size
.text
.globl  main

main :	
# prompt for new tree
		li 		$v0, 4
		la 		$a0, msge
		syscall
# get number of lines in $t7
		li 		$v0, 5
		syscall
		move	$t7, $v0
# $t0 = lines loop counter (we gonna do a $t7-line tree)				
		li 		$t0, 0
# store the chars we need in $s0,$s1,$s2.
		lb		$s0, spac
		lb		$s1, star
		lb		$s2, newl
# now we have to find the middle where the first star will be put
		li 		$t6, 0
		addiu	$t6, $t7, 1
		divu	$t6, $t6, 2		
# pointers to add bytes in string $t1 = left, $t2 = right initialised at middle
		move 	$t1, $t6
		move 	$t2, $t6
# for($t0 = 0; $t0 < 5, $t0 ++)
loop :	beq		$t0, $t7, exit
# store the stars in right and left first run their position is same
		sb		$s1, buff($t1)
		sb		$s1, buff($t2)
# print the top of the tree 1st run, then rest
		li 		$v0, 4
		la		$a0, buff
		syscall
# out a new line
		li 		$v0, 4
		la		$a0, new
		syscall		
# points left and right to the next slot
		addiu	$t2, $t2, 1
		addiu	$t1, $t1, -1
# line counter ++		
		addiu	$t0, $t0, 1
# loop again		
		j 		loop
		nop
# play again
		b		main
		nop

exit :	li 		$v0, 10
		syscall				

.data

star :	.byte	'*'
spac :	.word	' '
newl :	.word	'\n'
buff :  .space  100  # max 99 stars + 1 stop for new line
msge :	.asciiz "\nPlease give number of tree height in range [1,99]\n"
new  :  .asciiz "\n"
