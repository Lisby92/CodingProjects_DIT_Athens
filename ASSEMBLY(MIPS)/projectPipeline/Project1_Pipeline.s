#Project1 Pipeline
#Βασίλειος Μαυρομμάτης
#1115201200106

		.data
arrayA:	.word 0x3FD999999999999A,0xBFD999999999999A,0x3FE0000000000000,0xBFE0000000000000,0xC128011D5999999A # +0.4 , - 0.4, + 0.5, -0.5, -786574.675
		.word 0x3FF0000000000000,0xBFF0000000000000,0xC02370A3D70A3D71,0x4022FFFFFFFFEA03,0x4029000000000006 # +1.0, -1.0, -9.72, 9.49999999999, 12.50000000000001
		.word 0x4004000000000000,0x3FECCCCCCCCCCCCD,0xBFECCCCCCCCCCCCD,0x4050000000000000,0xC0B0000000000000 # +2.5, 0.9, -0.9, 64, -4096
		.word 0x7FFFFFFFFFFFFFFF,0x7FFF0F0F0F0F0F0F,0xFFF02A72BC58BD9A,0x800CF61018721B2A,0x0002F9EED8B228F5 # +NaN, +NaN, -NaN, -subnormal, +subnormal
		.word 0x0000000000000000,0x8000000000000000,0x7FF0000000000000,0xFFF0000000000000,0xFFFFFFFFFFFFFFFF # +0, -0, +inf, -inf , -NaN
		.word 0x3FFF44E618CE2D1F,0xBFFF44E618CE2D1F,0xBBB7E494E16DFA85,0xA35FB36CFEEB3997,0x321047881BA39884 # 1.954321, -1.954321,  rest are random from random.org
		.word 0xB68C19A45EB0DE67,0xE06B163154907782,0xE573E078BB9B5112,0xA87A0584D9E3ACE8,0x9E574DFF46EE4A94
		.word 0xBBB394DCCC9DF3BF,0xDC235E3B4BF4A5D5,0xB94E2E068263CE76,0x2605B679554B412E,0xE1E00B471BF11A6A
		.word 0x957C161B3A2A5984,0xA5E5CBFDE2161028,0xE84205E29B6263FA,0xF9F9C975FC7878D8,0x456241B83416F15A
		.word 0x508F866C06ECE749,0x53A4513CE2B64D63,0x8785A24F9AF1D33D,0xD421145FDD1C941A,0xE60337D6B3C497C9
		.word 0x7F7B09F271B1D94B,0x5DDEA28BE73FBD80,0x0847FB6F907A1CE2,0xC68B8840C53061A4,0x835E6CC87374B94D
		.word 0x8186728C8CF42D58,0x2C2E67554693C975,0x4497CA812CD8D2E5,0xDFD0CDD86DA100E3,0x3C3AB0BE0E5519DD
		.word 0x6BA87CA7173147B0,0x0BAD2B1B25926B57,0x7487F3E6D9D52293,0xE39821DE66DC7530,0xE097C416955F6877
		.word 0x469A24B834D95767,0x0CF30E236A87B2DA,0x4484CAE5063B7866,0xC43AB297631F2318,0xB5F6A0FFEFC4A47B
		.word 0x10748482FCC00BE4,0xF8A9FC2F02DB2ADA,0xEAA1404F57C9149B,0x3E21095E8E5D62BC,0x79DFF895CCA4118B
		.word 0xFD9F1ED8F4DE3D83,0x6EC9CEA3D6A7E1C2,0x651DA2E5D93CF771,0xD8906A65B8F58630,0xDEF167136392B0E8
		.word 0x7535DE68CFAA5B3D,0x2829B2E9211B70F4,0x8AF6F27B36285D37,0x80CD49465D72F104,0x569C8E73879B7A45
		.word 0x183A47729FE9E782,0xF043347261748ECD,0x2561A9BA39ADA470,0x157A5FEEAF11BB7E,0x7227F0974F42B96A
		.word 0x87D7A3EF97AEF44A,0x37EDC7914C65FC8A,0x702C12D6BD315AA9,0xE4F90281B0525ACF,0x9366686E2B5AB663
		.word 0xBFDBFE7EC15238D1,0xE7B03B138F0C6F4D,0x8944E185FD8B58FB,0xC6E818C8577F2011,0xEAAD47919A13770B
		.word 0x38C5FD935970B180,0xF93179CE83E31578,0xBA67FB7392573CF8,0x9BF817E1DF50C91A,0x908F2F3AEDCFBFAF
		.word 0xFFD897ECB07A4046,0x6A21897BCBD37518,0xC749C23F77DA8EBF,0x0096E128656E263E,0xB6ACE8AF5048BF7A
		.word 0x537FAAA92B7D500E,0xC3A97CC6EAFB07CF,0x469BF5F009A33C77,0x0586C061D9C50230,0x974301EFDC9EDEF3
		.word 0x3834932DC6AFF10E,0xE52DD960D69686EA,0xBC5CF848A897722B,0xD47EBF6DE0832ABF,0x18EE4BB3201B778E
		.word 0x36FDDEEBFE6381D1,0x067764EC2A69C89E,0x81875F6908D8274D,0xAA2179AB3CD13B1B,0x57F4D7325D865AA3
		.word 0x36ABF854240CFAA3,0xC8A95A2F8788FA4C,0x6BD452D9BE0DC0F1,0x5D79AA22EAA83878,0x96030E6F3DC3972B
		.word 0xA054C696477BFE4D,0x2168AC3FE3BA3781,0x192E47F13FEEAF3D,0x7C87E458D0415062,0x11F38AFE98D90C38
		.word 0x8B60AA634BD2FC51,0x3E0FA4BD6E187C5E,0xB74C61CFA85DA9D2,0xBF97A711AF216CA0,0x32036D9EE567A824
		.word 0x63F76D0FD0096D87,0xAF6AD3A069AE0327,0x5008C1992E1932E5,0xC045C9C7C5C68C43,0x3C0A568EEC9A40B2
		.word 0x702489830B2C26C8,0xB748985E4DA674DD,0x8B0EFF3D716BF5B9,0x4375864095DA8F8A,0xA5B0C8982FC893AC
# arrayB = my_convert , arrayC = MIPS convert - my_convert
arrayB: .space 1200
arrayC:	.space 1200
# data to be stored after converting all decimals
P:		.word 0
N: 		.word 0
D:		.word 0
T:		.word 0
Z:		.word 0
I:		.word 0
# strings to be printed
pS: 	.asciiz "P = "
nS: 	.asciiz "N = "
dS: 	.asciiz "D = "
tS: 	.asciiz "T = "
zS: 	.asciiz "Z = "
iS: 	.asciiz "I = "
#c_sp: 	.asciiz ", "
# the 2 doubles we need to check D zone and convert correctly
half:	.double 0.5
nhalf: 	.double -0.5
# I/O registers
CONTROL: .word32 0x10000
DATA:    .word32 0x10008
		# end of data
		.text
		# initialising registers
		lwu r28, DATA(r0)	 # r28 = address of DATA register
		lwu r29, CONTROL(r0) # r29 = address of CONTROL register
		daddi r27, r0, 1 	 # to be converted to 1.0
		daddi r26, r0, -1 	 # to be converted to -1.0
		daddi r20, r0, 0     # r20 = index to arrayA for integer
		daddi r12, r0, 0x7FFF
		mtc1 r27, f25
		daddi r13, r0, 0x8000
		dsll r12, r12, 16
		daddi r5, r0, 0x1000
 		daddi r9, r0, 0 	 # r9 = index to arrayA for double
 		ori r12, r12, 0xFFFF
 		dsll r13, r13, 28
 		cvt.d.l f20, f25 	 # f20 = +1.0
 		dsll r12, r12, 16
		daddi r21, r0, 150   # r21 = main loop counter
		dsll r13, r13, 20 	 # r13 = minInt64
		ori r12, r12, 0xFFFF
		dsll r5, r5, 28
		mtc1 r26, f25
		daddi r10, r0, -8 	 # r10 = index to arrayC
		dsll r12, r12, 16
		cvt.d.l f21, f25 	 # f21 = -1.0
		dsll r5, r5, 12 	 # r5 = 0x10000000000000 to be added to mantissa
		ori r12, r12, 0xFFFF # r12 = maxInt64
		daddi r3, r0, 0xFFFFFFFF # r3 = sign changer used as 2nd operand of xor (2's complement) 
		l.d f5, half(r0) 	 # f5 = +0.5
		l.d f6, nhalf(r0) 	 # f6 = -0.5
		daddi r27, r0, 0 	 # r27 = P
		mtc1 r13, f13 		 # f13 = minInt64 as double based on winmips (trims last integer digit to 0)
		daddi r19, r0, 0 	 # r19 = index to arrayB
		daddi r26, r0, 0 	 # r26 = N
		cvt.d.l f14, f13 	 # f14 = the double version of minInt64 to be held
		mtc1 r12, f12   	 # f12 = maxInt64 as double based on winmips (trims last integer digit to 0)
		daddi r25, r0, 0	 # r25 = D
		daddi r24, r0, 0 	 # r24 = T
		cvt.d.l f15, f12 	 # f15 = the double version of maxInt64 to be held
		daddi r23, r0, 0 	 # r23 = Z
		daddi r22, r0, 0 	 # r22 = I
		daddi r15, r0, 0x00000000000007FF # r15 = special exponent
		# main loop starts here
loop:	l.d f4, arrayA(r20)  # f4 = 64bits as double
		ld r4, arrayA(r9) 	 # r4 = 64bits as integer
		daddi r20, r20, 8    # increment index to arrayA
		cvt.l.d f11, f4 	 # convert the double and temp store in f11
		dsll r17, r4, 1      # cut 1 bit left, r17 = exponent (bits 52~62)
		dsrl r18, r4, 31     # cut 31 bits right, r18 = sign (bit 63)				
		mfc1 r11, f11 		 # r11 = converted integer via cvt.l.d of winmips
		dsrl r17, r17, 31 	 # cut 31 bits right
		dsll r16, r4, 12     # cut 12 bits left, r16 = mantissa (bits 0~51)
		dsrl r18, r18, 31 	 # cut 31 bits right
		dsrl r17, r17, 22 	 # cut 22 bits right
		dsrl r16, r16, 12 	 # cut 12 bits right
		dsrl r18, r18, 1 	 # cut 1 bit right
		# some special case checks
e000:	beqz r17, spcl1		 # exponent = 000(16)? -> spcl1
e7FF:	beq r17, r15, spcl2  # exponent = 7FF(16)? -> spcl2
p05: 	c.le.d f5, f4 		 # 0.5 <= double?
		bc1t lmts1			 # yes -> lmts1
n05:	c.le.d f4, f6 		 # double <= -0.5?
		bc1t lmts1 			 # yes -> lmts1
		dsub r7, r0, r11 	 # r7 = the difference to be stored
		sd r0, arrayB(r19) 	 # else numbers in(-0.5, 0.5) are turned to zero
		daddi r19, r19, 8 	 # increment index to arrayB
		daddi r10, r10, 8 	 # increment index to arrayC
		daddi r21, r21, -1 	 # decrement loop counter
		daddi r9, r9, 8 	 # increment index to arrayA double
		sd r7, arrayC(r10) 	 # store the difference in arrayC
		daddi r25, r25, 1 	 # increment D
		j dowhl 			 # -> dowhl and continue
		# limit checking (minInt64, maxInt64)
lmts1:	c.le.d f15, f4 		 # maxInt64 <= double?
		bc1f lmts2 			 # no -> lmts2
		dsub r7, r13, r11 	 # r7 = the difference to be stored
		sd r12, arrayB(r19)	 # yes, store maxInt64
		daddi r19, r19, 8 	 # increment index to arrayB
		daddi r10, r10, 8 	 # increment index to arrayC
		daddi r21, r21, -1 	 # decrement loop counter
		daddi r9, r9, 8 	 # increment index to arrayA double
		sd r7, arrayC(r10) 	 # store the difference in arrayC
		daddi r24, r24, 1	 # increment T
		j dowhl 			 # -> dowhl and continue
lmts2:	c.le.d f4, f14 		 # double <= minInt64?
		bc1f pstv 			 # no -> dowhl
		dsub r7, r13, r11 	 # r7 = the difference to be stored
		sd r13, arrayB(r19)  # yes, store minInt64
		daddi r19, r19, 8 	 # increment index to arrayB
		daddi r10, r10, 8 	 # increment index to arrayC
		daddi r21, r21, -1 	 # decrement loop counter
		daddi r9, r9, 8 	 # increment index to arrayA double
		sd r7, arrayC(r10) 	 # store the difference in arrayC
		daddi r24, r24, 1	 # increment T
		j dowhl 			 # -> dowhl and continue
		# sign checking
pstv:	bnez r18, ngtv 		 # double < 0? -> ngtv
		add.d f23, f4, f5 	 # add +0.5 to positive double
		daddi r27, r27, 1 	 # increment P, double is positive
		daddi r17, r17, -1075# c_exponent = exponent - (bias==1023) - (mantissa bits==52)
		j cnvrt				 # -> cnvrt and skip
ngtv:	add.d f23, f4, f6 	 # add -0.5 to positive double
		daddi r17, r17, -1075# c_exponent = exponent - (bias==1023) - (mantissa bits==52)
		daddi r26, r26, 1 	 # increment N, double is negative
		# double is within integer limits
cnvrt:	dadd r16, r16, r5 	 # no, add the 1 to mantissa
		slt r2, r17, r0 	 # c_exponent < 0?
		cvt.l.d f11, f23 	 # convert the incremented double and temp store in f11
		daddi r21, r21, -1 	 # decrement loop counter
		daddi r10, r10, 8 	 # increment index to arrayC
		mfc1 r8, f11 		 # r8 = converted changed integer via cvt.l.d of winmips
		bnez r2, expN 		 # yes -> expN
		slt r2, r0, r17 	 # c_exponent > 0?
		daddi r21, r21, 1 	 # decrement loop counter
		daddi r10, r10, -8 	 # increment index to arrayC
		bnez r2, expP 		 # yes -> expP
		daddi r21, r21, -1 	 # decrement loop counter
		daddi r10, r10, 8 	 # increment index to arrayC
store:	bne r8, r11, chng    # r8 != r11? error conversion -> chng
ret2:	beqz r18, cont 		 # positive? -> cont
		xor r16, r16, r3 	 # 2's complement mantissa
		xor r11, r11, r3  	 # 2's complement for substraction
		daddi r9, r9, 16 	 # increment index to arrayA double
		daddi r16, r16, 1 	 # add 1 to complement
		daddi r11, r11, 1 	 # add 1 to complement
		j cont2 			 # -> cont2 and skip
cont: 	daddi r9, r9, 8 	 # increment index to arrayA double
		dsub r7, r16, r11 	 # r7 = the difference to be stored
		sd r16, arrayB(r19)  # store the calculated integer
		daddi r19, r19, 8 	 # increment index to arrayB
		sd r7, arrayC(r10) 	 # store the difference in arrayC
		j dowhl 			 # -> dowhl and continue
cont2:	daddi r9, r9, -8 	 # decrement index to arrayA double
		dadd r7, r16, r11 	 # r7 = the difference to be stored added here cause r11 changed sign
		sd r16, arrayB(r19)  # store the calculated integer
		daddi r19, r19, 8 	 # increment index to arrayB
		sd r7, arrayC(r10)	 # store the difference in arrayC
dowhl:	bnez r21, loop 		 # loop more? -> loop
 		# algorithm ended write oute the results!
end: 	daddi r4, r0, 4 	 # r4 = ascii output
		daddi r5, r0, pS 	 # r5 = pS string address
		daddi r3, r0, 1 	 # r3 = unsigned output
		sd r27, P(r0)		 # store P in memory
		sd r5, 0(r28) 		 # write address of string to DATA register
		sd r4, 0(r29) 		 # print the string
		daddi r5, r0, nS 	 # r5 = nS string address
		sd r27, 0(r28) 		 # write P to DATA register
		sd r3, 0(r29) 		 # print the unsigned integer
		sd r26, N(r0)		 # store N in memory
		sd r5, 0(r28) 		 # write address of string to DATA register
		sd r4, 0(r29) 		 # print the string
		daddi r5, r0, dS 	 # r5 = dS string address
 		sd r26, 0(r28) 		 # write N to DATA register
		sd r3, 0(r29) 		 # print the unsigned integer
		sd r25, D(r0)		 # store D in memory
		sd r5, 0(r28) 		 # write address of string to DATA register
		sd r4, 0(r29) 		 # print the string
		daddi r5, r0, tS 	 # r5 = tS string address
		sd r25, 0(r28) 		 # write D to DATA register
		sd r3, 0(r29) 		 # print the unsigned integer
		sd r24, T(r0)		 # store T in memory
		sd r5, 0(r28) 		 # write address of string to DATA register
		sd r4, 0(r29) 		 # print the string
		daddi r5, r0, zS     # r5 = zS string address
		sd r24, 0(r28) 		 # write T to DATA register
		sd r3, 0(r29) 		 # print the unsigned integer
		sd r23, Z(r0)		 # store Z in memory
		sd r5, 0(r28) 		 # write address of string to DATA register
		sd r4, 0(r29) 		 # print the string
		daddi r5, r0, iS 	 # r5 = iS string address
		sd r23, 0(r28) 		 # write Z to DATA register
		sd r3, 0(r29) 		 # print the unsigned integer
		sd r22, I(r0)		 # store I in memory
		sd r5, 0(r28) 		 # write address of string to DATA register
		sd r4, 0(r29) 		 # print the string
		sd r22, 0(r28) 		 # write I to DATA register
		sd r3, 0(r29) 		 # print the unsigned integer
		# end of program
		halt
		################			 
 		# double is signed zero or subnormal
spcl1:  bnez r16, sbnrm 	 # mantissa != 0? -> sbnrm
		dsub r7, r11, r0 	 # r7 = the difference to be stored
		daddi r10, r10, 8 	 # increment index to arrayC
		daddi r23, r23, 1 	 # else is signed zero, increment Z
		daddi r21, r21, -1 	 # decrement loop counter
		sd r7, arrayC(r10) 	 # store the difference in arrayC
		daddi r9, r9, 8 	 # increment index to arrayA double
		sd r0, arrayB(r19)   # store the 0 in arrayB
		daddi r19, r19, 8 	 # increment index to arrayB
		j dowhl 			 # -> dowhl and continue
sbnrm:	dsub r7, r11, r0 	 # r7 = the difference to be stored
		daddi r10, r10, 8 	 # increment index to arrayC
		daddi r25, r25, 1	 # is subnormal value, increment D
		daddi r21, r21, -1 	 # decrement loop counter
		sd r7, arrayC(r10) 	 # store the difference in arrayC
		daddi r9, r9, 8 	 # increment index to arrayA double
		sd r0, arrayB(r19)	 # all subnormal values are way less than abs(0.5) but bigger than 0 so store 0
		daddi r19, r19, 8 	 # increment index to arrayB
		j dowhl 			 # -> dowhl and continue
		# double is +inf -inf or NaN
spcl2:  bnez r18, ninf		 # sign == 1? -> ninf
pinf:	sd r12, arrayB(r19)	 # store the +inf in arrayB
		daddi r10, r10, 8 	 # increment index to arrayC
		dsub r7, r11, r13 	 # r7 = the difference to be stored
		daddi r19, r19, 8 	 # increment index to arrayB
		daddi r21, r21, -1 	 # decrement loop counter
		sd r7, arrayC(r10) 	 # store the difference in arrayC
		daddi r9, r9, 8 	 # increment index to arrayA double
		daddi r22, r22, 1	 # increment I
		j dowhl 			 # -> dowhl and continue
ninf:	sd r13, arrayB(r19)  # store the -inf in arrayB
		daddi r10, r10, 8 	 # increment index to arrayC
		dsub r7, r13, r11 	 # r7 = the difference to be stored
		daddi r19, r19, 8 	 # increment index to arrayB
		daddi r21, r21, -1 	 # decrement loop counter
		sd r7, arrayC(r10) 	 # store the difference in arrayC
		daddi r9, r9, 8 	 # increment index to arrayA double
		daddi r22, r22, 1 	 # increment I
		j dowhl				 # -> dowhl and continue
 		# calculate the integer
expN: 	beqz r17, store 	 # c_exponent = 0-? store
		daddi r17, r17, 1 	 # decrement c_exponent
		dsrl r16, r16, 1 	 # divide mantissa with 2
		j expN 				 # -> expN and continue creating the integer
expP:	daddi r21, r21, -1 	 # decrement loop counter
		daddi r10, r10, 8 	 # increment index to arrayC
expPc:	beqz r17, store 	 # c_exponent reached zero -> store
		daddi r17, r17, -1 	 # increment c_exponent
		dsll r16, r16, 1 	 # multiply mantissa with 2
		j expPc	 			 # -> expPc and continue creating the integer
 		# fix the error of convertion
chng: 	bnez r18, chngn 	 # double < 0? -> chngn
		daddi r16, r16, 1 	 # positive : add 1 to round
		j cont 				 # -> cont
chngn:  daddi r16, r16, 1 	 # negative : add 1 to round
	    j ret2 				 # -> ret2
	    # end of text