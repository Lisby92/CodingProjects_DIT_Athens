.text
.globl  main

main :
		li 		$t4, 0
		li		$s2, ' ' #load space in $s2
		li 		$t5, 0
		j		clear
		nop
# prompt for string#		
cont :	li 		$v0, 4
		la		$a0, prom
		syscall
# get string in buf
		li 		$v0, 8
		la		$a0, buf
		li		$a1, 200
		syscall		
# get string size in $s0, $t0 examines bytes
		li 		$s0, 0
		li		$t0, 0
	    lbu		$t0, buf($s0)
	    addiu	$s0, $s0, 1
# while we havent reached string end do	    			
size :	beqz	$t0, done1
		addiu	$s0, $s0, 1
		lbu		$t0, buf($s0)
		j 		size
		nop
# get each byte and print it like waterfall		
done1:	addiu   $s0, $s0, -1 #decrease we dont want newl
		li 		$t1, 0   # outer counter of chars read and max space to be added counter
		li 		$t3, 0
# print message
		li 		$v0, 4
		la		$a0, msg
		syscall
#outer loop			
loop1:  beq 	$t1, $s0, main 
		lbu		$t0, buf($t1)
# inner loop to add spaces		
loop3:  beq     $t3, $t1, done3
		sb		$s2, resu($t3)
		addiu 	$t3, $t3, 1
		j 		loop3
		nop
# load character after spaces		
done3:	sb  	$t0, resu($t1)
		li 		$t2, 0	 # inner counter of times printed
		li 		$t3, 0  # initialise space counter
# inner loop print it 20 times
loop2:	beq		$t2, 20, done2
		li 		$v0, 4
		la 		$a0, resu
		syscall
		li 		$v0, 4
		la 		$a0, newl
		syscall
		addiu   $t2, $t2, 1
		j 		loop2
		nop

done2:	addiu	$t1, $t1, 1
		j		loop1
		nop
# exit		
exit :  li 		$v0, 10
		syscall

clear:	beq 	$t5, 50, cont
		sb		$s2, resu($t5)
		add  	$t5, 1
		j 		clear
		nop

.data

prom : .asciiz "Please give a string!\n"
msg  : .asciiz "Resulting waterfall is : \n"
buf  : .space 200
resu : .space 200
newl : .asciiz "\n"
