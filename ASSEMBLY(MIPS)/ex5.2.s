.text
.globl  main

main :	l.s		$f1, x
		l.s		$f2, y
		l.s		$f3, none

		li 		$v0, 1
		mov.s	$f12, $f1
		syscall

		li 		$v0, 2
		mov.s	$f12, $f2
		syscall

		li		$v0,2
		mov.s	$f12, $f3
		syscall

exit :	li 		$v0, 10
		syscall


.data

x	 :	.float 1.0  		# +15
y	 :	.float -13.0  # -16
zero :	.float 0.0		  	  # 0
pInf :	.float 2147483647.0  # +2147483647
nInf :	.float -2147483648.0  # -2147483648
none :  .word NaN		  # Not_A_Number