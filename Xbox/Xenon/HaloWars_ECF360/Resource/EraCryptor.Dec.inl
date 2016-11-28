
static __declspec(naked) void EraProcessAndXorBlockDecypt()
{
#define arg_50 0x50
#define arg_58 0x58
	__asm {
		mfspr	r12, LR
		bl	    _savegprlr_17
		lis	    r11, -0x1C89 //; 0xE3779B90
		mr	    r30, r6
		ld	    r6,	arg_58(sp)
		ori	    r11, r11, 0x9B90 //; 0xE3779B90
		srwi	r29, r3, 5
		mr	    r31, r5
		ld	    r5,	arg_50(sp)
		xor	    r29, r29, r11
		mr	    r28, r6
		clrlwi  r27, r3, 0
		srdi	r26, r3, 32
		srwi	r25, r4, 5
		srwi	r24, r31, 5
		subf	r29, r29, r26
		xor	    r23, r27, r28
		xor	    r25, r25, r11
		clrlwi  r26, r4, 0
		srdi	r21, r4, 32
		clrlwi  r22, r31, 0
		xor	    r24, r24, r11
		srdi	r20, r31, 32
		subf	r29, r23, r29
		srwi	r19, r30, 5
		slwi	r3,	r3, 4
		subf	r25, r25, r21
		xor	    r18, r26, r28
		subf	r24, r24, r20
		xor	    r17, r22, r28
		subf	r3,	r3, r29
		srdi	r23, r6, 32
		srdi	r21, r30, 32
		xor	    r19, r19, r11
		subf	r29, r18, r25
		clrlwi  r6,	r30, 0
		subf	r25, r17, r24
		slwi	r4,	r4, 4
		subf	r24, r19, r21
		slwi	r31, r31, 4
		xor	    r21, r6, r28
		subf	r3,	r23, r3
		subf	r4,	r4, r29
		subf	r31, r31, r25
		subf	r29, r21, r24
		slwi	r30, r30, 4
		srwi	r25, r3, 5
		subf	r4,	r23, r4
		subf	r31, r23, r31
		xor	    r25, r25, r11
		subf	r30, r30, r29
		mr	    r24, r5
		srwi	r29, r4, 5
		srwi	r21, r31, 5
		subf	r27, r25, r27
		xor	    r29, r29, r11
		subf	r30, r23, r30
		xor	    r25, r3, r24
		xor	    r21, r21, r11
		subf	r27, r25, r27
		subf	r29, r29, r26
		srwi	r20, r30, 5
		subf	r26, r21, r22
		slwi	r25, r3, 4
		xor	    r21, r31, r24
		xor	    r11, r20, r11
		xor	    r22, r4, r24
		subf	r27, r25, r27
		srdi	r5,	r5, 32
		subf	r26, r21, r26
		subf	r21, r11, r6
		subf	r29, r22, r29
		slwi	r25, r4, 4
		slwi	r22, r31, 4
		subf	r27, r5, r27
		lis	    r11, 0x4540	//; 0x454021D7
		lis	    r6,	-0x58F8	//; 0xA708A81E
		subf	r29, r25, r29
		xor	    r20, r30, r24
		subf	r26, r22, r26
		ori	    r11, r11, 0x21D7 //; 0x454021D7
		srwi	r25, r27, 5
		ori	    r6,	r6, 0xA81E //; 0xA708A81E
		xor	    r25, r25, r11
		subf	r22, r20, r21
		subf	r3,	r25, r3
		xor	    r20, r27, r28
		subf	r29, r5, r29
		subf	r3,	r20, r3
		slwi	r21, r30, 4
		srwi	r20, r29, 5
		slwi	r25, r27, 4
		subf	r22, r21, r22
		xor	    r21, r20, r11
		subf	r3,	r25, r3
		subf	r26, r5, r26
		subf	r25, r5, r22
		subf	r4,	r21, r4
		xor	    r22, r29, r28
		srwi	r21, r26, 5
		subf	r3,	r23, r3
		srwi	r20, r25, 5
		subf	r4,	r22, r4
		xor	    r22, r21, r11
		slwi	r18, r29, 4
		xor	    r21, r20, r11
		srwi	r19, r3, 5
		subf	r31, r22, r31
		subf	r4,	r18, r4
		subf	r30, r21, r30
		xor	    r20, r19, r11
		xor	    r22, r26, r28
		xor	    r21, r25, r28
		subf	r4,	r23, r4
		subf	r31, r22, r31
		subf	r27, r20, r27
		slwi	r19, r26, 4
		subf	r30, r21, r30
		xor	    r20, r3, r24
		slwi	r18, r25, 4
		srwi	r21, r4, 5
		subf	r31, r19, r31
		slwi	r22, r3, 4
		subf	r27, r20, r27
		subf	r30, r18, r30
		xor	    r21, r21, r11
		subf	r31, r23, r31
		subf	r27, r22, r27
		subf	r30, r23, r30
		subf	r29, r21, r29
		srwi	r21, r31, 5
		subf	r27, r5, r27
		xor	    r22, r4, r24
		srwi	r20, r30, 5
		xor	    r21, r21, r11
		subf	r29, r22, r29
		xor	    r11, r20, r11
		srwi	r19, r27, 5
		slwi	r22, r4, 4
		subf	r11, r11, r25
		xor	    r20, r19, r6
		subf	r26, r21, r26
		subf	r29, r22, r29
		xor	    r25, r31, r24
		xor	    r22, r30, r24
		subf	r3,	r20, r3
		xor	    r21, r27, r28
		subf	r26, r25, r26
		subf	r11, r22, r11
		slwi	r25, r31, 4
		subf	r3,	r21, r3
		slwi	r22, r30, 4
		subf	r29, r5, r29
		slwi	r21, r27, 4
		subf	r26, r25, r26
		subf	r25, r22, r11
		srwi	r20, r29, 5
		lis	    r11, 0x8D1 //; 0x8D12E65
		subf	r3,	r21, r3
		xor	    r22, r20, r6
		ori	    r11, r11, 0x2E65 //; 0x8D12E65
		subf	r26, r5, r26
		subf	r25, r5, r25
		subf	r3,	r23, r3
		subf	r4,	r22, r4
		srwi	r22, r26, 5
		xor	    r21, r29, r28
		xor	    r22, r22, r6
		subf	r4,	r21, r4
		slwi	r20, r29, 4
		subf	r31, r22, r31
		subf	r4,	r20, r4
		xor	    r22, r26, r28
		srwi	r21, r25, 5
		subf	r31, r22, r31
		subf	r4,	r23, r4
		xor	    r22, r21, r6
		slwi	r20, r26, 4
		srwi	r21, r3, 5
		srwi	r19, r4, 5
		subf	r30, r22, r30
		subf	r31, r20, r31
		xor	    r21, r21, r6
		xor	    r22, r25, r28
		xor	    r20, r19, r6
		subf	r31, r23, r31
		subf	r27, r21, r27
		subf	r30, r22, r30
		slwi	r19, r25, 4
		subf	r29, r20, r29
		xor	    r21, r3, r24
		xor	    r22, r4, r24
		srwi	r20, r31, 5
		subf	r30, r19, r30
		subf	r27, r21, r27
		slwi	r18, r3, 4
		xor	    r20, r20, r6
		subf	r29, r22, r29
		slwi	r21, r4, 4
		subf	r30, r23, r30
		subf	r27, r18, r27
		subf	r29, r21, r29
		subf	r26, r20, r26
		xor	    r22, r31, r24
		srwi	r21, r30, 5
		subf	r27, r5, r27
		subf	r26, r22, r26
		xor	    r6,	r21, r6
		slwi	r22, r31, 4
		subf	r29, r5, r29
		srwi	r21, r27, 5
		subf	r26, r22, r26
		srwi	r20, r29, 5
		subf	r6,	r6, r25
		xor	    r22, r21, r11
		xor	    r25, r30, r24
		xor	    r21, r20, r11
		subf	r3,	r22, r3
		subf	r6,	r25, r6
		subf	r26, r5, r26
		xor	    r22, r27, r28
		subf	r4,	r21, r4
		slwi	r25, r30, 4
		xor	    r21, r29, r28
		subf	r3,	r22, r3
		srwi	r20, r26, 5
		subf	r6,	r25, r6
		slwi	r22, r27, 4
		subf	r4,	r21, r4
		slwi	r25, r29, 4
		xor	    r21, r20, r11
		subf	r3,	r22, r3
		subf	r22, r5, r6
		subf	r4,	r25, r4
		xor	    r6,	r26, r28
		subf	r31, r21, r31
		lis	    r25, 0x6A99	//; 0x6A99B4AC
		subf	r3,	r23, r3
		subf	r4,	r23, r4
		subf	r31, r6, r31
		ori	    r6,	r25, 0xB4AC //; 0x6A99B4AC
		slwi	r21, r26, 4
		srwi	r20, r22, 5
		srwi	r25, r3, 5
		srwi	r19, r4, 5
		subf	r31, r21, r31
		xor	    r21, r20, r11
		xor	    r25, r25, r11
		subf	r30, r21, r30
		xor	    r21, r22, r28
		subf	r31, r23, r31
		subf	r30, r21, r30
		xor	    r21, r19, r11
		slwi	r20, r22, 4
		subf	r27, r25, r27
		srwi	r19, r31, 5
		subf	r30, r20, r30
		subf	r29, r21, r29
		xor	    r25, r3, r24
		xor	    r21, r4, r24
		xor	    r20, r19, r11
		subf	r27, r25, r27
		subf	r30, r23, r30
		subf	r29, r21, r29
		slwi	r25, r4, 4
		subf	r26, r20, r26
		xor	    r21, r31, r24
		srwi	r20, r30, 5
		subf	r29, r25, r29
		subf	r26, r21, r26
		slwi	r25, r31, 4
		slwi	r19, r3, 4
		xor	    r11, r20, r11
		subf	r26, r25, r26
		subf	r27, r19, r27
		subf	r11, r11, r22
		xor	    r25, r30, r24
		subf	r27, r5, r27
		subf	r29, r5, r29
		subf	r11, r25, r11
		slwi	r25, r30, 4
		srwi	r22, r27, 5
		srwi	r21, r29, 5
		subf	r26, r5, r26
		subf	r11, r25, r11
		xor	    r22, r22, r6
		xor	    r21, r21, r6
		srwi	r20, r26, 5
		subf	r25, r5, r11
		subf	r3,	r22, r3
		subf	r4,	r21, r4
		xor	    r11, r27, r28
		xor	    r21, r20, r6
		subf	r3,	r11, r3
		subf	r31, r21, r31
		slwi	r11, r27, 4
		xor	    r22, r29, r28
		xor	    r21, r26, r28
		subf	r3,	r11, r3
		srwi	r20, r25, 5
		subf	r11, r21, r31
		subf	r4,	r22, r4
		slwi	r22, r29, 4
		slwi	r31, r26, 4
		xor	    r21, r20, r6
		subf	r11, r31, r11
		subf	r4,	r22, r4
		subf	r30, r21, r30
		xor	    r22, r25, r28
		subf	r3,	r23, r3
		subf	r21, r23, r11
		subf	r11, r22, r30
		srwi	r30, r3, 5
		subf	r4,	r23, r4
		slwi	r31, r25, 4
		lis	    r20, -0x339E //; 0xCC623AF3
		xor	    r30, r30, r6
		subf	r31, r31, r11
		srwi	r22, r4, 5
		srwi	r19, r21, 5
		ori	    r11, r20, 0x3AF3 //; 0xCC623AF3
		subf	r30, r30, r27
		xor	    r22, r22, r6
		subf	r31, r23, r31
		xor	    r20, r19, r6
		xor	    r27, r3, r24
		subf	r30, r27, r30
		slwi	r19, r3, 4
		srwi	r27, r31, 5
		subf	r30, r19, r30
		xor	    r6,	r27, r6
		subf	r29, r22, r29
		xor	    r22, r4, r24
		subf	r27, r20, r26
		subf	r30, r5, r30
		subf	r6,	r6, r25
		xor	    r26, r21, r24
		subf	r29, r22, r29
		xor	    r25, r31, r24
		slwi	r20, r4, 4
		srwi	r22, r30, 5
		subf	r27, r26, r27
		subf	r6,	r25, r6
		slwi	r19, r21, 4
		slwi	r26, r31, 4
		xor	    r25, r22, r11
		subf	r29, r20, r29
		subf	r6,	r26, r6
		subf	r27, r19, r27
		xor	    r26, r30, r28
		subf	r29, r5, r29
		subf	r3,	r25, r3
		subf	r25, r5, r6
		subf	r27, r5, r27
		subf	r3,	r26, r3
		srwi	r6,	r29, 5
		slwi	r26, r30, 4
		srwi	r22, r27, 5
		xor	    r6,	r6, r11
		srwi	r20, r25, 5
		subf	r3,	r26, r3
		xor	    r26, r22, r11
		subf	r6,	r6, r4
		xor	    r22, r20, r11
		xor	    r4,	r29, r28
		subf	r26, r26, r21
		subf	r31, r22, r31
		subf	r3,	r23, r3
		xor	    r21, r27, r28
		subf	r6,	r4, r6
		xor	    r22, r25, r28
		slwi	r4,	r29, 4
		subf	r26, r21, r26
		srwi	r20, r3, 5
		subf	r31, r22, r31
		slwi	r21, r27, 4
		subf	r6,	r4, r6
		slwi	r22, r25, 4
		xor	    r20, r20, r11
		subf	r4,	r21, r26
		subf	r26, r23, r6
		subf	r31, r22, r31
		subf	r30, r20, r30
		xor	    r6,	r3, r24
		subf	r4,	r23, r4
		subf	r31, r23, r31
		subf	r6,	r6, r30
		slwi	r22, r3, 4
		srwi	r30, r26, 5
		srwi	r21, r4, 5
		srwi	r20, r31, 5
		subf	r6,	r22, r6
		xor	    r21, r21, r11
		xor	    r30, r30, r11
		lis	    r22, 0x2E2A	//; 0x2E2AC13A
		xor	    r11, r20, r11
		subf	r19, r5, r6
		subf	r30, r30, r29
		ori	    r6,	r22, 0xC13A //; 0x2E2AC13A
		subf	r27, r21, r27
		subf	r11, r11, r25
		xor	    r29, r26, r24
		xor	    r22, r4, r24
		xor	    r25, r31, r24
		srwi	r21, r19, 5
		slwi	r20, r26, 4
		slwi	r18, r4, 4
		subf	r30, r29, r30
		xor	    r29, r21, r6
		subf	r30, r20, r30
		subf	r3,	r29, r3
		xor	    r29, r19, r28
		subf	r30, r5, r30
		subf	r27, r22, r27
		subf	r3,	r29, r3
		subf	r11, r25, r11
		slwi	r29, r19, 4
		slwi	r17, r31, 4
		srwi	r25, r30, 5
		subf	r27, r18, r27
		subf	r3,	r29, r3
		subf	r11, r17, r11
		xor	    r29, r25, r6
		subf	r27, r5, r27
		subf	r25, r5, r11
		subf	r29, r29, r26
		xor	    r26, r30, r28
		srwi	r11, r27, 5
		subf	r3,	r23, r3
		srwi	r22, r25, 5
		xor	    r11, r11, r6
		subf	r29, r26, r29
		slwi	r26, r30, 4
		srwi	r21, r3, 5
		xor	    r22, r22, r6
		subf	r4,	r11, r4
		xor	    r21, r21, r6
		xor	    r11, r27, r28
		subf	r29, r26, r29
		subf	r31, r22, r31
		xor	    r26, r25, r28
		subf	r22, r21, r19
		subf	r4,	r11, r4
		subf	r29, r23, r29
		subf	r31, r26, r31
		slwi	r11, r27, 4
		xor	    r21, r3, r24
		slwi	r26, r25, 4
		srwi	r20, r29, 5
		subf	r22, r21, r22
		subf	r4,	r11, r4
		subf	r11, r26, r31
		slwi	r21, r3, 4
		xor	    r31, r20, r6
		subf	r26, r21, r22
		subf	r31, r31, r30
		subf	r4,	r23, r4
		xor	    r21, r29, r24
		subf	r22, r23, r11
		subf	r31, r21, r31
		srwi	r30, r4, 5
		subf	r26, r5, r26
		srwi	r21, r22, 5
		lis	    r11, -0x700D //; 0x8FF34781
		slwi	r20, r29, 4
		xor	    r30, r30, r6
		xor	    r6,	r21, r6
		ori	    r11, r11, 0x4781 //; 0x8FF34781
		srwi	r19, r26, 5
		subf	r31, r20, r31
		subf	r30, r30, r27
		xor	    r21, r19, r11
		subf	r6,	r6, r25
		xor	    r27, r4, r24
		subf	r31, r5, r31
		xor	    r25, r22, r24
		subf	r30, r27, r30
		subf	r3,	r21, r3
		xor	    r21, r26, r28
		slwi	r20, r4, 4
		srwi	r27, r31, 5
		slwi	r19, r22, 4
		subf	r6,	r25, r6
		slwi	r18, r26, 4
		subf	r3,	r21, r3
		subf	r30, r20, r30
		xor	    r27, r27, r11
		subf	r6,	r19, r6
		subf	r30, r5, r30
		subf	r29, r27, r29
		xor	    r27, r31, r28
		srwi	r25, r30, 5
		subf	r29, r27, r29
		slwi	r21, r31, 4
		xor	    r27, r25, r11
		subf	r25, r5, r6
		subf	r6,	r21, r29
		subf	r4,	r27, r4
		subf	r3,	r18, r3
		subf	r27, r23, r6
		xor	    r29, r30, r28
		srwi	r6,	r25, 5
		subf	r3,	r23, r3
		subf	r4,	r29, r4
		xor	    r6,	r6, r11
		slwi	r29, r30, 4
		srwi	r21, r3, 5
		subf	r4,	r29, r4
		subf	r6,	r6, r22
		srwi	r20, r27, 5
		xor	    r29, r21, r11
		xor	    r22, r25, r28
		xor	    r21, r20, r11
		subf	r29, r29, r26
		subf	r6,	r22, r6
		subf	r4,	r23, r4
		xor	    r26, r3, r24
		slwi	r22, r25, 4
		subf	r31, r21, r31
		xor	    r21, r27, r24
		srwi	r20, r4, 5
		subf	r29, r26, r29
		subf	r6,	r22, r6
		slwi	r26, r3, 4
		xor	    r22, r20, r11
		subf	r31, r21, r31
		subf	r20, r23, r6
		slwi	r21, r27, 4
		subf	r6,	r26, r29
		subf	r30, r22, r30
		subf	r31, r21, r31
		xor	    r29, r4, r24
		subf	r26, r5, r6
		lis	    r6,	-0xE45 //; 0xF1BBCDC8
		subf	r30, r29, r30
		subf	r31, r5, r31
		srwi	r29, r20, 5
		slwi	r22, r4, 4
		ori	    r6,	r6, 0xCDC8 //; 0xF1BBCDC8
		srwi	r21, r26, 5
		srwi	r19, r31, 5
		xor	    r11, r29, r11
		subf	r30, r22, r30
		xor	    r29, r21, r6
		xor	    r22, r19, r6
		subf	r3,	r29, r3
		subf	r11, r11, r25
		subf	r27, r22, r27
		subf	r30, r5, r30
		xor	    r25, r20, r24
		xor	    r29, r26, r28
		xor	    r22, r31, r28
		subf	r3,	r29, r3
		subf	r11, r25, r11
		srwi	r21, r30, 5
		subf	r29, r22, r27
		slwi	r25, r26, 4
		slwi	r19, r20, 4
		slwi	r18, r31, 4
		xor	    r27, r21, r6
		subf	r3,	r25, r3
		subf	r11, r19, r11
		subf	r29, r18, r29
		subf	r4,	r27, r4
		subf	r11, r5, r11
		xor	    r27, r30, r28
		subf	r3,	r23, r3
		subf	r29, r23, r29
		slwi	r25, r30, 4
		subf	r4,	r27, r4
		srwi	r27, r11, 5
		subf	r4,	r25, r4
		xor	    r27, r27, r6
		xor	    r25, r11, r28
		subf	r27, r27, r20
		subf	r4,	r23, r4
		subf	r27, r25, r27
		slwi	r25, r11, 4
		srwi	r22, r3, 5
		srwi	r21, r29, 5
		subf	r27, r25, r27
		srwi	r20, r4, 5
		xor	    r25, r22, r6
		xor	    r22, r21, r6
		xor	    r21, r20, r6
		subf	r26, r25, r26
		subf	r27, r23, r27
		xor	    r25, r3, r24
		subf	r31, r22, r31
		subf	r30, r21, r30
		xor	    r22, r29, r24
		xor	    r21, r4, r24
		srwi	r20, r27, 5
		subf	r26, r25, r26
		slwi	r25, r3, 4
		subf	r31, r22, r31
		subf	r30, r21, r30
		xor	    r6,	r20, r6
		slwi	r22, r29, 4
		slwi	r21, r4, 4
		subf	r26, r25, r26
		subf	r11, r6, r11
		subf	r31, r22, r31
		subf	r6,	r21, r30
		subf	r30, r5, r26
		xor	    r25, r27, r24
		lis	    r26, 0x5384	//; 0x5384540F
		subf	r31, r5, r31
		subf	r6,	r5, r6
		subf	r25, r25, r11
		ori	    r11, r26, 0x540F //; 0x5384540F
		slwi	r22, r27, 4
		srwi	r21, r31, 5
		srwi	r26, r30, 5
		srwi	r20, r6, 5
		subf	r25, r22, r25
		xor	    r26, r26, r11
		xor	    r22, r21, r11
		xor	    r21, r20, r11
		subf	r3,	r26, r3
		subf	r25, r5, r25
		subf	r29, r22, r29
		subf	r4,	r21, r4
		xor	    r26, r30, r28
		xor	    r22, r31, r28
		xor	    r21, r6, r28
		srwi	r20, r25, 5
		subf	r3,	r26, r3
		subf	r29, r22, r29
		subf	r4,	r21, r4
		slwi	r26, r6, 4
		slwi	r19, r30, 4
		slwi	r18, r31, 4
		xor	    r22, r20, r11
		subf	r4,	r26, r4
		subf	r3,	r19, r3
		subf	r29, r18, r29
		subf	r27, r22, r27
		xor	    r26, r25, r28
		subf	r3,	r23, r3
		subf	r29, r23, r29
		subf	r27, r26, r27
		slwi	r26, r25, 4
		subf	r4,	r23, r4
		srwi	r22, r3, 5
		srwi	r21, r29, 5
		subf	r27, r26, r27
		srwi	r26, r4, 5
		xor	    r22, r22, r11
		xor	    r21, r21, r11
		subf	r27, r23, r27
		subf	r30, r22, r30
		xor	    r22, r3, r24
		srwi	r20, r27, 5
		xor	    r26, r26, r11
		subf	r30, r22, r30
		xor	    r11, r20, r11
		slwi	r22, r3, 4
		subf	r11, r11, r25
		subf	r30, r22, r30
		subf	r31, r21, r31
		xor	    r22, r29, r24
		xor	    r25, r27, r24
		subf	r26, r26, r6
		subf	r30, r5, r30
		xor	    r21, r4, r24
		lis	    r6,	-0x4AB4	//; 0xB54CDA56
		subf	r11, r25, r11
		subf	r31, r22, r31
		slwi	r22, r29, 4
		slwi	r25, r27, 4
		ori	    r6,	r6, 0xDA56 //; 0xB54CDA56
		subf	r26, r21, r26
		srwi	r20, r30, 5
		slwi	r21, r4, 4
		subf	r11, r25, r11
		subf	r31, r22, r31
		xor	    r22, r20, r6
		subf	r26, r21, r26
		subf	r25, r5, r11
		subf	r3,	r22, r3
		xor	    r11, r30, r28
		subf	r31, r5, r31
		subf	r26, r5, r26
		subf	r11, r11, r3
		slwi	r22, r30, 4
		srwi	r21, r26, 5
		srwi	r3,	r31, 5
		srwi	r20, r25, 5
		subf	r11, r22, r11
		xor	    r3,	r3, r6
		xor	    r22, r21, r6
		xor	    r21, r20, r6
		subf	r20, r23, r11
		subf	r11, r3, r29
		xor	    r3,	r31, r28
		subf	r4,	r22, r4
		xor	    r29, r26, r28
		subf	r27, r21, r27
		xor	    r22, r25, r28
		subf	r3,	r3, r11
		srwi	r21, r20, 5
		subf	r4,	r29, r4
		slwi	r19, r31, 4
		slwi	r11, r26, 4
		subf	r29, r22, r27
		slwi	r18, r25, 4
		xor	    r27, r21, r6
		subf	r11, r11, r4
		subf	r3,	r19, r3
		subf	r4,	r18, r29
		subf	r30, r27, r30
		xor	    r29, r20, r24
		subf	r3,	r23, r3
		subf	r27, r23, r11
		subf	r11, r29, r30
		srwi	r29, r3, 5
		subf	r4,	r23, r4
		slwi	r30, r20, 4
		srwi	r22, r27, 5
		xor	    r29, r29, r6
		subf	r30, r30, r11
		xor	    r22, r22, r6
		srwi	r21, r4, 5
		lis	    r19, 0x1715	//; 0x1715609D
		subf	r31, r29, r31
		xor	    r6,	r21, r6
		ori	    r11, r19, 0x609D //; 0x1715609D
		subf	r30, r5, r30
		xor	    r29, r3, r24
		subf	r26, r22, r26
		subf	r31, r29, r31
		slwi	r29, r3, 4
		subf	r6,	r6, r25
		srwi	r25, r30, 5
		subf	r31, r29, r31
		xor	    r29, r25, r11
		subf	r31, r5, r31
		xor	    r25, r27, r24
		subf	r29, r29, r20
		xor	    r22, r4, r24
		srwi	r20, r31, 5
		subf	r26, r25, r26
		subf	r6,	r22, r6
		slwi	r25, r27, 4
		xor	    r22, r20, r11
		xor	    r21, r30, r28
		slwi	r20, r4, 4
		subf	r26, r25, r26
		subf	r29, r21, r29
		slwi	r25, r30, 4
		subf	r6,	r20, r6
		subf	r3,	r22, r3
		xor	    r21, r31, r28
		subf	r29, r25, r29
		subf	r25, r5, r6
		subf	r26, r5, r26
		subf	r3,	r21, r3
		slwi	r6,	r31, 4
		subf	r29, r23, r29
		srwi	r22, r26, 5
		srwi	r21, r25, 5
		subf	r6,	r6, r3
		srwi	r20, r29, 5
		xor	    r22, r22, r11
		xor	    r3,	r21, r11
		subf	r21, r23, r6
		xor	    r6,	r20, r11
		subf	r27, r22, r27
		subf	r4,	r3, r4
		xor	    r22, r26, r28
		subf	r6,	r6, r30
		xor	    r3,	r25, r28
		xor	    r30, r29, r24
		subf	r27, r22, r27
		slwi	r19, r26, 4
		subf	r4,	r3, r4
		srwi	r20, r21, 5
		subf	r3,	r30, r6
		subf	r6,	r19, r27
		xor	    r22, r20, r11
		slwi	r20, r25, 4
		subf	r30, r23, r6
		slwi	r18, r29, 4
		subf	r31, r22, r31
		xor	    r6,	r21, r24
		subf	r4,	r20, r4
		subf	r3,	r18, r3
		subf	r31, r6, r31
		lis	    r27, 0x78DD	//; 0x78DDE6E4
		slwi	r22, r21, 4
		subf	r4,	r23, r4
		srwi	r20, r30, 5
		subf	r3,	r5, r3
		ori	    r6,	r27, 0xE6E4 //; 0x78DDE6E4
		subf	r31, r22, r31
		xor	    r27, r20, r11
		srwi	r22, r4, 5
		srwi	r20, r3, 5
		xor	    r11, r22, r11
		subf	r27, r27, r26
		xor	    r22, r20, r6
		xor	    r26, r30, r24
		subf	r31, r5, r31
		subf	r29, r22, r29
		subf	r27, r26, r27
		lis	    r20, -0x255A //; 0xDAA66D2B
		subf	r25, r11, r25
		xor	    r22, r4, r24
		xor	    r19, r3, r28
		srwi	r18, r31, 5
		slwi	r26, r30, 4
		subf	r27, r26, r27
		subf	r26, r22, r25
		subf	r27, r5, r27
		slwi	r25, r4, 4
		srwi	r22, r27, 5
		subf	r26, r25, r26
		xor	    r25, r18, r6
		xor	    r22, r22, r6
		subf	r26, r5, r26
		subf	r25, r25, r21
		ori	    r11, r20, 0x6D2B //; 0xDAA66D2B
		subf	r30, r22, r30
		subf	r29, r19, r29
		xor	    r21, r31, r28
		slwi	r20, r3, 4
		xor	    r19, r27, r28
		srwi	r22, r26, 5
		subf	r25, r21, r25
		subf	r29, r20, r29
		xor	    r22, r22, r6
		slwi	r21, r31, 4
		slwi	r20, r27, 4
		subf	r30, r19, r30
		subf	r29, r23, r29
		subf	r4,	r22, r4
		subf	r25, r21, r25
		subf	r30, r20, r30
		xor	    r22, r26, r28
		srwi	r21, r29, 5
		subf	r25, r23, r25
		subf	r30, r23, r30
		subf	r4,	r22, r4
		slwi	r20, r26, 4
		xor	    r22, r21, r6
		srwi	r19, r30, 5
		srwi	r21, r25, 5
		subf	r4,	r20, r4
		subf	r3,	r22, r3
		xor	    r21, r21, r6
		xor	    r20, r19, r6
		xor	    r22, r29, r24
		subf	r4,	r23, r4
		subf	r31, r21, r31
		subf	r3,	r22, r3
		subf	r27, r20, r27
		xor	    r21, r25, r24
		xor	    r22, r30, r24
		srwi	r20, r4, 5
		subf	r27, r22, r27
		xor	    r6,	r20, r6
		subf	r31, r21, r31
		slwi	r18, r25, 4
		slwi	r19, r29, 4
		slwi	r22, r30, 4
		subf	r6,	r6, r26
		subf	r31, r18, r31
		subf	r27, r22, r27
		subf	r3,	r19, r3
		xor	    r26, r4, r24
		subf	r31, r5, r31
		subf	r6,	r26, r6
		subf	r3,	r5, r3
		subf	r27, r5, r27
		slwi	r26, r4, 4
		srwi	r21, r31, 5
		srwi	r22, r3, 5
		srwi	r20, r27, 5
		subf	r6,	r26, r6
		xor	    r26, r21, r11
		xor	    r22, r22, r11
		lis	    r21, 0x3C6E	//; 0x3C6EF372
		xor	    r20, r20, r11
		subf	r19, r5, r6
		subf	r29, r22, r29
		subf	r26, r26, r25
		ori	    r6,	r21, 0xF372 //; 0x3C6EF372
		subf	r30, r20, r30
		xor	    r22, r3, r28
		xor	    r25, r31, r28
		xor	    r21, r27, r28
		srwi	r20, r19, 5
		subf	r29, r22, r29
		slwi	r22, r3, 4
		subf	r26, r25, r26
		xor	    r20, r20, r11
		subf	r29, r22, r29
		slwi	r25, r31, 4
		subf	r30, r21, r30
		subf	r4,	r20, r4
		slwi	r22, r27, 4
		xor	    r21, r19, r28
		subf	r26, r25, r26
		subf	r29, r23, r29
		subf	r4,	r21, r4
		subf	r30, r22, r30
		slwi	r25, r19, 4
		subf	r26, r23, r26
		srwi	r22, r29, 5
		subf	r4,	r25, r4
		xor	    r22, r22, r11
		srwi	r25, r26, 5
		subf	r30, r23, r30
		subf	r4,	r23, r4
		xor	    r25, r25, r11
		subf	r3,	r22, r3
		xor	    r22, r29, r24
		srwi	r21, r30, 5
		srwi	r20, r4, 5
		subf	r31, r25, r31
		xor	    r21, r21, r11
		slwi	r18, r29, 4
		subf	r3,	r22, r3
		xor	    r25, r26, r24
		xor	    r11, r20, r11
		subf	r31, r25, r31
		subf	r3,	r18, r3
		subf	r11, r11, r19
		xor	    r25, r4, r24
		subf	r27, r21, r27
		xor	    r22, r30, r24
		subf	r3,	r5, r3
		subf	r11, r25, r11
		slwi	r21, r26, 4
		subf	r27, r22, r27
		slwi	r25, r4, 4
		slwi	r20, r30, 4
		srwi	r22, r3, 5
		subf	r31, r21, r31
		subf	r11, r25, r11
		subf	r27, r20, r27
		xor	    r25, r22, r6
		subf	r31, r5, r31
		subf	r22, r5, r11
		subf	r29, r25, r29
		subf	r27, r5, r27
		xor	    r25, r3, r28
		srwi	r11, r31, 5
		srwi	r21, r27, 5
		srwi	r20, r22, 5
		subf	r29, r25, r29
		xor	    r11, r11, r6
		slwi	r25, r3, 4
		xor	    r21, r21, r6
		xor	    r20, r20, r6
		subf	r11, r11, r26
		subf	r29, r25, r29
		xor	    r26, r31, r28
		subf	r30, r21, r30
		subf	r4,	r20, r4
		xor	    r21, r27, r28
		lis	    r25, -0x61C9 //; 0x9E3779B9
		xor	    r20, r22, r28
		subf	r26, r26, r11
		subf	r29, r23, r29
		slwi	r19, r31, 4
		subf	r30, r21, r30
		ori	    r11, r25, 0x79B9 //; 0x9E3779B9
		subf	r4,	r20, r4
		slwi	r21, r27, 4
		srwi	r25, r29, 5
		slwi	r20, r22, 4
		subf	r26, r19, r26
		subf	r30, r21, r30
		subf	r4,	r20, r4
		subf	r26, r23, r26
		subf	r30, r23, r30
		subf	r4,	r23, r4
		srwi	r21, r26, 5
		srwi	r20, r30, 5
		xor	    r25, r25, r6
		srwi	r19, r4, 5
		xor	    r21, r21, r6
		xor	    r20, r20, r6
		subf	r3,	r25, r3
		xor	    r18, r29, r24
		xor	    r6,	r19, r6
		subf	r31, r21, r31
		xor	    r25, r26, r24
		subf	r27, r20, r27
		xor	    r21, r30, r24
		subf	r6,	r6, r22
		slwi	r20, r29, 4
		subf	r3,	r18, r3
		subf	r31, r25, r31
		xor	    r22, r4, r24
		slwi	r19, r26, 4
		subf	r27, r21, r27
		slwi	r18, r30, 4
		subf	r3,	r20, r3
		subf	r6,	r22, r6
		slwi	r25, r4, 4
		subf	r31, r19, r31
		subf	r27, r18, r27
		subf	r3,	r5, r3
		subf	r6,	r25, r6
		subf	r31, r5, r31
		subf	r27, r5, r27
		srwi	r25, r3, 5
		subf	r6,	r5, r6
		srwi	r22, r31, 5
		srwi	r21, r27, 5
		xor	    r25, r25, r11
		xor	    r22, r22, r11
		srwi	r20, r6, 5
		xor	    r21, r21, r11
		subf	r29, r25, r29
		xor	    r25, r3, r28
		subf	r26, r22, r26
		xor	    r20, r20, r11
		xor	    r22, r31, r28
		subf	r30, r21, r30
		xor	    r21, r27, r28
		subf	r29, r25, r29
		xor	    r28, r6, r28
		subf	r4,	r20, r4
		subf	r26, r22, r26
		slwi	r25, r31, 4
		slwi	r19, r3, 4
		slwi	r22, r27, 4
		subf	r30, r21, r30
		subf	r4,	r28, r4
		subf	r26, r25, r26
		slwi	r28, r6, 4
		subf	r29, r19, r29
		subf	r30, r22, r30
		subf	r29, r23, r29
		subf	r26, r23, r26
		subf	r4,	r28, r4
		subf	r30, r23, r30
		subf	r4,	r23, r4
		srwi	r28, r29, 5
		srwi	r25, r26, 5
		srwi	r23, r30, 5
		xor	    r28, r28, r11
		xor	    r25, r25, r11
		xor	    r23, r23, r11
		srwi	r22, r4, 5
		subf	r3,	r28, r3
		subf	r31, r25, r31
		xor	    r11, r22, r11
		xor	    r28, r29, r24
		xor	    r25, r26, r24
		subf	r27, r23, r27
		subf	r11, r11, r6
		xor	    r6,	r4, r24
		subf	r3,	r28, r3
		xor	    r23, r30, r24
		subf	r31, r25, r31
		slwi	r28, r29, 4
		subf	r11, r6, r11
		slwi	r25, r26, 4
		slwi	r6,	r4, 4
		subf	r27, r23, r27
		slwi	r24, r30, 4
		subf	r3,	r28, r3
		subf	r31, r25, r31
		subf	r11, r6, r11
		subf	r28, r24, r27
		subf	r6,	r5, r3
		subf	r3,	r5, r31
		subf	r11, r5, r11
		subf	r31, r5, r28
		clrldi  r5,	r3, 32
		clrldi  r6,	r6, 32
		clrldi  r3,	r31, 32
		clrldi  r11, r11, 32
		insrdi  r6,	r29, 32,0
		insrdi  r5,	r26, 32,0
		insrdi  r3,	r30, 32,0
		std	    r6,	0(r7)
		insrdi  r11, r4, 32,0
		std	    r5,	0(r8)
		std	    r3,	0(r9)
		std	    r11, 0(r10)
		bl	    _restgprlr_17
		blr
	}
#undef arg_50
#undef arg_58
}

static __declspec(naked) void EraDecyptBlock()
{
#define var_E0 -0xE0
#define var_D8 -0xD8
#define var_D0 -0xD0
#define var_C8 -0xC8
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

	__asm {
		mfspr	r12, LR
		bl	    _savegprlr_20
		stwu	sp,	-0x130(sp)
		mr	    r11, r7
		mr	    r27, r6
		mr	    r29, r4
		std	    r29, 0x130+var_E0(sp)
		mr	    r28, r5
		std	    r28, 0x130+var_D8(sp)
		mr	    r31, r8
		ld	    r30, 0x20(r11)
		mr	    r26, r9
		ld	    r7,	0x38(r11)
		addi	r10, sp, 0x98//0x130+var_98
		ld	    r6,	0x30(r11)
		addi	r9,	sp, 0x90//0x130+var_A0
		xor	    r25, r7, r30			//; xor r25, 0x38, 0x20
		ld	    r24, 0x28(r11)
		ld	    r3,	0(r11)
		addi	r8,	sp, 0x88//0x130+var_A8
		xor	    r23, r6, r25			//; xor r23, 0x30, r25
		ld	    r4,	8(r11)
		ld	    r5,	0x10(r11)
		addi	r7,	sp, 0x80//0x130+var_B0
		xor	    r24, r24, r23			//; xor r24, 0x28, r23
		ld	    r6,	0x18(r11)
		xor	    r11, r24, r30			//; xor r11, r24, 0x20
		std	    r3,	0x130+var_D0(sp)
		std	    r4,	0x130+var_C8(sp)
		xor	    r30, r25, r11			//; r30, r25, r11
		std	    r5,	0x130+var_C0(sp)
		std	    r6,	0x130+var_B8(sp)
		xor	    r25, r30, r23
		xor	    r24, r25, r24
		xor	    r11, r24, r11
		xor	    r23, r30, r11
		xor	    r25, r23, r25
		xor	    r30, r25, r24
		xor	    r24, r30, r11
		bl	    EraProcessAndXorBlockDecypt
		xor	    r6,	r23, r24
		std	    r27, 0x130+var_D8(sp)
		addi	r10, sp, 0xB8//0x130+var_78
		std	    r28, 0x130+var_E0(sp)
		xor	    r5,	r6, r25
		addi	r9,	sp, 0xB0//0x130+var_80
		xor	    r4,	r5, r30
		addi	r8,	sp, 0xA8//0x130+var_88
		xor	    r3,	r4, r24
		addi	r7,	sp, 0xA0//0x130+var_90
		xor	    r11, r6, r3
		xor	    r6,	r11, r5
		xor	    r5,	r6, r4
		xor	    r4,	r5, r3
		xor	    r3,	r11, r4
		xor	    r11, r3, r6
		ld	    r27, 0x130+var_B0(sp)
		mr	    r6,	r3
		xor	    r22, r11, r5
		ld	    r21, 0x130+var_A8(sp)
		mr	    r5,	r11
		ld	    r11, 0x130+var_98(sp)
		ld	    r20, 0x130+var_A0(sp)
		xor	    r3,	r22, r4
		mr	    r4,	r22
		subf	r23, r23, r27
		xor	    r25, r21, r25
		add	    r30, r20, r30
		xor	    r27, r11, r24
		bl	    EraProcessAndXorBlockDecypt
		ld	    r4,	0x130+var_90(sp)
		xor	    r6,	r23, r27
		ld	    r3,	0x130+var_88(sp)
		addi	r10, sp, 0x78//0x130+var_B8
		std	    r29, 0x130+var_D8(sp)
		xor	    r5,	r30, r6
		xor	    r29, r4, r27
		ld	    r11, 0x130+var_80(sp)
		std	    r28, 0x130+var_E0(sp)
		addi	r9,	sp, 0x70//0x130+var_C0
		addi	r8,	sp, 0x68//0x130+var_C8
		addi	r7,	sp, 0x60//0x130+var_D0
		add	    r30, r3, r30
		ld	    r4,	0x130+var_78(sp)
		xor	    r3,	r25, r5
		xor	    r28, r11, r25
		xor	    r11, r23, r3
		subf	r27, r23, r4
		xor	    r6,	r11, r6
		xor	    r5,	r5, r6
		xor	    r4,	r3, r5
		xor	    r3,	r11, r4
		xor	    r6,	r3, r6
		xor	    r5,	r5, r6
		xor	    r4,	r4, r5
		xor	    r3,	r3, r4
		bl	    EraProcessAndXorBlockDecypt
		addis	r9,	r26, -0x4333
		addic.  r9,	r9, 0x923
		bne	    loc_8265E10C
		addi	r9,	r9, 1

loc_8265E10C:
		slwi	r11, r9, 17
		ld	    r8,	0x130+var_D0(sp)
		lis	    r7,	0x3424 //; 0x34248FE2
		ld	    r6,	0x130+var_C8(sp)
		xor	    r5,	r11, r9
		ld	    r4,	0x130+var_C0(sp)
		ori	    r10, r7, 0x8FE2 //; 0x34248FE2
		ld	    r9,	0x130+var_B8(sp)
		srwi	r11, r5, 13
		lis	    r3,	0x15EF //; 0x15EF0AF3
		xor	    r7,	r11, r5
		ori	    r5,	r3, 0xAF3 //; 0x15EF0AF3
		slwi	r3,	r7, 5
		insrdi  r10, r5, 32,0
		xor	    r7,	r3, r7
		lis	    r11, -0x3425 //; 0xCBDB701E
		slwi	r26, r7, 17
		lis	    r3,	-0x15F0	//; 0xEA10F50C
		xor	    r5,	r26, r7
		ori	    r3,	r3, 0xF50C //; 0xEA10F50C
		srwi	r26, r5, 13
		ori	    r11, r11, 0x701E //; 0xCBDB701E
		xor	    r5,	r26, r5
		insrdi  r11, r3, 32,0
		slwi	r26, r5, 5
		clrldi  r7,	r7, 32
		xor	    r3,	r26, r5
		add	    r7,	r7, r10
		slwi	r5,	r3, 17
		xor	    r8,	r7, r8
		xor	    r5,	r5, r3
		clrldi  r3,	r3, 32
		std	    r8,	0(r31)
		srwi	r26, r5, 13
		add	    r3,	r3, r11
		xor	    r5,	r26, r5
		slwi	r7,	r5, 5
		xor	    r8,	r7, r5
		xor	    r7,	r3, r6
		slwi	r6,	r8, 17
		clrldi  r5,	r8, 32
		std	    r7,	8(r31)
		xor	    r3,	r6, r8
		add	    r8,	r5, r10
		srwi	r6,	r3, 13
		xor	    r5,	r8, r4
		xor	    r4,	r6, r3
		slwi	r3,	r4, 5
		xor	    r8,	r3, r4
		slwi	r6,	r8, 17
		clrldi  r4,	r8, 32
		xor	    r3,	r6, r8
		add	    r8,	r4, r11
		srwi	r6,	r3, 13
		xor	    r4,	r8, r9
		xor	    r3,	r6, r3
		slwi	r9,	r3, 5
		xor	    r8,	r9, r3
		slwi	r6,	r8, 17
		clrldi  r3,	r8, 32
		xor	    r9,	r6, r8
		add	    r8,	r3, r10
		srwi	r6,	r9, 13
		xor	    r3,	r8, r29
		xor	    r9,	r6, r9
		slwi	r8,	r9, 5
		xor	    r6,	r8, r9
		slwi	r9,	r6, 17
		clrldi  r8,	r6, 32
		xor	    r6,	r9, r6
		add	    r9,	r8, r11
		srwi	r8,	r6, 13
		xor	    r9,	r9, r30
		xor	    r8,	r8, r6
		slwi	r6,	r8, 5
		xor	    r8,	r6, r8
		slwi	r6,	r8, 17
		clrldi  r30, r8, 32
		xor	    r8,	r6, r8
		add	    r6,	r30, r10
		srwi	r10, r8, 13
		xor	    r7,	r6, r28
		std	    r5,	0x10(r31)
		xor	    r6,	r10, r8
		std	    r4,	0x18(r31)
		std	    r3,	0x20(r31)
		slwi	r5,	r6, 5
		std	    r9,	0x28(r31)
		clrldi  r4,	r6, 32
		std	    r7,	0x30(r31)
		xor	    r3,	r5, r4
		add	    r11, r3, r11
		xor	    r10, r11, r27
		std	    r10, 0x38(r31)
		addi	sp,	sp, 0x130
		bl	    _restgprlr_20
		blr
	}
#undef var_E0
#undef var_D8
#undef var_D0
#undef var_C8
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
}

//										r3													 r4			 r5			  r6				  r7
static __declspec(naked) void EraDecypt(const Phoenix::s_ensemble_cryptor_context* ecf_data, void* addr, void* addr2, size_t buffer_size, __int32)
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
		bl	    EraDecyptBlock
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