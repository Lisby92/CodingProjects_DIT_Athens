#program to find prime numbers.
#register usage 
# $8 = base register to hold prime numbers
# $9 = current number
# $10 = $9 limit = 0xFFFF
# $11 = possible divisors
# $12 = $11 limit = $9/2
# $13 = remainder to be checked

.text
.globl  main

main :	lui $8, 0x1000		#init memory 1000 0000
		ori $9, $0, 3		#init current = 3
		ori $10, $0, 0xFFFF  #numbers to be checked 1 till 0xFFFF

while1:	beq $9, $10, Exit	#
		ori $11, $0, 2   #init divisors
		srl $12, $9, 1     #init div limit 	

while2: bgt $11, $12, w2_end 
		divu $9, $11 #current /divisor
		mfhi $13 #move remeainder to check
		sll $0, $0, 0 #noop
		sll $0, $0, 0 #noop
if1:	beq $13, $0, notPrime #if it has divisor then its not prime		
		addiu $11, $11, 1 #increment divisor
		j while2
		sll $0, $0, 0

Exit :	li $v0, 10
		syscall

w2_end: j isPrime
		sll $0, $0, 0 #noop

notPrime: addiu $9, $9, 1 #proved noit prime increment and check next number
		  j while1
		  sll $0, $0, 0 #noop

isPrime:  sw $9, 0($8)	#store prime
		  addiu $8, $8, 0x4 #increment next mem slot
		  addiu $9, $9, 1
		  j while1
		  sll $0, $0, 0 #noop		   		

.data
Array : .space 1000 #space to hold 250 prime numbers every 4 bytes		