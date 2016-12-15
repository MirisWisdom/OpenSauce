
static __declspec(naked) void EraProcessAndXorBlockEncypt()
{
#define var_A0 -0xA0
#define arg_50 0x50
#define arg_58 0x58

	__asm {
		mfspr	r12, LR
		bl	    _savegprlr_14
		mr	    r20, r3
		ld	    r11, arg_50(sp)
		mr	    r26, r4
		ld	    r25, arg_58(sp)
		mr	    r18, r6
		srdi	r27, r20, 32
		srdi	r28, r26, 32
		lis	    r6,	-0x61C9	//; 0x9E3779B9
		mr	    r19, r5
		ori	    r5,	r6, 0x79B9 //; 0x9E3779B9
		mr	    r15, r11
		srwi	r3,	r27, 5
		srwi	r31, r28, 5
		srdi	r4,	r19, 32
		xor	    r30, r31, r5
		xor	    r3,	r3, r5
		xor	    r6,	r27, r15
		xor	    r31, r28, r15
		srwi	r29, r4, 5
		add	    r6,	r3, r6
		xor	    r24, r29, r5
		add	    r3,	r30, r31
		slwi	r22, r28, 4
		xor	    r29, r4, r15
		slwi	r23, r27, 4
		add	    r21, r3, r22
		add	    r30, r24, r29
		slwi	r31, r4, 4
		clrlwi  r22, r26, 0
		add	    r23, r6, r23
		srdi	r26, r18, 32
		srdi	r6,	r11, 32
		add	    r11, r21, r22
		add	    r30, r30, r31
		clrlwi  r29, r19, 0
		srwi	r22, r26, 5
		clrlwi  r24, r20, 0
		add	    r31, r11, r6
		add	    r30, r30, r29
		xor	    r11, r22, r5
		xor	    r29, r26, r15
		add	    r3,	r23, r24
		add	    r11, r11, r29
		add	    r3,	r3, r6
		slwi	r29, r26, 4
		add	    r30, r30, r6
		add	    r22, r11, r29
		mr	    r14, r25
		srwi	r24, r3, 5
		clrlwi  r21, r18, 0
		srwi	r11, r31, 5
		srwi	r20, r30, 5
		xor	    r24, r24, r5
		xor	    r23, r3, r14
		add	    r19, r22, r21
		xor	    r11, r11, r5
		xor	    r29, r31, r14
		xor	    r22, r20, r5
		xor	    r21, r30, r14
		add	    r23, r24, r23
		add	    r11, r11, r29
		slwi	r20, r3, 4
		slwi	r24, r31, 4
		add	    r22, r22, r21
		slwi	r21, r30, 4
		add	    r23, r23, r20
		add	    r29, r19, r6
		add	    r24, r11, r24
		srdi	r11, r25, 32
		add	    r25, r22, r21
		mr	    r22, r5
		add	    r27, r23, r27
		srwi	r20, r29, 5
		add	    r5,	r24, r28
		add	    r21, r25, r4
		xor	    r25, r20, r22
		add	    r28, r27, r11
		xor	    r23, r29, r14
		lis	    r4,	0x3C6E //; 0x3C6EF372
		lis	    r22, -0x255A //; 0xDAA66D2B
		add	    r27, r5, r11
		add	    r25, r25, r23
		xor	    r23, r27, r15
		ori	    r5,	r22, 0x6D2B //; 0xDAA66D2B
		stw	    r23, var_A0(sp)
		slwi	r24, r29, 4
		ori	    r4,	r4, 0xF372 //; 0x3C6EF372
		srwi	r22, r28, 5
		add	    r25, r25, r24
		xor	    r24, r22, r4
		xor	    r22, r28, r15
		add	    r25, r25, r26
		add	    r24, r24, r22
		slwi	r23, r28, 4
		add	    r25, r25, r11
		add	    r26, r21, r11
		add	    r24, r24, r23
		srwi	r22, r25, 5
		srwi	r23, r26, 5
		add	    r3,	r24, r3
		xor	    r24, r22, r4
		xor	    r23, r23, r4
		xor	    r17, r26, r15
		xor	    r18, r25, r15
		srwi	r22, r27, 5
		add	    r24, r24, r18
		lwz	    r19, var_A0(sp)
		xor	    r22, r22, r4
		add	    r23, r23, r17
		slwi	r20, r26, 4
		slwi	r21, r25, 4
		add	    r3,	r3, r6
		add	    r24, r24, r21
		add	    r23, r23, r20
		add	    r22, r22, r19
		slwi	r19, r27, 4
		srwi	r21, r3, 5
		add	    r29, r24, r29
		add	    r30, r23, r30
		xor	    r24, r21, r4
		add	    r22, r22, r19
		xor	    r23, r3, r14
		add	    r31, r22, r31
		add	    r24, r24, r23
		slwi	r23, r3, 4
		add	    r30, r30, r6
		add	    r29, r29, r6
		add	    r31, r31, r6
		add	    r24, r24, r23
		srwi	r22, r30, 5
		srwi	r19, r29, 5
		srwi	r20, r31, 5
		add	    r28, r24, r28
		xor	    r23, r22, r4
		xor	    r24, r19, r4
		xor	    r4,	r20, r4
		xor	    r21, r30, r14
		xor	    r22, r29, r14
		xor	    r20, r31, r14
		add	    r23, r23, r21
		add	    r24, r24, r22
		add	    r4,	r4, r20
		slwi	r21, r30, 4
		slwi	r22, r29, 4
		slwi	r20, r31, 4
		add	    r28, r28, r11
		add	    r23, r23, r21
		add	    r24, r24, r22
		add	    r4,	r4, r20
		srwi	r22, r28, 5
		add	    r21, r23, r26
		add	    r23, r24, r25
		add	    r4,	r4, r27
		xor	    r24, r28, r15
		xor	    r26, r22, r5
		add	    r27, r4, r11
		add	    r26, r26, r24
		slwi	r25, r28, 4
		lis	    r4,	0x78DD //; 0x78DDE6E4
		add	    r26, r26, r25
		ori	    r4,	r4, 0xE6E4 //; 0x78DDE6E4
		srwi	r24, r27, 5
		add	    r25, r26, r3
		add	    r3,	r21, r11
		xor	    r24, r24, r5
		xor	    r21, r3, r15
		xor	    r22, r27, r15
		stw	    r21, var_A0(sp)
		add	    r26, r23, r11
		add	    r24, r24, r22
		slwi	r23, r27, 4
		add	    r25, r25, r6
		add	    r24, r24, r23
		srwi	r22, r25, 5
		add	    r31, r24, r31
		srwi	r21, r3, 5
		xor	    r24, r22, r5
		xor	    r18, r25, r14
		srwi	r23, r26, 5
		xor	    r22, r21, r5
		add	    r24, r24, r18
		slwi	r21, r25, 4
		xor	    r23, r23, r5
		xor	    r17, r26, r15
		add	    r24, r24, r21
		add	    r23, r23, r17
		slwi	r20, r26, 4
		add	    r31, r31, r6
		lwz	    r21, var_A0(sp)
		add	    r23, r23, r20
		slwi	r19, r3, 4
		add	    r22, r22, r21
		srwi	r20, r31, 5
		add	    r28, r24, r28
		add	    r29, r23, r29
		add	    r22, r22, r19
		xor	    r24, r20, r5
		xor	    r23, r31, r14
		add	    r30, r22, r30
		add	    r24, r24, r23
		slwi	r23, r31, 4
		add	    r30, r30, r6
		add	    r29, r29, r6
		add	    r24, r24, r23
		add	    r28, r28, r11
		srwi	r23, r30, 5
		srwi	r22, r29, 5
		srwi	r19, r28, 5
		xor	    r23, r23, r5
		add	    r27, r24, r27
		xor	    r20, r30, r14
		xor	    r24, r22, r5
		xor	    r21, r29, r14
		xor	    r22, r28, r15
		xor	    r5,	r19, r4
		add	    r23, r23, r20
		add	    r24, r24, r21
		slwi	r20, r30, 4
		add	    r5,	r5, r22
		slwi	r21, r29, 4
		add	    r27, r27, r11
		slwi	r22, r28, 4
		add	    r23, r23, r20
		add	    r24, r24, r21
		add	    r5,	r5, r22
		srwi	r21, r27, 5
		add	    r22, r23, r3
		add	    r23, r24, r26
		xor	    r3,	r21, r4
		xor	    r26, r27, r15
		add	    r24, r5, r25
		add	    r26, r3, r26
		slwi	r25, r27, 4
		add	    r3,	r22, r11
		add	    r26, r26, r25
		lis	    r5,	0x1715 //; 0x1715609D
		add	    r25, r26, r31
		srwi	r21, r3, 5
		add	    r31, r23, r11
		add	    r26, r24, r6
		ori	    r5,	r5, 0x609D //; 0x1715609D
		add	    r25, r25, r6
		xor	    r22, r3, r15
		xor	    r24, r21, r4
		slwi	r23, r3, 4
		add	    r24, r24, r22
		srwi	r22, r26, 5
		add	    r24, r24, r23
		srwi	r21, r31, 5
		stw	    r24, var_A0(sp)
		srwi	r24, r25, 5
		xor	    r18, r25, r14
		xor	    r24, r24, r4
		xor	    r23, r22, r4
		xor	    r22, r21, r4
		add	    r24, r24, r18
		slwi	r21, r25, 4
		xor	    r17, r26, r14
		add	    r24, r24, r21
		xor	    r16, r31, r15
		add	    r27, r24, r27
		add	    r23, r23, r17
		slwi	r20, r26, 4
		add	    r22, r22, r16
		slwi	r19, r31, 4
		add	    r23, r23, r20
		add	    r22, r22, r19
		add	    r28, r23, r28
		add	    r29, r22, r29
		add	    r28, r28, r11
		lwz	    r24, var_A0(sp)
		add	    r29, r29, r6
		add	    r27, r27, r11
		add	    r30, r24, r30
		srwi	r20, r28, 5
		add	    r30, r30, r6
		srwi	r18, r29, 5
		srwi	r24, r30, 5
		xor	    r22, r30, r14
		xor	    r24, r24, r4
		slwi	r23, r30, 4
		add	    r24, r24, r22
		srwi	r17, r27, 5
		add	    r19, r24, r23
		xor	    r24, r20, r5
		xor	    r21, r28, r15
		xor	    r23, r18, r4
		xor	    r20, r29, r14
		xor	    r22, r27, r15
		xor	    r4,	r17, r5
		add	    r3,	r19, r3
		add	    r24, r24, r21
		slwi	r21, r28, 4
		add	    r4,	r4, r22
		add	    r23, r23, r20
		add	    r3,	r3, r11
		slwi	r22, r27, 4
		slwi	r20, r29, 4
		add	    r24, r24, r21
		srwi	r21, r3, 5
		add	    r4,	r4, r22
		add	    r23, r23, r20
		add	    r22, r24, r26
		xor	    r24, r3, r15
		xor	    r26, r21, r5
		add	    r31, r23, r31
		add	    r23, r4, r25
		add	    r26, r26, r24
		slwi	r25, r3, 4
		add	    r31, r31, r11
		add	    r24, r26, r25
		srwi	r4,	r31, 5
		add	    r30, r24, r30
		add	    r26, r22, r6
		xor	    r24, r4, r5
		xor	    r22, r31, r15
		add	    r25, r23, r6
		lis	    r4,	-0x4AB4	//; 0xB54CDA56
		add	    r24, r24, r22
		add	    r30, r30, r6
		slwi	r23, r31, 4
		ori	    r4,	r4, 0xDA56 //; 0xB54CDA56
		srwi	r22, r26, 5
		srwi	r21, r25, 5
		add	    r24, r24, r23
		xor	    r23, r21, r5
		srwi	r20, r30, 5
		xor	    r21, r25, r14
		add	    r29, r24, r29
		xor	    r24, r20, r5
		add	    r23, r23, r21
		slwi	r20, r25, 4
		xor	    r18, r30, r14
		add	    r23, r23, r20
		add	    r24, r24, r18
		slwi	r21, r30, 4
		xor	    r22, r22, r5
		add	    r29, r29, r6
		xor	    r17, r26, r14
		mr	    r20, r5
		add	    r24, r24, r21
		add	    r5,	r23, r27
		add	    r22, r22, r17
		slwi	r19, r26, 4
		srwi	r23, r29, 5
		add	    r3,	r24, r3
		add	    r27, r5, r11
		add	    r22, r22, r19
		xor	    r5,	r23, r20
		xor	    r24, r29, r14
		add	    r28, r22, r28
		add	    r5,	r5, r24
		slwi	r24, r29, 4
		add	    r3,	r3, r11
		add	    r28, r28, r11
		srwi	r22, r27, 5
		add	    r23, r5, r24
		srwi	r19, r3, 5
		xor	    r5,	r22, r4
		xor	    r21, r27, r15
		srwi	r24, r28, 5
		add	    r31, r23, r31
		xor	    r23, r19, r4
		xor	    r22, r3, r15
		xor	    r24, r24, r4
		add	    r5,	r5, r21
		xor	    r20, r28, r15
		slwi	r21, r27, 4
		add	    r23, r23, r22
		add	    r31, r31, r11
		add	    r24, r24, r20
		slwi	r22, r3, 4
		add	    r5,	r5, r21
		slwi	r20, r28, 4
		srwi	r19, r31, 5
		add	    r21, r23, r22
		add	    r24, r24, r20
		add	    r22, r5, r25
		xor	    r23, r31, r15
		xor	    r25, r19, r4
		add	    r26, r24, r26
		add	    r25, r25, r23
		slwi	r24, r31, 4
		add	    r26, r26, r6
		add	    r24, r25, r24
		srwi	r5,	r26, 5
		add	    r29, r24, r29
		add	    r25, r22, r6
		xor	    r24, r5, r4
		xor	    r22, r26, r14
		add	    r29, r29, r6
		add	    r30, r21, r30
		add	    r24, r24, r22
		srwi	r22, r29, 5
		add	    r30, r30, r6
		lis	    r5,	0x5384 //; 0x5384540F
		stw	    r22, var_A0(sp)
		slwi	r23, r26, 4
		srwi	r21, r25, 5
		mr	    r20, r4
		mr	    r17, r4
		ori	    r5,	r5, 0x540F //; 0x5384540F
		srwi	r16, r30, 5
		add	    r24, r24, r23
		xor	    r22, r25, r14
		xor	    r4,	r21, r4
		lwz	    r21, var_A0(sp)
		add	    r28, r24, r28
		xor	    r23, r16, r20
		xor	    r18, r30, r14
		add	    r4,	r4, r22
		xor	    r24, r21, r17
		xor	    r19, r29, r14
		slwi	r20, r25, 4
		add	    r23, r23, r18
		slwi	r21, r30, 4
		add	    r24, r24, r19
		add	    r4,	r4, r20
		slwi	r22, r29, 4
		add	    r28, r28, r11
		add	    r23, r23, r21
		add	    r24, r24, r22
		add	    r4,	r4, r27
		srwi	r22, r28, 5
		add	    r23, r23, r3
		add	    r27, r24, r31
		add	    r3,	r4, r11
		xor	    r4,	r22, r5
		xor	    r24, r28, r15
		add	    r31, r23, r11
		add	    r4,	r4, r24
		slwi	r24, r28, 4
		add	    r27, r27, r11
		srwi	r23, r3, 5
		add	    r24, r4, r24
		srwi	r22, r31, 5
		srwi	r19, r27, 5
		xor	    r4,	r23, r5
		xor	    r20, r3, r15
		add	    r26, r24, r26
		xor	    r22, r22, r5
		xor	    r21, r31, r15
		xor	    r24, r19, r5
		xor	    r23, r27, r15
		add	    r4,	r4, r20
		add	    r22, r22, r21
		slwi	r20, r3, 4
		add	    r26, r26, r6
		add	    r24, r24, r23
		slwi	r21, r31, 4
		slwi	r23, r27, 4
		add	    r4,	r4, r20
		srwi	r19, r26, 5
		add	    r20, r22, r21
		add	    r21, r24, r23
		xor	    r24, r19, r5
		xor	    r22, r26, r14
		add	    r25, r4, r25
		add	    r24, r24, r22
		slwi	r23, r26, 4
		add	    r25, r25, r6
		add	    r24, r24, r23
		srwi	r4,	r25, 5
		add	    r28, r24, r28
		xor	    r24, r4, r5
		xor	    r22, r25, r14
		add	    r29, r21, r29
		add	    r30, r20, r30
		add	    r24, r24, r22
		slwi	r23, r25, 4
		add	    r30, r30, r6
		add	    r29, r29, r6
		add	    r28, r28, r11
		lis	    r4,	-0xE45 //;# 0xF1BBCDC8
		add	    r24, r24, r23
		srwi	r23, r30, 5
		srwi	r22, r29, 5
		ori	    r4,	r4, 0xCDC8 //;# 0xF1BBCDC8
		srwi	r21, r28, 5
		add	    r3,	r24, r3
		xor	    r23, r23, r5
		xor	    r24, r22, r5
		xor	    r17, r30, r14
		xor	    r18, r29, r14
		xor	    r19, r28, r15
		xor	    r5,	r21, r4
		add	    r24, r24, r18
		add	    r5,	r5, r19
		slwi	r21, r29, 4
		slwi	r22, r28, 4
		add	    r3,	r3, r11
		add	    r24, r24, r21
		add	    r5,	r5, r22
		add	    r23, r23, r17
		slwi	r20, r30, 4
		srwi	r22, r3, 5
		add	    r27, r24, r27
		add	    r23, r23, r20
		add	    r26, r5, r26
		xor	    r5,	r22, r4
		xor	    r24, r3, r15
		add	    r31, r23, r31
		add	    r5,	r5, r24
		slwi	r24, r3, 4
		add	    r31, r31, r11
		add	    r27, r27, r11
		add	    r5,	r5, r24
		add	    r26, r26, r6
		srwi	r24, r31, 5
		srwi	r23, r27, 5
		add	    r25, r5, r25
		srwi	r19, r26, 5
		xor	    r5,	r24, r4
		xor	    r20, r31, r15
		xor	    r22, r23, r4
		xor	    r21, r27, r15
		xor	    r24, r19, r4
		xor	    r23, r26, r14
		add	    r5,	r5, r20
		add	    r22, r22, r21
		slwi	r20, r31, 4
		add	    r25, r25, r6
		add	    r24, r24, r23
		slwi	r21, r27, 4
		slwi	r23, r26, 4
		add	    r5,	r5, r20
		srwi	r19, r25, 5
		add	    r20, r22, r21
		add	    r21, r24, r23
		xor	    r24, r19, r4
		xor	    r22, r25, r14
		add	    r30, r5, r30
		add	    r24, r24, r22
		slwi	r23, r25, 4
		add	    r30, r30, r6
		add	    r24, r24, r23
		add	    r23, r20, r29
		srwi	r5,	r30, 5
		add	    r29, r21, r28
		add	    r28, r24, r3
		xor	    r24, r5, r4
		xor	    r22, r30, r14
		add	    r3,	r23, r6
		add	    r24, r24, r22
		slwi	r23, r30, 4
		add	    r29, r29, r11
		add	    r28, r28, r11
		lis	    r5,	-0x700D	//; 0x8FF34781
		add	    r24, r24, r23
		srwi	r23, r3, 5
		ori	    r5,	r5, 0x4781 //; 0x8FF34781
		srwi	r22, r29, 5
		srwi	r21, r28, 5
		add	    r31, r24, r31
		xor	    r23, r23, r4
		xor	    r24, r22, r5
		xor	    r4,	r21, r5
		xor	    r17, r3, r14
		xor	    r18, r29, r15
		xor	    r19, r28, r15
		add	    r31, r31, r11
		slwi	r20, r3, 4
		slwi	r21, r29, 4
		slwi	r22, r28, 4
		add	    r23, r23, r17
		add	    r24, r24, r18
		add	    r4,	r4, r19
		add	    r24, r24, r21
		add	    r4,	r4, r22
		srwi	r22, r31, 5
		add	    r23, r23, r20
		add	    r26, r24, r26
		add	    r25, r4, r25
		xor	    r4,	r22, r5
		xor	    r24, r31, r15
		add	    r27, r23, r27
		add	    r4,	r4, r24
		add	    r27, r27, r11
		slwi	r24, r31, 4
		add	    r26, r26, r6
		add	    r25, r25, r6
		add	    r4,	r4, r24
		srwi	r23, r27, 5
		srwi	r24, r26, 5
		srwi	r19, r25, 5
		xor	    r23, r23, r5
		xor	    r20, r27, r15
		xor	    r22, r24, r5
		add	    r30, r4, r30
		xor	    r21, r26, r14
		xor	    r4,	r19, r5
		xor	    r24, r25, r14
		add	    r23, r23, r20
		add	    r22, r22, r21
		add	    r30, r30, r6
		slwi	r20, r27, 4
		add	    r4,	r4, r24
		slwi	r21, r26, 4
		slwi	r24, r25, 4
		add	    r23, r23, r20
		srwi	r19, r30, 5
		add	    r20, r22, r21
		add	    r21, r4, r24
		xor	    r24, r19, r5
		xor	    r22, r30, r14
		add	    r3,	r23, r3
		add	    r24, r24, r22
		slwi	r23, r30, 4
		add	    r3,	r3, r6
		add	    r24, r24, r23
		mr	    r4,	r5
		add	    r5,	r24, r31
		add	    r23, r20, r29
		srwi	r24, r3, 5
		add	    r29, r21, r28
		add	    r28, r5, r11
		add	    r31, r23, r11
		xor	    r5,	r24, r4
		xor	    r23, r3, r14
		lis	    r4,	0x2E2A //; 0x2E2AC13A
		add	    r29, r29, r11
		add	    r5,	r5, r23
		slwi	r24, r3, 4
		ori	    r4,	r4, 0xC13A //; 0x2E2AC13A
		srwi	r23, r31, 5
		srwi	r21, r29, 5
		srwi	r20, r28, 5
		add	    r22, r5, r24
		xor	    r24, r23, r4
		xor	    r17, r31, r15
		xor	    r23, r21, r4
		xor	    r5,	r20, r4
		xor	    r18, r29, r15
		xor	    r19, r28, r15
		add	    r27, r22, r27
		add	    r24, r24, r17
		slwi	r20, r31, 4
		add	    r5,	r5, r19
		slwi	r21, r29, 4
		slwi	r22, r28, 4
		add	    r23, r23, r18
		add	    r24, r24, r20
		add	    r23, r23, r21
		add	    r5,	r5, r22
		add	    r24, r24, r26
		add	    r26, r23, r25
		add	    r5,	r5, r30
		add	    r27, r27, r11
		add	    r30, r24, r6
		srwi	r24, r27, 5
		add	    r25, r5, r6
		xor	    r5,	r24, r4
		xor	    r23, r27, r15
		slwi	r24, r27, 4
		add	    r5,	r5, r23
		add	    r26, r26, r6
		srwi	r23, r30, 5
		add	    r19, r5, r24
		srwi	r5,	r26, 5
		xor	    r24, r23, r4
		xor	    r20, r30, r14
		srwi	r18, r25, 5
		xor	    r22, r5, r4
		xor	    r21, r26, r14
		add	    r3,	r19, r3
		add	    r24, r24, r20
		xor	    r23, r25, r14
		xor	    r5,	r18, r4
		slwi	r20, r30, 4
		add	    r22, r22, r21
		add	    r3,	r3, r6
		add	    r5,	r5, r23
		slwi	r21, r26, 4
		add	    r24, r24, r20
		slwi	r23, r25, 4
		mr	    r19, r4
		srwi	r20, r3, 5
		add	    r21, r22, r21
		add	    r4,	r24, r31
		add	    r22, r5, r23
		xor	    r31, r20, r19
		xor	    r23, r3, r14
		slwi	r24, r3, 4
		add	    r31, r31, r23
		add	    r23, r21, r29
		add	    r4,	r4, r11
		lis	    r5,	-0x339E	//; 0xCC623AF3
		add	    r29, r22, r28
		add	    r28, r31, r24
		ori	    r5,	r5, 0x3AF3 //; 0xCC623AF3
		srwi	r24, r4, 5
		add	    r28, r28, r27
		add	    r31, r23, r11
		xor	    r27, r24, r5
		xor	    r23, r4, r15
		add	    r29, r29, r11
		add	    r27, r27, r23
		slwi	r24, r4, 4
		add	    r28, r28, r11
		srwi	r23, r31, 5
		add	    r24, r27, r24
		srwi	r21, r29, 5
		srwi	r20, r28, 5
		xor	    r27, r23, r5
		add	    r30, r24, r30
		xor	    r22, r29, r15
		xor	    r23, r21, r5
		xor	    r18, r31, r15
		xor	    r24, r20, r5
		xor	    r19, r28, r15
		add	    r27, r27, r18
		add	    r23, r23, r22
		slwi	r20, r31, 4
		add	    r24, r24, r19
		slwi	r22, r28, 4
		slwi	r21, r29, 4
		add	    r30, r30, r6
		add	    r27, r27, r20
		add	    r24, r24, r22
		add	    r23, r23, r21
		srwi	r22, r30, 5
		add	    r27, r27, r26
		add	    r26, r23, r25
		add	    r3,	r24, r3
		xor	    r24, r30, r14
		xor	    r25, r22, r5
		add	    r27, r27, r6
		add	    r26, r26, r6
		add	    r25, r25, r24
		slwi	r24, r30, 4
		srwi	r23, r27, 5
		add	    r22, r25, r24
		srwi	r21, r26, 5
		xor	    r24, r23, r5
		mr	    r19, r5
		xor	    r23, r27, r14
		xor	    r5,	r21, r5
		add	    r3,	r3, r6
		xor	    r25, r26, r14
		add	    r20, r22, r4
		add	    r24, r24, r23
		add	    r5,	r5, r25
		slwi	r23, r27, 4
		slwi	r4,	r26, 4
		srwi	r22, r3, 5
		add	    r25, r20, r11
		lis	    r18, 0x6A99	//; 0x6A99B4AC
		add	    r24, r24, r23
		add	    r20, r5, r4
		xor	    r22, r22, r19
		xor	    r21, r3, r14
		ori	    r5,	r18, 0xB4AC //; 0x6A99B4AC
		srwi	r4,	r25, 5
		add	    r31, r24, r31
		add	    r21, r22, r21
		xor	    r24, r4, r5
		xor	    r22, r25, r15
		slwi	r19, r3, 4
		add	    r24, r24, r22
		slwi	r23, r25, 4
		add	    r21, r21, r19
		add	    r24, r24, r23
		add	    r23, r20, r29
		add	    r29, r21, r28
		add	    r28, r24, r30
		add	    r30, r23, r11
		add	    r31, r31, r11
		xor	    r4,	r30, r15
		srwi	r24, r31, 5
		stw	    r4,	var_A0(sp)
		xor	    r22, r31, r15
		xor	    r24, r24, r5
		slwi	r23, r31, 4
		add	    r24, r24, r22
		add	    r29, r29, r11
		add	    r28, r28, r6
		srwi	r4,	r30, 5
		add	    r24, r24, r23
		srwi	r21, r28, 5
		srwi	r23, r29, 5
		xor	    r22, r4, r5
		add	    r27, r24, r27
		xor	    r23, r23, r5
		xor	    r24, r21, r5
		xor	    r17, r29, r15
		xor	    r18, r28, r14
		slwi	r19, r30, 4
		add	    r24, r24, r18
		add	    r23, r23, r17
		slwi	r20, r29, 4
		lwz	    r4,	var_A0(sp)
		slwi	r21, r28, 4
		add	    r27, r27, r6
		add	    r22, r22, r4
		add	    r24, r24, r21
		add	    r22, r22, r19
		add	    r23, r23, r20
		srwi	r4,	r27, 5
		add	    r22, r22, r26
		add	    r26, r23, r3
		add	    r25, r24, r25
		xor	    r24, r4, r5
		xor	    r23, r27, r14
		lis	    r4,	0x8D1 //;	0x8D12E65
		add	    r24, r24, r23
		add	    r3,	r22, r6
		add	    r26, r26, r6
		add	    r25, r25, r11
		slwi	r23, r27, 4
		ori	    r4,	r4, 0x2E65 //; 0x8D12E65
		add	    r24, r24, r23
		srwi	r23, r26, 5
		xor	    r20, r26, r14
		xor	    r21, r23, r5
		slwi	r18, r26, 4
		add	    r20, r21, r20
		srwi	r22, r3, 5
		add	    r20, r20, r18
		add	    r31, r24, r31
		add	    r29, r20, r29
		xor	    r24, r22, r5
		xor	    r23, r3, r14
		add	    r29, r29, r11
		add	    r24, r24, r23
		xor	    r5,	r29, r15
		srwi	r19, r25, 5
		slwi	r23, r3, 4
		stw	    r5,	var_A0(sp)
		add	    r31, r31, r11
		add	    r24, r24, r23
		xor	    r5,	r19, r4
		xor	    r22, r25, r15
		srwi	r23, r31, 5
		add	    r30, r24, r30
		add	    r21, r5, r22
		xor	    r24, r23, r4
		xor	    r22, r31, r15
		slwi	r23, r31, 4
		add	    r24, r24, r22
		add	    r30, r30, r11
		slwi	r19, r25, 4
		add	    r24, r24, r23
		srwi	r5,	r30, 5
		add	    r21, r21, r19
		add	    r27, r24, r27
		xor	    r24, r5, r4
		xor	    r22, r30, r15
		add	    r28, r21, r28
		lwz	    r5,	var_A0(sp)
		add	    r24, r24, r22
		slwi	r23, r30, 4
		add	    r28, r28, r6
		add	    r27, r27, r6
		add	    r24, r24, r23
		srwi	r22, r29, 5
		srwi	r23, r28, 5
		srwi	r21, r27, 5
		xor	    r22, r22, r4
		add	    r3,	r24, r3
		xor	    r23, r23, r4
		xor	    r17, r28, r14
		xor	    r24, r21, r4
		xor	    r18, r27, r14
		add	    r22, r22, r5
		slwi	r19, r29, 4
		add	    r23, r23, r17
		slwi	r20, r28, 4
		add	    r24, r24, r18
		slwi	r21, r27, 4
		add	    r3,	r3, r6
		add	    r22, r22, r19
		add	    r23, r23, r20
		add	    r24, r24, r21
		add	    r22, r22, r26
		srwi	r5,	r3, 5
		add	    r26, r23, r25
		add	    r25, r24, r31
		xor	    r24, r5, r4
		xor	    r23, r3, r14
		add	    r31, r22, r6
		add	    r24, r24, r23
		slwi	r23, r3, 4
		add	    r26, r26, r11
		add	    r24, r24, r23
		lis	    r5,	-0x58F8	//; 0xA708A81E
		add	    r25, r25, r11
		ori	    r5,	r5, 0xA81E //; 0xA708A81E
		srwi	r23, r31, 5
		srwi	r22, r26, 5
		add	    r30, r24, r30
		srwi	r19, r25, 5
		xor	    r4,	r23, r4
		xor	    r24, r31, r14
		add	    r30, r30, r11
		add	    r4,	r4, r24
		slwi	r24, r31, 4
		xor	    r21, r22, r5
		add	    r24, r4, r24
		xor	    r20, r26, r15
		xor	    r23, r19, r5
		xor	    r22, r25, r15
		srwi	r4,	r30, 5
		add	    r29, r24, r29
		add	    r20, r21, r20
		add	    r21, r23, r22
		slwi	r19, r25, 4
		xor	    r24, r4, r5
		xor	    r22, r30, r15
		add	    r29, r29, r11
		add	    r21, r21, r19
		add	    r24, r24, r22
		slwi	r23, r30, 4
		srwi	r4,	r29, 5
		add	    r27, r21, r27
		add	    r21, r24, r23
		slwi	r18, r26, 4
		xor	    r24, r4, r5
		xor	    r22, r29, r15
		add	    r20, r20, r18
		add	    r24, r24, r22
		slwi	r23, r29, 4
		add	    r27, r27, r6
		add	    r28, r20, r28
		add	    r24, r24, r23
		srwi	r4,	r27, 5
		add	    r28, r28, r6
		add	    r31, r24, r31
		add	    r3,	r21, r3
		xor	    r24, r4, r5
		srwi	r4,	r28, 5
		add	    r3,	r3, r6
		xor	    r21, r27, r14
		xor	    r22, r4, r5
		add	    r24, r24, r21
		slwi	r20, r27, 4
		srwi	r4,	r3, 5
		xor	    r17, r28, r14
		add	    r31, r31, r6
		xor	    r23, r4, r5
		add	    r24, r24, r20
		add	    r22, r22, r17
		slwi	r19, r28, 4
		srwi	r4,	r31, 5
		xor	    r18, r3, r14
		add	    r25, r24, r25
		add	    r22, r22, r19
		xor	    r5,	r4, r5
		add	    r23, r23, r18
		slwi	r21, r3, 4
		xor	    r24, r31, r14
		add	    r26, r22, r26
		add	    r5,	r5, r24
		add	    r23, r23, r21
		slwi	r24, r31, 4
		add	    r26, r26, r11
		lis	    r4,	0x4540 //; 0x454021D7
		add	    r30, r23, r30
		add	    r5,	r5, r24
		ori	    r4,	r4, 0x21D7 //; 0x454021D7
		srwi	r23, r26, 5
		add	    r25, r25, r11
		add	    r30, r30, r11
		add	    r29, r5, r29
		xor	    r5,	r23, r4
		srwi	r22, r25, 5
		xor	    r24, r26, r15
		srwi	r23, r30, 5
		xor	    r22, r22, r4
		xor	    r20, r25, r15
		add	    r5,	r5, r24
		xor	    r23, r23, r4
		xor	    r21, r30, r15
		slwi	r24, r26, 4
		add	    r29, r29, r11
		add	    r24, r5, r24
		srwi	r5,	r29, 5
		add	    r28, r24, r28
		add	    r20, r22, r20
		slwi	r18, r25, 4
		add	    r21, r23, r21
		slwi	r19, r30, 4
		xor	    r24, r5, r4
		xor	    r22, r29, r15
		add	    r20, r20, r18
		add	    r28, r28, r6
		add	    r21, r21, r19
		add	    r24, r24, r22
		slwi	r23, r29, 4
		add	    r20, r20, r27
		srwi	r5,	r28, 5
		add	    r27, r21, r3
		add	    r21, r24, r23
		xor	    r24, r5, r4
		xor	    r22, r28, r14
		add	    r31, r21, r31
		add	    r24, r24, r22
		slwi	r23, r28, 4
		add	    r3,	r20, r6
		add	    r27, r27, r6
		add	    r24, r24, r23
		add	    r31, r31, r6
		mr	    r5,	r4
		srwi	r23, r3, 5
		mr	    r21, r4
		mr	    r20, r4
		srwi	r19, r27, 5
		add	    r4,	r24, r26
		srwi	r17, r31, 5
		xor	    r24, r23, r5
		xor	    r18, r3, r14
		xor	    r26, r19, r21
		xor	    r22, r27, r14
		xor	    r23, r17, r20
		xor	    r19, r31, r14
		add	    r24, r24, r18
		slwi	r20, r3, 4
		add	    r26, r26, r22
		add	    r4,	r4, r11
		lis	    r5,	-0x1C89	//; 0xE3779B90
		add	    r23, r23, r19
		slwi	r22, r31, 4
		add	    r24, r24, r20
		slwi	r21, r27, 4
		ori	    r5,	r5, 0x9B90 //; 0xE3779B90
		srwi	r20, r4, 5
		add	    r22, r23, r22
		add	    r23, r24, r25
		add	    r26, r26, r21
		xor	    r25, r20, r5
		xor	    r24, r4, r15
		add	    r26, r26, r30
		add	    r25, r25, r24
		add	    r29, r22, r29
		slwi	r24, r4, 4
		add	    r30, r23, r11
		add	    r26, r26, r11
		add	    r29, r29, r11
		add	    r25, r25, r24
		srwi	r24, r30, 5
		srwi	r22, r26, 5
		srwi	r19, r29, 5
		add	    r28, r25, r28
		xor	    r23, r24, r5
		xor	    r20, r30, r15
		xor	    r21, r26, r15
		xor	    r25, r22, r5
		xor	    r22, r29, r15
		xor	    r24, r19, r5
		add	    r23, r23, r20
		add	    r25, r25, r21
		slwi	r20, r30, 4
		slwi	r21, r26, 4
		add	    r24, r24, r22
		slwi	r22, r29, 4
		add	    r23, r23, r20
		add	    r25, r25, r21
		add	    r24, r24, r22
		add	    r3,	r23, r3
		add	    r25, r25, r27
		add	    r27, r24, r31
		add	    r3,	r3, r6
		add	    r31, r25, r6
		add	    r28, r28, r6
		add	    r6,	r27, r6
		srwi	r25, r3, 5
		srwi	r23, r31, 5
		srwi	r27, r28, 5
		srwi	r22, r6, 5
		xor	    r24, r25, r5
		xor	    r27, r27, r5
		xor	    r25, r23, r5
		xor	    r5,	r22, r5
		xor	    r20, r28, r14
		xor	    r21, r3, r14
		xor	    r22, r31, r14
		xor	    r23, r6, r14
		add	    r27, r27, r20
		slwi	r20, r28, 4
		add	    r24, r24, r21
		add	    r25, r25, r22
		add	    r5,	r5, r23
		slwi	r21, r3, 4
		slwi	r22, r31, 4
		slwi	r23, r6, 4
		add	    r27, r27, r20
		add	    r24, r24, r21
		add	    r25, r25, r22
		add	    r5,	r5, r23
		add	    r27, r27, r4
		add	    r30, r24, r30
		add	    r4,	r25, r26
		add	    r5,	r5, r29
		add	    r29, r27, r11
		add	    r30, r30, r11
		add	    r4,	r4, r11
		clrldi  r28, r28, 32
		clrldi  r3,	r3, 32
		clrldi  r31, r31, 32
		add	    r11, r5, r11
		clrldi  r6,	r6, 32
		insrdi  r28, r29, 32,0
		insrdi  r3,	r30, 32,0
		insrdi  r31, r4, 32,0
		std	    r28, 0(r7)
		insrdi  r6,	r11, 32,0
		std	    r3,	0(r8)
		std	    r31, 0(r9)
		std	    r6,	0(r10)
		bl	    _restgprlr_14
		blr
	}

#undef var_A0
#undef arg_50
#undef arg_58
}

static __declspec(naked) void EraEncyptBlock()
{
#define var_C0 -0xC0
#define var_B8 -0xB8
#define var_B0 -0xB0
#define var_A8 -0xA8
#define var_A0 -0xA0
#define var_98 -0x98
#define var_90 -0x90
#define var_88 -0x88
#define var_80 -0x80
#define var_78 -0x78
#define var_70 -0x70

	__asm {
		mfspr	r12, LR
		bl	    _savegprlr_20
		stwu	sp,	-0x110(sp)
		mr	    r11, r7
		addis	r10, r9, -0x4333
		mr	    r27, r6
		mr	    r31, r8
		mr	    r28, r4
		ld	    r8,	0(r11)
		mr	    r29, r5
		ld	    r7,	8(r11)
		addic.  r10, r10, 0x923
		ld	    r6,	0x10(r11)
		ld	    r26, 0x18(r11)
		ld	    r25, 0x20(r11)
		ld	    r24, 0x28(r11)
		ld	    r23, 0x30(r11)
		ld	    r22, 0x38(r11)
		bne	    loc_8265E2E0
		addi	r10, r10, 1

loc_8265E2E0:
		slwi	r11, r10, 17
		std	    r28, 0x110+var_B8(sp)
		lis	    r4,	0x15EF //; 0x15EF0AF3
		std	    r29, 0x110+var_C0(sp)
		xor	    r5,	r11, r10
		lis	    r11, -0x3425 //; 0xCBDB701E
		srwi	r3,	r5, 13
		ori	    r30, r11, 0x701E //; 0xCBDB701E
		xor	    r10, r3, r5
		lis	    r9,	0x3424 //; 0x34248FE2
		slwi	r3,	r10, 5
		ori	    r9,	r9, 0x8FE2 //; 0x34248FE2
		xor	    r3,	r3, r10
		ori	    r10, r4, 0xAF3 //; 0x15EF0AF3
		slwi	r4,	r3, 17
		lis	    r5,	-0x15F0	//; 0xEA10F50C
		xor	    r11, r4, r3
		insrdi  r9,	r10, 32,0
		srwi	r4,	r11, 13
		ori	    r5,	r5, 0xF50C //; 0xEA10F50C
		xor	    r10, r4, r11
		insrdi  r30, r5, 32,0
		slwi	r5,	r10, 5
		mr	    r11, r9
		xor	    r4,	r5, r10
		clrldi  r9,	r3, 32
		slwi	r10, r4, 17
		xor	    r5,	r10, r4
		add	    r10, r9, r11
		srwi	r3,	r5, 13
		clrldi  r4,	r4, 32
		xor	    r9,	r3, r5
		add	    r5,	r4, r30
		slwi	r4,	r9, 5
		xor	    r3,	r10, r8
		xor	    r10, r4, r9
		xor	    r4,	r5, r7
		std	    r3,	0x110+var_B0(sp)
		slwi	r9,	r10, 17
		clrldi  r8,	r10, 32
		std	    r4,	0x110+var_A8(sp)
		xor	    r7,	r9, r10
		add	    r5,	r8, r11
		srwi	r10, r7, 13
		xor	    r5,	r5, r6
		xor	    r8,	r10, r7
		addi	r10, sp, 0x110+var_80
		std	    r5,	0x110+var_A0(sp)
		slwi	r7,	r8, 5
		addi	r9,	sp, 0x110+var_88
		xor	    r6,	r7, r8
		addi	r8,	sp, 0x110+var_90
		slwi	r7,	r6, 17
		clrldi  r21, r6, 32
		xor	    r7,	r7, r6
		add	    r6,	r21, r30
		srwi	r21, r7, 13
		xor	    r6,	r6, r26
		xor	    r26, r21, r7
		std	    r6,	0x110+var_98(sp)
		addi	r7,	sp, 0x110+var_78
		slwi	r21, r26, 5
		xor	    r26, r21, r26
		slwi	r21, r26, 17
		clrldi  r20, r26, 32
		xor	    r26, r21, r26
		add	    r21, r20, r11
		srwi	r20, r26, 13
		xor	    r25, r21, r25
		xor	    r26, r20, r26
		slwi	r21, r26, 5
		xor	    r26, r21, r26
		slwi	r21, r26, 17
		clrldi  r20, r26, 32
		xor	    r26, r21, r26
		add	    r21, r20, r30
		srwi	r20, r26, 13
		xor	    r24, r21, r24
		xor	    r26, r20, r26
		slwi	r21, r26, 5
		xor	    r26, r21, r26
		slwi	r21, r26, 17
		clrldi  r20, r26, 32
		xor	    r26, r21, r26
		add	    r11, r20, r11
		srwi	r21, r26, 13
		xor	    r23, r11, r23
		xor	    r26, r21, r26
		bl	    EraProcessAndXorBlockEncypt
		slwi	r9,	r26, 5
		std	    r27, 0x110+var_B8(sp)
		clrldi  r8,	r26, 32
		std	    r29, 0x110+var_C0(sp)
		ld	    r11, 0x110+var_90(sp)
		addi	r10, sp, 0x110+var_90
		xor	    r7,	r9, r8
		ld	    r6,	0x110+var_88(sp)
		ld	    r5,	0x110+var_80(sp)
		addi	r9,	sp, 0x110+var_88
		add	    r4,	r7, r30
		xor	    r3,	r11, r6
		xor	    r6,	r6, r5
		xor	    r4,	r4, r22
		xor	    r27, r3, r6
		addi	r8,	sp, 0x110+var_80
		addi	r7,	sp, 0x110+var_70
		ld	    r30, 0x110+var_78(sp)
		xor	    r11, r30, r11
		xor	    r5,	r11, r5
		xor	    r3,	r11, r3
		xor	    r11, r6, r5
		xor	    r5,	r3, r5
		xor	    r30, r3, r27
		xor	    r26, r11, r5
		xor	    r22, r30, r5
		xor	    r27, r27, r11
		add	    r6,	r4, r30
		xor	    r5,	r27, r23
		subf	r4,	r26, r24
		xor	    r3,	r22, r25
		bl	    EraProcessAndXorBlockEncypt
		std	    r29, 0x110+var_B8(sp)
		std	    r28, 0x110+var_C0(sp)
		addi	r10, sp, 0x110+var_98
		ld	    r6,	0x110+var_88(sp)
		addi	r9,	sp, 0x110+var_A0
		ld	    r11, 0x110+var_80(sp)
		addi	r8,	sp, 0x110+var_A8
		ld	    r5,	0x110+var_90(sp)
		addi	r7,	sp, 0x110+var_B0
		xor	    r4,	r11, r6
		xor	    r3,	r6, r5
		xor	    r6,	r4, r3
		ld	    r29, 0x110+var_70(sp)
		xor	    r11, r29, r11
		xor	    r5,	r11, r5
		xor	    r4,	r11, r4
		xor	    r3,	r3, r5
		xor	    r11, r4, r5
		xor	    r28, r4, r6
		xor	    r25, r6, r3
		xor	    r24, r3, r11
		xor	    r29, r28, r11
		xor	    r6,	r28, r22
		add	    r3,	r29, r30
		subf	r5,	r25, r26
		xor	    r4,	r24, r27
		bl	    EraProcessAndXorBlockEncypt
		xor	    r10, r28, r25
		xor	    r9,	r25, r24
		ld	    r5,	0x110+var_B0(sp)
		xor	    r8,	r24, r29
		ld	    r4,	0x110+var_A8(sp)
		xor	    r7,	r10, r29
		ld	    r11, 0x110+var_A0(sp)
		xor	    r6,	r10, r9
		xor	    r3,	r9, r8
		xor	    r9,	r8, r7
		std	    r5,	0(r31)
		xor	    r8,	r6, r7
		xor	    r7,	r6, r3
		ld	    r10, 0x110+var_98(sp)
		std	    r4,	8(r31)
		xor	    r6,	r3, r9
		xor	    r5,	r9, r8
		std	    r11, 0x10(r31)
		xor	    r4,	r7, r8
		std	    r10, 0x18(r31)
		std	    r7,	0x20(r31)
		std	    r6,	0x28(r31)
		std	    r5,	0x30(r31)
		std	    r4,	0x38(r31)
		addi	sp,	sp, 0x110
		bl	    _restgprlr_20
		blr
	}

#undef var_C0
#undef var_B8
#undef var_B0
#undef var_A8
#undef var_A0
#undef var_98
#undef var_90
#undef var_88
#undef var_80
#undef var_78
#undef var_70
}

// Whoops...this is still a WIP
//										r3													 r4			 r5			  r6				  r7
static __declspec(naked) void EraEncypt(const Phoenix::s_ensemble_cryptor_context* ecf_data, void* addr, void* addr2, size_t buffer_size, __int32)
{
	__asm {
		mfspr	r12, LR
		bl	    _savegprlr_26
		stwu	sp,	-0x90(sp)
		srwi	r28, r6, 6
		srdi	r11, r7, 6
		mr	    r29, r3
		li	    r30, 0
		clrlwi  r26, r11, 0
		cmplwi  cr6, r28, 0
		beq	    cr6, loc_825F40C0
		mr	    r31, r4
		subf	r27, r4, r5

loc_825F4080:
		lis	    r11, 0x3424	//; 0x34248FE2
		ld	    r6,	0x10(r29)
		lis	    r10, 0x15EF	//; 0x15EF0AF3
		ld	    r5,	0x8(r29)
		ori	    r3,	r11, 0x8FE2 //; 0x34248FE2
		ld	    r4,	0x0(r29)
		ori	    r8,	r10, 0xAF3 //; 0x15EF0AF3
		add	    r9,	r30, r26
		insrdi  r3,	r8, 32,0
		add	    r8,	r27, r31
		mr	    r7,	r31
		bl	    EraEncyptBlock
		addi	r30, r30, 1
		addi	r31, r31, 0x40
		cmplw	cr6, r30, r28
		blt	    cr6, loc_825F4080

loc_825F40C0:
		addi	sp,	sp, 0x90
		bl		_restgprlr_26
		blr
	}
}