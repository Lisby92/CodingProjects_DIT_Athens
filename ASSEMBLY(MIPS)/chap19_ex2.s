.text
.globl  main
#program to check if a number is perfect = all his divisors add up to the number 

main: lui $8, 0x1001 # $8 is base register
	  lw  $9, 0($8)  # $9 = N(496)
	  ori $10, $0, 1	# $10 = start counter to find divisors = 1 
	  ori $11, $0, 0	#sum of divisors = 0
	  srl $12, $9, 1  # $12 = end counter = N/2
	  addiu $12, $12, 1

while:	beq $10, $12, end_while
		divu $9, $10  # N/counter
		mfhi $13     #$13 holds the remainder
		sll $0, $0, 0 #no op
		sll $0, $0, 0 #no op
		beq $13, $0, sum #if is divisor goto sum
if:		addiu $10, $10, 1 #counter ++
		j while 	#back to start
		sll $0, $0, 0 #no op 	  

end_while:	sll $0, $0, 0

check: beq $11, $9, set #if sum = N goto set
	   sll $0, $0, 0 #no op
#else it remains 0 :)
exit: 	li $v0, 10
		syscall		

sum :	addu $11, $11, $10 #sum += counter
		j if
		sll $0, $0, 0 #no op

set : 	ori $13, $0, 1 #load 1 in $13
		sw $13, 4($8)	#store our findings in isPerf
		j exit
		sll $0, $0, 0

.data

N : 	.word 496 #edit here to change number checked
isPerf:	.word 0

#dataflow #sum = 0,i = 1
#  while i <= N/2 (496/2)
#			if N/2 % i = 0 is divisor
#				sum +=i
#			i++	
# if sum = N it is perfect