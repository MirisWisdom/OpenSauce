/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <Memory/MemoryInterfaceShared.hpp>

namespace Yelo
{
	namespace Memory
	{
		__declspec(noinline) BOOL WriteMemory(void* address, const void* src, int32 size)
		{
			//DWORD old, old2;

			if( 
		//		VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &old) && 
				memcpy(address, src, size) 
				)
				return TRUE;
		//		return VirtualProtect(address, size, old, &old2);
			else
				return FALSE;
		}

#pragma warning( push )
#pragma warning( disable : 4311 ) // bitching about this typecast
#pragma warning( disable : 4312 ) // bitching about typecast
		__declspec(noinline) BOOL WriteMemory(void* address, void* value)
		{
			//int32 size = 1<<2;
			//DWORD old, old2;
			//	if( VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &old) )
			//	{
			*CAST_PTR(uint32*, address) = CAST_PTR(uint32, value);
			return true;
			//		return VirtualProtect(address, size, old, &old2);
			//	}
			//	else
			//		return FALSE;
		}

		__declspec(noinline) void CreateHookRelativeCall(void* hook, void* hook_address, byte end)
		{
			static byte CreateHookRelativeJmp_asm_code[] =
			{
				Enums::_x86_opcode_call_near, 0x00, 0x00, 0x00, 0x00, // call near
				0x00 // retn\nop
			};

			byte* asm_code = &CreateHookRelativeJmp_asm_code[0];

			uint32* temp = CAST_PTR(uint32*, &asm_code[1]);

			// relative call to [hook]
			*temp = CAST_PTR(uint32, hook) - CAST_PTR(uint32, hook_address) - 5;

			asm_code[5] = end;

			WriteMemory(hook_address, asm_code, sizeof(CreateHookRelativeJmp_asm_code));
		}

		__declspec(noinline) uint32 WriteRelativeJmp(void* to_address, void* jmp_address, bool write_opcode)
		{
			static byte real_opcode = Enums::_x86_opcode_jmp_near; // jmp [function]

			if(write_opcode) WriteMemory(jmp_address, &real_opcode, sizeof(real_opcode));

			uint32 original = CAST_PTR(uint32, jmp_address) + *CAST_PTR(uint32*, CAST_PTR(uint32, jmp_address)+1) + 5;

			uint32 relative = CAST_PTR(uint32, to_address) - (CAST_PTR(uint32, jmp_address) + 5);
			WriteMemory(CAST_PTR(void*, CAST_PTR(uint32, jmp_address)+1 ), CAST_PTR(void*, relative));

			return original;
		}

		__declspec(noinline) uint32 WriteRelativeCall(void* to_address, void* call_address, bool write_opcode)
		{
			static byte real_opcode = Enums::_x86_opcode_call_near; // call [function]

			if(write_opcode) WriteMemory(call_address, &real_opcode, sizeof(real_opcode));

			uint32 original = CAST_PTR(uint32, call_address) + *CAST_PTR(uint32*, CAST_PTR(uint32, call_address)+1) + 5;

			uint32 relative = CAST_PTR(uint32, to_address) - (CAST_PTR(uint32, call_address) + 5);
			WriteMemory(CAST_PTR(void*, CAST_PTR(uint32, call_address)+1 ), CAST_PTR(void*, relative));

			return original;
		}
#pragma warning( pop )


		__declspec(noinline) void s_memory_exec_change_data::ApplyAsRelativeJmp()
		{
			this->Initialize();
			WriteRelativeJmp(ExecAddress, MemoryAddress, WriteOpcode);
		}

		__declspec(noinline) void s_memory_exec_change_data::ApplyAsRelativeCall()
		{
			this->Initialize();
			WriteRelativeCall(ExecAddress, MemoryAddress, WriteOpcode);
		}

		__declspec(noinline) void s_memory_exec_change_data::Undo()
		{
			if(this->IsInitialized)
				this->IsInitialized = 
				!(memcpy_s(MemoryAddress, NUMBEROF(UndoData), UndoData, NUMBEROF(UndoData)) == k_errnone);
		}

		__declspec(noinline) void s_memory_exec_change_data::Initialize()
		{
			if(!this->IsInitialized)
				this->IsInitialized = 
				memcpy_s(UndoData, NUMBEROF(UndoData), MemoryAddress, NUMBEROF(UndoData)) == k_errnone;
		}
	};
};