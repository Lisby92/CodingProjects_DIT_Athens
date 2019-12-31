## newton.asm -- compute sqrt(n) 

## given an approximation x to sqrt(n),
## an improved approximation is:

## x' = (1/2)(x + n/x)

## $f0  ---  n
## $f1  ---  1.0
## $f2  ---  2.0
## $f3  ---  x  : current approx.
## $f4  ---  x' : next approx.
## $f8  ---  temp

        .text
        .globl main

main:   

        l.s     $f0,n               # get n
        li.s    $f1,1.0             # constant 1.0
        li.s    $f2,2.0             # constant 2.0
        li.s    $f3,1.0             # x == first approx.
        li.s    $f10,1.0e-5         # five figure accuracy

loop:   
        mov.s   $f4,$f0             # x' = n
        div.s   $f4,$f4,$f3         # x' = n/x
        add.s   $f4,$f3,$f4         # x' = x + n/x
        div.s   $f3,$f4,$f2         # x    = (1/2)(x + n/x)

        mul.s   $f8,$f3,$f3         # x^2
        div.s   $f8,$f0,$f8         # n/x^2
        sub.s   $f8,$f8,$f1         # n/x^2 - 1.0
        abs.s   $f8,$f8             # |n/x^2 - 1.0|
        c.lt.s  $f8,$f10            # |x^2 - n| < small ?
        bc1t    done                # yes: done
  
        j       loop                # next approximation
        
done:
        mov.s   $f12,$f3            # print the result
        li      $v0,2
        syscall
                
        jr      $ra                 # return to OS

##
##  Data Segment  
##
        .data
n:      .float  256.0