# hellospim.s
#
        .text
        .globl  main
main:
        li      $v0,4       # code 4 == print string
        la      $a0,string  # $a0 == address of the string
        syscall             # Invoke the exception handler.

        li      $v0,10      # code 10 == exit
        syscall             # Halt the program.

        .data
string: .asciiz      "Hello SPIM!\n"
