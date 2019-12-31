# Program that computes the average height and average weight
# Registers :
# $8  -- base register pointer
# $9  -- holds number of pairs
# $10 -- holds avg_height
# $11 -- holds avg_weight
# $12 -- counter of pairs
# $13 -- current word read

.text
.globl  main

main :	lui		$8, 0x1001		# point at pairs
		lw		$9, 0($8)		# get number of pairs
		addiu	$8, $8, 0x4		# point at first pair
		and		$10, $10, $0	# initialise avg_height = 0
		and		$11, $11, $0	# initialise avg_weight = 0
		and		$12, $12, $0	# initialise counter of pairs
# While counter < (num of pairs) do
loop :	beq 	$12, $9, exit
		lw 		$13, 0($8)		# get weight
		addu 	$10, $10, $13	# sum += weight
		addiu	$8, $8, 0x4		# point at same pair next number
		lw 		$13, 0($8)		# get height
		addu 	$11, $11, $13	# sum += height
		addiu	$8, $8, 0x4		# point at same pair next number
		addiu	$12, $12, 1		# counter ++
		j loop
		sll $0, $0, 0 			#noop

exit :	divu	$10, $12
		mflo	$10
		divu	$11, $12
		mflo	$11
		li 		$v0, 10
		syscall

.data

pairs:  .word 5                  # number of pairs
        .word 60, 90             # first pair: height, weight
        .word 65, 105
        .word 72, 256
        .word 68, 270
        .word 62, 115