## poly.asm -- complete program
##
## evaluate  5x^2 -12x + 97 
##
## Register Use:
##
## $10 base register, address of x
## $11 x
## $12 value of the polynomial
## $13 temporary

        .text
        .globl  main

main: 
        lui   $10,0x1001     #  Init base register
        lw    $11,0($10)     #  Load x

        ori   $12,$0,97      #  Initialize the accumulator
                             #  during the "load delay slot"

        ori   $13,$0,12      #  evaluate second term
        mult  $11,$13        #  12x
        mflo  $13            #  assume 32 bit result
        subu  $12,$12,$13    #  accumulator = -12x +97

                             #  evaluate third term
        mult  $11,$11        #  x^2
        mflo  $11            #  assume 32 bit result
        ori   $13,$0,5       #  5
        mult  $11,$13        #  5x^2
        mflo  $13            #
        addu  $12,$12,$13    #  accumulator = 5x^2-12x+97

        sw    $12,4($10)     #  Store result in poly

        .data
x:      .word   17           #  Edit this line to change the value of x
poly:   .word   0            #  Result is placed here.

## End of file