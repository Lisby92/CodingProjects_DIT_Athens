.text
.globl  main

main :	   ori $8, $0, 10 #load x = 1 in $8
	     multu $8, $8    # = x^2
	      mflo $9		 #load $9 with lo
	       sll $0, $0, 0 # no-op
	       ori $10, $0, 3 # load 3 in $10
	     multu $9, $10  # = 3*x^2
	      mflo $9
	       ori $10, $0, 5
	     addiu $9, $9, -12
	     multu $10, $8
	     mflo  $10
	     addu  $9, $9, $10

	     li $v0, 10
	     syscall
