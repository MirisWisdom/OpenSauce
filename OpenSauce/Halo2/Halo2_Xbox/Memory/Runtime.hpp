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
		void WriteCall(void* call_buffer, void* address, const void* target);

		// [call_ret_buffer] is a buffer to receive the old bytes
		// [address] address to put\overwrite a call and ret
		// [target] address to make the call goto
		void WriteCallRet(void* call_ret_buffer, void* address, const void* target);

		// [call_ret_buffer] is a buffer to receive the old bytes
		// [address] address to put\overwrite a call and ret
		// [target] address to make the call goto
		// [count] number of 32-bit args in the function we're modding. If there are any 
		// 64-bit arguments, count them twice!
		void WriteCallRet(void* call_ret_buffer, void* address, const void* target, const uint16 count);

		// [call] buffer containing the data we wish to write
		// [address] address to put [call]
		void WriteRet(Opcode::CallRet& call, void* address);

		// [call] buffer containing the data we wish to write
		// [address] address to put [call]
		void WriteRetn(Opcode::CallRet& call, void* address);

		// [jmp_buffer] is a buffer to receive the old bytes
		// [address] address to put\overwrite a jmp
		// [target] address to make the jmp goto
		void WriteJmp(void* jmp_buffer, void* address, const void* target);

		// [jmp_buffer] is a buffer to receive the old jmp address
		// [address] address to overwrite a jmp
		// [target] address to make the jmp goto
		// REMARKS:
		// Jmp type can be anything as long as the address used is 32bits
		void OverwriteJmp(void* jmp_buffer, void* address, const void* target);

#if FALSE // Can't touch this, ha!
		// [address] address to perform the store-push-ret code at
		// [target] address to make the store-push-ret goto
		// REMARKS:
		// Ported for legacy code. USE IS HIGHLY DISCOURAGED!
		void StorePushRet(void* address, void* target);
#endif

		// Overwrite the memory at [address] with [array]
		// Copies the original memory at [address] into [array] before returning
		// This is useful when writing an array of opaque bytes to game memory
		template<typename T, size_t TSizeOfArray>
		void OverwriteMemory(void* address, T(& array)[TSizeOfArray])
		{
			T old_memory[TSizeOfArray];
			// Copy the old memory from the address
			memcpy(old_memory, address, TSizeOfArray);
			// Write the new memory to the address
			memcpy(address, array, TSizeOfArray);
			// Fill [array] with the old memory
			memcpy(array, old_memory, TSizeOfArray);
		}
		// Overwrite the memory at [address] with [array]
		// Does NOT copy the original memory at [address] into [array] before returning
		// This is useful when writing an array of opaque bytes to game memory, esp. during unwinding operations
		template<typename T, size_t TSizeOfArray>
		void OverwriteMemorySansCopy(void* address, const T(& array)[TSizeOfArray])
		{
			// Write the new memory to the address
			memcpy(address, array, TSizeOfArray);
		}
		// Overwrite the memory at [address] with [data]
		// Copies the original memory at [address] into [data] before returning
		// This is useful when writing a concrete object to game memory
		template<typename T>
		void OverwriteMemory(void* address, T& data)
		{
			T old_memory;
			// Copy the old memory from the address
			memcpy(&old_memory, address, sizeof(T));
			// Write the new memory to the address
			memcpy(address, &data, sizeof(T));
			// Fill [data] with the old memory
			memcpy(&data, &old_memory, sizeof(T));
		}
		// Overwrite the memory at [address] with [data]
		// Does NOT copy the original memory at [address] into [data] before returning
		// This is useful when writing a concrete object to game memory, esp. during unwinding operations
		template<typename T>
		void OverwriteMemorySansCopy(void* address, const T& data)
		{
			// Write the new memory to the address
			memcpy(address, &data, sizeof(T));
		}
	};
};