#ask 1.6
.text
.globl  main

main :  li 		$v0, 4
		la 		$a0, prom1
		syscall
		
		li 		$v0, 5
		syscall

		move	$s0, $v0

		li 		$v0, 4
		la 		$a0, prom2
		syscall
		
		li 		$v0, 5
		syscall

		move 	$s1, $v0

		add		$s2, $s0, $s1
		move 	$a0, $s2

		li		$v0, 1
		syscall

		li 		$v0, 4
		la 		$a0, newl
		syscall

		sub		$s2, $s0, $s1
		move	$a0, $s2

		li 		$v0, 1
		syscall



exit : li 		$v0, 10
	   syscall

.data

prom1 : .asciiz "Please give 1st integer!\n"
prom2 : .asciiz "Please give 2nd integer\n"
resu : .asciiz "Results are : \n"
newl : .asciiz "\n"