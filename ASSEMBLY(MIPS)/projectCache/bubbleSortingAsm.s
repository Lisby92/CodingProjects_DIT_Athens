.text
.globl  main

main:   li $s0, 1000 # $s0 = size
        li $s1, 1    # $s1 = 1 increment
        li $s2, -1   # $s2 = 1 decrement
        li $t0, 1    # $t0 = i == outer loop iterator
        li $t1, 999  # $t1 = j == inner loop iterator and arrayS index
        li $t2, 0    # $t2 = j - 1 arrayS index
        li $t3, 0    # $t3 = reorder , 0 == sorting finished / 1 == sorting continues
        li $t4, 0    # $t4 = temp for swapping
        li $t5, 0    # $t5 = arrayS[j]
        li $t6, 0    # $t6 = arrayS[j-1]

outer:  beq $t0, $s0, exit        # while i < size , no -> exit
          li $t3, 0               # yes -> reorder = 0
          li $t1, 999             # j = size - 1
inner:    blt $t1, $t0, reord     # while j >= i, -> no -> reord
loadD:      add $t2, $t1, $s2     # yes -> calculate j-1
            lb $t5, arrayS($t1)   # load byte from arrayS[j]
            lb $t6, arrayS($t2)   # load byte from arrayS[j-1]
check:      blt $t5, $t6, swap    # if arrayS[j] < arrayS[j-1], yes -> swap 
cont1:      add $t1, $t1, $s2     # j--
          j inner
          nop 
cont2:  add $t0, $t0, $s1         # i++
        j outer                   # -> outer
        nop        

exit:   li $v0, 10                # load exit call
        syscall                   # execute it

reord:  beqz $t3, exit      # if reorder == 0, yes -> exit
        j cont2             # no -> cont2
        nop

swap:   move $t4, $t5       # temp = arrayS[j]
        move $t5, $t6       # arrayS[j] = arrayS[j-1]
        move $t6, $t4       # arrayS[j-1] = temp
        li $t3, 1           # reorder = 1
storD:  sb $t5, arrayS($t1) # store new arrayS[j]
        sb $t6, arrayS($t2) # store new arrayS[j-1]
        j cont1             # -> cont1
        nop             

.data
# 1000 byte size integers from random.org range -128 ~ 127
arrayS:
.byte 67, 38, 28, 92, 38, -2, -59, -118, 28, 19, -82, -22, 104, 106, 17, -114, -24, 81, 75, -109
.byte 44, -123, 11, 94, 75, -125, -119, 10, 18, 115, -95, 57, 55, 95, -72, -89, -83, -55, -24, 15
.byte 126, -8, -87, 77, -31, 42, -85, -93, -52, 22, -83, 127, 96, 32, -46, 73, -117, 71, -123, -39
.byte -30, 29, -92, 115, 39, 29, 111, 43, -109, -5, -59, -81, -55, -39, 88, 66, 95, -3, -30, 12
.byte 112, -128, 32, 63, 38, -27, -53, -48, -105, -58, -63, -50, 36, -13, 79, -11, 3, -108, 43, -57
.byte 126, 67, -121, -10, 119, 2, -120, 28, 15, -54, 94, 3, -105, 10, -124, 5, -76, -63, 41, 68
.byte 62, 7, -125, 64, 25, -41, -3, -124, -61, -105, -36, -25, -59, -115, 106, 70, -6, -68, 13, -65
.byte -20, -112, 115, 76, -18, -2, -1, -72, -107, 9, 87, 34, -22, -117, -39, 105, -89, 36, 55, -120
.byte -36, 43, 12, 115, -30, 23, 43, -4, 94, 98, -5, -65, 68, 26, -80, 57, 98, 68, -38, 42
.byte -54, 27, 125, 80, -16, -57, -11, 37, 126, 105, 83, -86, 62, -100, -20, 106, -50, 77, -114, -116
.byte 93, -80, 80, 86, 70, 83, -105, 114, -67, 108, 39, 56, 48, 107, -64, -40, -18, 71, -36, -107
.byte -55, -1, -31, -126, 48, -25, -40, 65, -98, -29, 59, 101, -106, -76, -84, -72, 9, 62, -104, 12
.byte 118, -126, -96, 62, -52, -99, 122, -73, 11, -110, -85, 89, -64, 25, 45, -46, -64, -82, -57, 18
.byte -110, 46, 25, -96, 19, 90, 84, -3, -102, -118, -94, -66, 15, 33, -34, -86, 64, -38, -56, -104
.byte -31, 50, -20, 6, -88, -69, 20, -90, -25, 74, -99, 119, -117, -79, -95, 21, -122, 77, -71, 13
.byte 32, 63, -107, 67, 12, 91, -2, 109, 72, -92, -126, 26, 20, -65, 72, 95, -33, -18, -116, 106
.byte 12, -56, -14, -113, -97, 124, 61, 28, -103, -106, -62, 115, 46, -104, 122, 123, -118, -115, -16, 96
.byte 81, -82, -31, -67, 22, 64, 21, -109, -88, 124, -60, -111, 65, 120, -21, -50, -128, 85, 90, 116
.byte -94, 35, -45, 34, 101, -121, 4, -5, 112, -52, -9, 52, -104, 98, 73, 96, -120, 3, -93, -9
.byte -7, 119, -5, 80, 3, 78, 54, -71, -44, 92, -79, 100, 110, -96, -83, -64, -17, 65, -113, 116
.byte 72, 47, -3, -102, -51, -124, -67, 75, 18, 57, -65, 13, 18, 94, -12, -28, -30, -95, -57, -94
.byte -62, -32, -84, 14, 13, 61, -89, -46, -96, -106, -109, 86, -13, 122, 14, 34, 107, -94, 14, -35
.byte 26, 44, -105, -93, 70, -61, 56, -100, -1, -51, 56, 66, 13, -51, -61, 93, 45, -97, 105, 95
.byte 67, -113, -28, -65, -94, 77, 65, 70, 108, 96, -64, -86, 6, -83, -23, -43, 108, -48, 73, 74
.byte 83, -11, -46, 77, -73, -109, 58, 9, -104, -81, -73, -93, -13, 122, 41, 112, -80, -58, -113, -116
.byte 10, 110, 1, 110, -22, -115, 59, 60, -127, -85, 40, -80, 59, -12, 32, -10, -126, -72, 31, 45
.byte -3, 26, -127, -105, -12, -121, 21, 78, -38, -92, -30, 60, 119, -84, 97, -96, -97, -25, 79, -32
.byte -115, 82, 112, -113, -17, -36, -27, 1, -109, -119, 84, -38, -92, 11, 46, -104, -98, 123, 42, -104
.byte -14, 28, 26, -122, 37, 71, 119, -82, -4, -46, 49, 86, -101, 53, 126, 37, 123, -10, -49, 0
.byte -96, 47, 114, -4, 13, 102, 77, 76, -40, 47, -121, -20, 108, 98, 64, 30, 0, 71, -72, 73
.byte -39, -114, 72, 67, 0, -69, 36, -52, -18, -16, -82, -40, -76, -57, 0, 16, 27, -102, -16, 55
.byte 81, -87, 77, 51, -98, -63, 35, 3, 120, 44, 115, -12, -85, -89, 9, -108, -15, -40, 34, 21
.byte -57, 111, 68, 104, 13, 98, 18, 23, 46, 111, 109, 107, -91, -56, 47, 51, -90, 59, 124, -81
.byte 22, 33, 30, 5, -79, -9, -93, 38, 92, -52, -28, -107, -70, -71, 94, 95, 3, -24, -62, -19
.byte -36, 88, 84, -42, 84, 5, -109, -107, -33, 66, 83, 33, -11, 103, 78, 54, 59, -88, -128, -45
.byte -87, -6, 125, 50, -57, 68, -103, 64, -79, -42, 42, -54, 87, -73, -24, -128, 37, -85, 104, 111
.byte 97, -81, -23, 6, -10, 77, 33, -42, 63, -59, 73, 38, -91, -113, -112, -81, -83, -24, 103, -81
.byte -40, 101, -100, -39, -28, -70, 106, 28, 121, -76, -98, -72, 55, -104, 88, 111, -41, 71, 71, -57
.byte 73, 95, -67, 38, 103, 97, 69, 63, 3, 81, -87, -73, -67, 100, 91, -80, -70, -2, -65, -46
.byte -116, 69, -81, 114, 8, 103, 75, -93, 91, 39, -2, -95, -90, -35, -2, 31, -101, 62, 18, 20
.byte -110, 19, -69, -50, -104, 13, -53, -110, -88, -77, -117, -65, -85, -17, -3, 3, 16, 114, -79, 39
.byte 51, -85, -62, -30, 125, -10, 31, -127, -8, -2, 115, -123, 16, 125, -113, 47, -45, 20, -15, 83
.byte -71, 59, 23, 24, 75, 33, 10, 101, -127, 99, -5, 0, 118, -52, 119, 37, -92, -112, 87, -126
.byte -80, 103, -55, 122, 97, -71, 4, 13, 110, 62, -98, -41, -88, 36, -97, 105, 54, -14, -95, -62
.byte 68, -125, 126, 91, 92, -111, -113, -46, -122, -95, 12, -61, 89, -70, -78, 41, -10, -117, -103, -106
.byte 87, -123, -12, -112, 66, -25, -112, -100, 116, -84, -79, 74, 126, 102, -125, -28, 66, -15, -125, 16
.byte 116, -54, 92, 0, -91, -124, -114, 95, -103, -38, -124, 55, -81, 95, 49, -77, 18, -46, 25, -65
.byte -77, 69, 59, -47, 110, -41, 105, 76, -37, -90, -14, -27, -91, 48, 50, -52, -95, 98, 127, 72
.byte 28, 122, 71, 21, 125, -112, -71, 38, -80, 100, 70, -120, -76, -86, -124, -68, -8, 115, -40, -98
.byte -64, 14, 10, 81, -51, 31, -14, 21, 127, -123, -57, -103, 56, -92, 12, -59, -98, -18, -39, -21

# C code from Book {Γ.Σ. Τσελίκης, Ν.Δ. Τσέλικας, "C: Από τη Θεωρία στην Εφαρμογή" , page 407}

#  #define SIZE 1000
#  void bubble_sort(int arr[])
#  {
#    int i, j, temp, reorder;
#
#    for(i = 1; i < SIZE; i++)
#    {
#      reorder = 0;
#      for(j = SIZE-1; j >= i; j--)
#      {
#        if(arr[j] < arr[j-1])
#        {
#          temp = arr[j];
#          arr[j] = arr[j-1];
#          arr[j-1] = temp;
#          reorder = 1;
#        }
#      }
#      if(reorder == 0)
#        return;
#    }
#  }
