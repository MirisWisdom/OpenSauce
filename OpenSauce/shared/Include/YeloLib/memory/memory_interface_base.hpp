/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

//////////////////////////////////////////////////////////////////////////
// Engine pointer markup system
//
//-Reference based interface
//	ENGINE_DPTR
//		Define a platform variant reference to a pointer in the engine's memory
//	ENGINE_PTR
//		Define a platform variant reference to a value in the engine's memory
//	Macro parameter notes:
//		* If [PTR_NULL], labels that when compiling for that platform we don't 
//			make use of the 'reference' we defined
//		* If [PTR_UNKNOWN], labels that when compiling for that platform we don't 
//			know the address of the 'reference' we're trying to create. In this 
//			situation, compilation of the source will be halted and no output 
//			executable will be created.
//
//-Constant based interface
//	FUNC_PTR
//		Declare a platform variant constant representing a engine function's address
//	DATA_PTR
//		Declare a platform variant constant representing a engine variable's address
//	Macro parameter notes:
//		* See the 'reference' based interface notes.
//		* FUNC_PTR uses: [FUNC_PTR_NULL], [FUNC_PTR_UNKNOWN]
//		* DATA_PTR uses: [DATA_PTR_NULL], [DATA_PTR_UNKNOWN]
//	PLATFORM_VALUE
//		Inline a platform variant constant (could be a number, string, code, etc)
//////////////////////////////////////////////////////////////////////////

// Use this value for platforms where this pointer does not matter
#define PTR_NULL			0x1337BEEF
// Use this value for platforms where this pointer hasn't been located yet
#define PTR_UNKNOWN			NULL
// Use this value for platforms where this function does not matter
#define FUNC_PTR_NULL		0xDEADC0DE
// Use this value for platforms where this function hasn't been located yet
#define FUNC_PTR_UNKNOWN	NULL
// Use this value for platforms where this data does not matter
#define DATA_PTR_NULL		0xDEADBEEF
// Use this value for platforms where this data hasn't been located yet
#define DATA_PTR_UNKNOWN	NULL

// Gets the enum value of [name]
#define GET_FUNC_PTR(name) BOOST_PP_CAT(PTR_,name)
#define GET_DATA_PTR(name) BOOST_PP_CAT(PTR_,name)
// Type casts [name] to a void*, since FUNC_PTRs are really enums
#define GET_FUNC_VPTR(name) (CAST_PTR(void*, GET_FUNC_PTR(##name##) ))
#define GET_DATA_VPTR(name) (CAST_PTR(void*, GET_FUNC_PTR(##name##) ))

// Double pointer inline get
#define GET_DPTR2(name)		BOOST_PP_CAT(pp,name)
// Pointer inline get
#define GET_PTR2(name)		BOOST_PP_CAT(p,name)
// Double pointer inline dereference-get
#define GET_DPTR(name)		( *GET_DPTR2(##name##) )
// Pointer inline dereference-get
#define GET_PTR(name)		( *GET_PTR2(##name##) )

// Double pointer implement get by-value
#define DPTR_IMP_GET(name)	{ return GET_DPTR(##name##); }
// Double pointer implement get by-reference
#define DPTR_IMP_GET2(name)	{ return GET_DPTR2(##name##); }
// Pointer implement get by-value
#define PTR_IMP_GET(name)	{ return GET_PTR(##name##); }
// Pointer implement get by-reference
#define PTR_IMP_GET2(name)	{ return GET_PTR2(##name##); }

namespace Yelo
{
	namespace Enums
	{
		enum x86_opcode : byte {
			// push imm16\32
			_x86_opcode_push_imm = 0x68,
			_x86_opcode_nop = 0x90,

			// mov [e]ax, [offset]
			_x86_opcode_mov_eax_y_offset = 0xA1,

			// mov [offset], [e]ax
			_x86_opcode_mov_offset_y_eax = 0xA3,

			// return near imm16\32
			_x86_opcode_retn = 0xC2,
			// regular return near
			_x86_opcode_ret = 0xC3,
			// call near imm16\32
			_x86_opcode_call_near = 0xE8,
			// jmp near imm16\32
			_x86_opcode_jmp_near = 0xE9,

			// jmp short imm8
			_x86_opcode_jmp_short = 0xEB,
		};
		enum x86_opcode_twobyte : uint16 {
			// call ds:[address]
			_x86_opcode_call_abs = 0x15FF, /*FF 15*/
			// call ds:[address]
			_x86_opcode_jmp_abs = 0x25FF, /*FF 25*/
		};
		BOOST_STATIC_ASSERT( sizeof(Enums::x86_opcode_twobyte)==2 );
	};

	namespace Memory
	{
		struct Opcode
		{
			struct s_call
			{
				byte Op;
				uint32 Address;
			};

			struct s_call_ret
			{
				byte Op;
				uint32 Address;
				byte Ret;
				uint16 Count;
			};

			// absolute call opcode
			struct s_call_abs
			{
				uint16 Op;
				uint32 Address;
			};

			// absolute call opcode with return
			struct s_call_abs_ret
			{
				uint16 Op;
				uint32 Address;
				byte Ret;
				uint16 Count;
			};
		};

		// Copies [size] bytes of [src] to [address]
		// Changes the protection of [address] while its coping [src]
		// Then changes the protection of [address] back
		BOOL WriteMemory(void* address, const void* src, int32 size);
		// makes the 32 bits at [address] equal [value]
		BOOL WriteMemory(void* address, void* value);


		// At [hook_address] writes 'call [hook]' plus user specified end opcode (total: 6 bytes)
		void CreateHookRelativeCall(void* hook, void* hook_address, byte end);

		// At [jmp_address]+1, write the relative address of [to_address] (total: 5 bytes)
		// Writes '0xE9' if [write_opcode]
		// returns the original jmp address (absolute)
		uint32 WriteRelativeJmp(void* to_address, void* jmp_address, bool write_opcode = false);

		// At [call_address]+1, write the relative address of [to_address] (total: 5 bytes)
		// Writes '0xE8' if [write_opcode]
		// returns the original call address (absolute)
		uint32 WriteRelativeCall(void* to_address, void* call_address, bool write_opcode = false);


		// [call] buffer containing the data we wish to write
		// [address] address to put [call]
		void Write(Opcode::s_call& call, void* address);
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
		void WriteRet(Opcode::s_call_ret& call, void* address);
		// [call] buffer containing the data we wish to write
		// [address] address to put [call]
		void WriteRetn(Opcode::s_call_ret& call, void* address);

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


		struct s_memory_exec_change_data {
			void* const ExecAddress;	// 'to_address', address to be executed (jmp or call)
			void* const MemoryAddress;	// 'call_address', memory address to apply the execution modification

			const bool WriteOpcode;
			bool IsInitialized;
			PAD16;
			byte UndoData[5];	// internal buffer to store
			PAD24;

			// At [MemoryAddress], modify the assembly code to execute the code stored 
			// at address [ExecAddress] (total: 5 bytes)
			// See: WriteRelativeJmp
			void ApplyAsRelativeJmp();
			// At [MemoryAddress], modify the assembly code to execute the code stored 
			// at address [ExecAddress] (total: 5 bytes)
			// See: WriteRelativeCall
			void ApplyAsRelativeCall();

			// Restore the memory back to it's original data
			void Undo();

		private:
			void Initialize();
		};
#define DEFINE_MEMORY_EXEC_CHANGE(name, to_address, call_address, ...)	\
	static Yelo::Memory::s_memory_exec_change_data name = {				\
			CAST_PTR(void*, (to_address) ),								\
			CAST_PTR(void*, (call_address) ),							\
			__VA_ARGS__													\
		};


		// Overwrite the memory at [address] with [array]
		// Copies the original memory at [address] into [array] before returning
		// This is useful when writing an array of opaque bytes to game memory
		template<typename T, size_t TSizeOfArray>
		void OverwriteMemoryArray(void* address, T(& array)[TSizeOfArray])
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
		void OverwriteMemorySansCopyArray(void* address, const T(& array)[TSizeOfArray])
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


		uint32 CRC(uint32& crc_reference, const void* buffer, int32 size);
	};
};