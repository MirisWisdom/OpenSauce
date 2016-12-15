#include "Precompile.hpp"
#include <xbox/xenon/GPRLR.hpp>

extern "C" {
	__declspec(naked) void _savegprlr_26()
	{
		__asm {
			std       r26, -0x38(r1)
			std       r27, -0x30(r1)
			std       r28, -0x28(r1)
			std       r29, -0x20(r1)
			std       r30, -0x18(r1)
			std       r31, -0x10(r1)
			stw       r12, -8(r1)
			blr
		}
	}
	__declspec(naked) void _restgprlr_26()
	{
		__asm {
			ld        r26, -0x38(r1)
			ld        r27, -0x30(r1)
			ld        r28, -0x28(r1)
			ld        r29, -0x20(r1)
			ld        r30, -0x18(r1)
			ld        r31, -0x10(r1)
			lwz       r12, -8(r1)
			mtspr   LR, r12
			blr
		}
	}

	__declspec(naked) void _savegprlr_20()
	{
		__asm {
			std       r20, -0x68(r1)
			std       r21, -0x60(r1)
			std       r22, -0x58(r1)
			std       r23, -0x50(r1)
			std       r24, -0x48(r1)
			std       r25, -0x40(r1)
			std       r26, -0x38(r1)
			std       r27, -0x30(r1)
			std       r28, -0x28(r1)
			std       r29, -0x20(r1)
			std       r30, -0x18(r1)
			std       r31, -0x10(r1)
			stw       r12, -8(r1)
			blr
		}
	}
	__declspec(naked) void _restgprlr_20()
	{
		__asm {
			ld        r20, -0x68(r1)
			ld        r21, -0x60(r1)
			ld        r22, -0x58(r1)
			ld        r23, -0x50(r1)
			ld        r24, -0x48(r1)
			ld        r25, -0x40(r1)
			ld        r26, -0x38(r1)
			ld        r27, -0x30(r1)
			ld        r28, -0x28(r1)
			ld        r29, -0x20(r1)
			ld        r30, -0x18(r1)
			ld        r31, -0x10(r1)
			lwz       r12, -8(r1)
			mtspr   LR, r12
			blr
		}
	}

	__declspec(naked) void _savegprlr_17()
	{
		__asm {
			std       r17, -0x80(r1)
			std       r18, -0x78(r1)
			std       r19, -0x70(r1)
			std       r20, -0x68(r1)
			std       r21, -0x60(r1)
			std       r22, -0x58(r1)
			std       r23, -0x50(r1)
			std       r24, -0x48(r1)
			std       r25, -0x40(r1)
			std       r26, -0x38(r1)
			std       r27, -0x30(r1)
			std       r28, -0x28(r1)
			std       r29, -0x20(r1)
			std       r30, -0x18(r1)
			std       r31, -0x10(r1)
			stw       r12, -8(r1)
			blr
		}
	}
	__declspec(naked) void _restgprlr_17()
	{
		__asm {
			ld        r17, -0x80(r1)
			ld        r18, -0x78(r1)
			ld        r19, -0x70(r1)
			ld        r20, -0x68(r1)
			ld        r21, -0x60(r1)
			ld        r22, -0x58(r1)
			ld        r23, -0x50(r1)
			ld        r24, -0x48(r1)
			ld        r25, -0x40(r1)
			ld        r26, -0x38(r1)
			ld        r27, -0x30(r1)
			ld        r28, -0x28(r1)
			ld        r29, -0x20(r1)
			ld        r30, -0x18(r1)
			ld        r31, -0x10(r1)
			lwz       r12, -8(r1)
			mtspr   LR, r12
			blr
		}
	}

	__declspec(naked) void _savegprlr_14()
	{
		__asm {
			std       r14, -0x98(r1)
			std       r15, -0x90(r1)
			std       r16, -0x88(r1)
			std       r17, -0x80(r1)
			std       r18, -0x78(r1)
			std       r19, -0x70(r1)
			std       r20, -0x68(r1)
			std       r21, -0x60(r1)
			std       r22, -0x58(r1)
			std       r23, -0x50(r1)
			std       r24, -0x48(r1)
			std       r25, -0x40(r1)
			std       r26, -0x38(r1)
			std       r27, -0x30(r1)
			std       r28, -0x28(r1)
			std       r29, -0x20(r1)
			std       r30, -0x18(r1)
			std       r31, -0x10(r1)
			stw       r12, -8(r1)
			blr
		}
	}
	__declspec(naked) void _restgprlr_14()
	{
		__asm {
			ld        r14, -0x98(r1)
			ld        r15, -0x90(r1)
			ld        r16, -0x88(r1)
			ld        r17, -0x80(r1)
			ld        r18, -0x78(r1)
			ld        r19, -0x70(r1)
			ld        r20, -0x68(r1)
			ld        r21, -0x60(r1)
			ld        r22, -0x58(r1)
			ld        r23, -0x50(r1)
			ld        r24, -0x48(r1)
			ld        r25, -0x40(r1)
			ld        r26, -0x38(r1)
			ld        r27, -0x30(r1)
			ld        r28, -0x28(r1)
			ld        r29, -0x20(r1)
			ld        r30, -0x18(r1)
			ld        r31, -0x10(r1)
			lwz       r12, -8(r1)
			mtspr   LR, r12
			blr
		}
	}
};