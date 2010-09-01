/*
    Yelo: Open Sauce SDK

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
#define GET_FUNC_PTR(name) PTR_##name
#define GET_DATA_PTR(name) PTR_##name
// Type casts [name] to a void*, since FUNC_PTRs are really enums
#define GET_FUNC_VPTR(name) (CAST_PTR(void*, GET_FUNC_PTR(##name##) ))
#define GET_DATA_VPTR(name) (CAST_PTR(void*, GET_FUNC_PTR(##name##) ))

// Double pointer inline get
#define GET_DPTR2(name)		pp##name
// Pointer inline get
#define GET_PTR2(name)		p##name
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
	namespace Memory
	{
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
	};
};