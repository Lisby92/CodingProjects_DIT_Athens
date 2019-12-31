# Program to print Star christmas tree up to down of defined size
.text
.globl  main

main :	li 		$t0, 0		# lines loop counter (we gonna do a 5-line tree)
# store the chars we need in $s0,$s1,$s2.
		lb		$s1, star
#		lb		$s0, spac
#		lb		$s2, newl
# pointers to add bytes in string $t1 = left, $t2 = right
		li 		$t1, 4
		li 		$t2, 4
# for($t0 = 0; $t0 < 5, $t0 ++)
loop :	beq		$t0, 5, exit
# store the stars in right and left first run this is nop
		sb		$s1, strn($t1)
		sb		$s1, strn($t2)
# print the top of the tree 1st run, then rest
		li 		$v0, 4
		la		$a0, strn
		syscall
# points left and right to the next slot
		addiu	$t2, $t2, 1
		addiu	$t1, $t1, -1
# line counter ++		
		addiu	$t0, $t0, 1
# loop again		
		j 		loop
		nop

exit :	li 		$v0, 10
		syscall				

.data

star :	.byte	'*'
#spac :	.word	' '
#newl :	.word	'\n'
strn :	.ascii	"    *    \n"
#buff :  .space 100  # max 99 stars + 1 stop for new line 