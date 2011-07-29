/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

#define YELO_MEM_WRITE(func, ...) \
	{ \
		CR0_BEGIN(); \
		Yelo::Memory::func(__VA_ARGS__); \
		CR0_END(); \
	};

// begin a 'list' of memory writes
#define YELO_MEM_WLIST_BEGIN() \
	{ \
		CR0_BEGIN();

// perform a memory write with supplied arguments
#define YELO_MEM_WLIST_ITEM(func, ...) Yelo::Memory::func(__VA_ARGS__)

// end a pre-existing 'list' of memory writes
#define YELO_MEM_WLIST_END() \
		CR0_END(); \
	};

namespace Yelo
{
	namespace Enums
	{
		enum opcode
		{
			// push imm16\32
			_opcode_push = 0x68,

			// return near imm16\32
			_opcode_retn = 0xC2,

			// regular return near
			_opcode_ret = 0xC3,

			// call near imm16\32
			_opcode_call_near = 0xE8,

			// jmp near imm16\32
			_opcode_jmp_near = 0xE9,

			// call ds:[address]
			_opcode_call_abs = 0x15FF, /*FF 15*/

			// call ds:[address]
			_opcode_jmp_abs = 0x25FF, /*FF 25*/
		};
	};

	namespace Memory
	{
		struct Opcode
		{
			struct Call
			{
				byte Op;
				uint32 Address;
			};

			struct CallRet
			{
				byte Op;
				uint32 Address;
				byte Ret;
				uint16 Count;
			};

			// absolute call opcode
			struct CallAbs
			{
				uint16 Op;
				uint32 Address;
			};

			// absolute call opcode with return
			struct CallAbsRet
			{
				uint16 Op;
				uint32 Address;
				byte Ret;
				uint16 Count;
			};
		};


		// [call] buffer containing the data we wish to write
		// [address] address to put [call]
		void Write(Opcode::Call& call, void* address);

		// [call_buffer] is a buffer to receive the old bytes
		// [address] address to put\overwrite a call
		// [target] address to make the call goto
		void WriteCall(void* call_buffer, void* address, void* target);

		// [call_ret_buffer] is a buffer to receive the old bytes
		// [address] address to put\overwrite a call and ret
		// [target] address to make the call goto
		void WriteCallRet(void* call_ret_buffer, void* address, void* target);

		// [call_ret_buffer] is a buffer to receive the old bytes
		// [address] address to put\overwrite a call and ret
		// [target] address to make the call goto
		// [count] number of args in the function we're modding has. If there are any 
		// 64-bit arguments, count them twice!
		void WriteCallRet(void* call_ret_buffer, void* address, void* target, uint16 count);

		// [call] buffer containing the data we wish to write
		// [address] address to put [call]
		void WriteRet(Opcode::CallRet& call, void* address);

		// [call] buffer containing the data we wish to write
		// [address] address to put [call]
		void WriteRetn(Opcode::CallRet& call, void* address);

		// [jmp_buffer] is a buffer to receive the old bytes
		// [address] address to put\overwrite a jmp
		// [target] address to make the jmp goto
		void WriteJmp(void* jmp_buffer, void* address, void* target);

		// [jmp_buffer] is a buffer to receive the old jmp address
		// [address] address to overwrite a jmp
		// [target] address to make the jmp goto
		// REMARKS:
		// Jmp type can be anything as long as the address used is 32bits
		void OverwriteJmp(void* jmp_buffer, void* address, void* target);

		// [address] address to perform the store-push-ret code at
		// [target] address to make the store-push-ret goto
		// REMARKS:
		// Ported for legacy code. USE IS HIGHLY DISCOURAGED!
		void StorePushRet(void* address, void* target);
	};
};