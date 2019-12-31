.text
.globl  main

main:  ori $s0, $0, 0x8765
	   sll $s0, $s0, 16
	   ori $s0, $s0, 0x4321
		
	   sll $s1, $s0, 28

	   srl $s0, $s0, 4
	   sll $s2, $s0, 28
	   srl $s2, $s2, 4
	   or  $s1, $s1, $s2

	   srl $s0, $s0, 4
	   sll $s2, $s0, 28
	   srl $s2, $s2, 8
	   or  $s1, $s1, $s2

	   srl $s0, $s0, 4
	   sll $s2, $s0, 28
	   srl $s2, $s2, 12
	   or  $s1, $s1, $s2

	   srl $s0, $s0, 4
	   sll $s2, $s0, 28
	   srl $s2, $s2, 16
	   or  $s1, $s1, $s2

	   srl $s0, $s0, 4
	   sll $s2, $s0, 28
	   srl $s2, $s2, 20
	   or  $s1, $s1, $s2

	   srl $s0, $s0, 4
	   sll $s2, $s0, 28
	   srl $s2, $s2, 24
	   or  $s1, $s1, $s2

	   srl $s0, $s0, 4
	   sll $s2, $s0, 28
	   srl $s2, $s2, 28
	   or  $s1, $s1, $s2

	   li $v0, 10
	   syscall

