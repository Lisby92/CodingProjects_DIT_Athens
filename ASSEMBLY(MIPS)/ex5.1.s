.text
.globl  main

main :	li 		$v0, 4
		la 		$a0, prom
		syscall

		li 		$v0, 6
		syscall

		mov.s	$f12, $f0

		li 		$v0, 2
		syscall

		j 		main
		syscall

exit : 	li 		$v0, 10
		syscall


.data

prom : .asciiz "\nPlease give a float!\n"		