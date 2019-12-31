.text
.globl  main

main :	li 		$v0, 4
		la		$a0, prom
		syscall
# get x
		li 		$v0, 6
		syscall
# x in $f3
		mov.s	$f3, $f0
# $t0 = 8 terms counter to be calculated and $f1 = current term
		li 	 	$t0, 0
		li.s 	$f1, 0.0
# $f2 holds the sum of terms, $f5 holds 1.0		
		li.s	$f2, 1.0
		li.s	$f5, 1.0
		li.s	$f0, 0.0
# if x = 0, exp(0) = 1		
if1	 :	c.eq.s	$f3, $f0
		bc1t	exit
# else x != 0, calculate the terms
		add.s	$f2, $f2, $f3
# first two terms calculated
		add 	$t0, 2
# x in $f1, $f4 is the term divident		
		mov.s	$f1, $f3
		li.s	$f4, 2.0		
# while the rest terms havent been calculated do		
loop :	beq 	$t0, 50, exit				
		add 	$t0, 1
# $f1 = x * $f1		
		mul.s	$f1, $f3, $f1
# $f1 = $f1 / $f4		
		div.s	$f1, $f1, $f4
		add.s	$f4, $f4, $f5
# add it to the sum
		add.s	$f2, $f2, $f1
		j 		loop
		nop

exit :	li 		$v0, 4
		la 		$a0, resu
		syscall

		li 		$v0, 2
		mov.s	$f12, $f3
		syscall
		li 		$v0, 4
		la		$a0, res2
		syscall
		
		li 		$v0, 2
		mov.s	$f12, $f2
		syscall

		j 		main

.data

prom :	.asciiz "\nPlease give x to calculate exp(x)!\n"
resu :	.asciiz "exp("
res2 :  .asciiz ") = "	