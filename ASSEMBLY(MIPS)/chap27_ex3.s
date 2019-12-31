# Program to print square or triangle of given size(number of lines)

.text
.globl  main

main   :
# out prompt for type of figure
			li 		$v0, 4
			la		$a0, prom2
			syscall
# get answer in $s0
			li 		$v0, 5
			syscall
			move	$s0, $v0		
# out prompt for size
			li 		$v0, 4
			la		$a0, prom1
			syscall
# get size in $s1
			li 		$v0, 5
			syscall
			move	$s1, $v0
# if $s0 = 0 we want triangle			
if     :	beqz	$s0, trig		
# else we want squarre
			j		tetr
			nop

exit   :	li 		$v0, 10
			syscall
# we want triangle
trig   :	move	$a2, $s1
			li  	$t3, 0
			li 		$a1, 1
# while we havent printed all the required lines do
loop2  :	beq 	$t3, $a2, cont2
			jal		starln
			nop
			addiu	$a1, $a1, 1
			addiu	$t3, $t3, 1
			j 		loop2
			nop
# done exit
cont2  :	j 		exit
			nop
# we want square
tetr   : 	move	$a1, $s1
			li 		$t3, 0
# while we havent printed all the required lines do
loop1  :	beq 	$t3, $a1, cont1
			jal		starln
			nop
			addiu	$t3, $t3, 1
			j 		loop1
			nop
# done exit			
cont1  :	j 		exit
			nop




# function to out a line of given number of stars in $a1
starln :	li 		$t0, 0
			li 		$t1, '*'
			li 		$t2, '\n'
# while line not full of given stars do			
loop  :		beq 	$t0, $a1, cont
			sb		$t1, buff($t0)
			addiu	$t0, $t0, 1
			j		loop
			nop
# add new line and return
cont  :		sb		$t2, buff($t0)
			li 		$v0, 4
			la		$a0, buff
			syscall
			jr		$ra
			nop			 
# end of starln


.data

prom1  :	.asciiz	"Please give size (number of lines)!\n"
prom2  :	.asciiz "Please enter '1' for square '0' for triangle!\n"
buff   :	.space	100