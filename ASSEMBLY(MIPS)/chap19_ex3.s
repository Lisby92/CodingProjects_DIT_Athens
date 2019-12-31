.text
.globl  main
# program to find all perfect numbers up to 0xFFFF. 
# register_use below #
# $8 is base register
# $9 is counter for while2 , $14 is the outer limit
# $10 is counter for while1, $12 is the inner limit
# $13 holds remainder/used for calc
# $11 is the sum of divisors

main: 	lui $8, 0x1000 # handles isPerf
	  	ori  $9, $0, 2  # check all numbers starting from 2
	  	ori  $14, $0, 0xFFFF # until reg limit
	  	

while2: 	beq $9, $14, exit #outer while
			and $11, $11, $0	#initialise sum for next number
			and $10, $10, $0   	#initialise inner counter
			ori $10, $10, 1
			srl $12, $9, 1   # $12 = ($9)/2 inner limit
			addiu $12, $12, 1 #increment by one

while1:			beq $10, $12, end_wh1 #inner while
				divu $9, $10  # N/counter
				mfhi $13     #$13 holds the remainder
				sll $0, $0, 0 #no op
				sll $0, $0, 0 #no op
				beq $13, $0, sum #if is divisor goto sum
if1:				addiu $10, $10, 1 #counter ++
					j while1 	#back to start
					sll $0, $0, 0 #no op 	  
end_wh1:		beq $11, $9, set #if sum = N goto set
if2:			addiu $9, $9, 1# $9++
				j while2
				sll $0, $0, 0 #no op
				
exit: 	li $v0, 10
		syscall		

sum :	addu $11, $11, $10 #sum += counter
		j if1
		sll $0, $0, 0 #no op

set : 	and $13, $13, $0 #initialise 13 to hold our finding
		or $13, $0, $11 #load perfect number in $13
		sw $13, 0($8)	#store our findings in isPerf
		addiu $8, $8, 0x4 #increment base register to hold perfect in next 4 bytes
		j if2
		sll $0, $0, 0

.data

isPerf : 	.space 24 #holds perfect numbers every 4 bytes


#dataflow #sum = 0,i = 1
#  while i <= N/2 (496/2)
#			if N/2 % i = 0 is divisor
#				sum +=i
#			i++	
# if sum = N it is perfect