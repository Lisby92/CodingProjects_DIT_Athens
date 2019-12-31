.text
.globl  main

main: ori $s0, $0, 0x5555
	  sll $s0, $s0, 16
	  ori $s0, $s0, 0x5555
	  sll $s1, $s0, 1
	  or  $s2, $s0, $s1
	  and $s3, $s0, $s1
	  xor $s4, $s0, $s1

	  li $v0, 10
	  syscall