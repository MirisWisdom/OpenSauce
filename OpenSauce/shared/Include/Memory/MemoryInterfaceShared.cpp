/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <Memory/MemoryInterfaceShared.hpp>

// We apply '__declspec(noinline)' to the memory functions to avoid
// any invalid inlining cases with the linker when it thinks a supplied
// address is actually in our DLL's code when it's not...it's a pointer to halo's
// code.
// Ex:
// 1>\memory\memoryinterface.cpp(149) : fatal error C1001: An internal error has occurred in the compiler.
// 1>(compiler file 'F:\SP\vctools\compiler\utc\src\P2\main.c[0x10BEFEAB:0x003A0073]', line 182)
// 1> To work around this problem, try simplifying or changing the program near the locations listed above.
// 
// This was from 'WriteMemory' before it was marked as noinline, and was trying
// to inline with a halo address

#if PLATFORM_TARGET == PLATFORM_TARGET_XBOX
	#define memcpy Std_memcpy
	#define memcpy_s(dst, dst_size, src, src_size) (dst==memcpy(dst, src, src_size))
#endif

namespace Yelo
{
	namespace Memory
	{
		__declspec(noinline) BOOL WriteMemory(void* address, const void* src, int32 size)
		{
			return address == memcpy(address, src, size);
		}

#pragma warning( push )
#pragma warning( disable : 4311 ) // bitching about this typecast
#pragma warning( disable : 4312 ) // bitching about typecast
		__declspec(noinline) BOOL WriteMemory(void* address, void* value)
		{
			*CAST_PTR(uint32*, address) = CAST_PTR(uint32, value);
			return true;
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


		void Write(Opcode::s_call& call, void* address)
		{
			memcpy(address, &call, sizeof(call));
		}
		void WriteCall(void* call_buffer, void* address, const void* target)
		{
			Opcode::s_call* call = CAST_PTR(Opcode::s_call*, call_buffer);
			Opcode::s_call* call_address = CAST_PTR(Opcode::s_call*, address);

			call->Op = call_address->Op;					// copy the old
			call_address->Op = Enums::_x86_opcode_call_near;// set the new
			call->Address = call_address->Address;			// copy the old
			call_address->Address =							// set the new
				CAST_PTR(uint32, target) -		// cast the pointer to a number to perform math on
				(
					CAST_PTR(uint32, address) + sizeof(Opcode::s_call)
				);
		}

		void WriteCallRet(void* call_ret_buffer, void* address, const void* target)
		{	
			CAST_PTR(Opcode::s_call_ret*, call_ret_buffer)->Ret = 
				CAST_PTR(Opcode::s_call_ret*, address)->Ret;						// copy the old
			WriteCall(call_ret_buffer, address, target);
			CAST_PTR(Opcode::s_call_ret*, address)->Ret = Enums::_x86_opcode_ret;	// set the new
		}
		void WriteCallRet(void* call_ret_buffer, void* address, const void* target, const uint16 count)
		{
			CAST_PTR(Opcode::s_call_ret*, call_ret_buffer)->Ret = 
				CAST_PTR(Opcode::s_call_ret*, address)->Ret;						// copy the old
			WriteCall(call_ret_buffer, address, target);
			CAST_PTR(Opcode::s_call_ret*, address)->Ret = Enums::_x86_opcode_retn;	// set the new
			CAST_PTR(Opcode::s_call_ret*, call_ret_buffer)->Count = 
				CAST_PTR(Opcode::s_call_ret*, address)->Count;						// copy the old
			CAST_PTR(Opcode::s_call_ret*, address)->Count = (count * 4);			// set the new
		}

		void WriteRet(Opcode::s_call_ret& call, void* address)
		{
			memcpy(address, &call, sizeof(call) - sizeof(uint16)); // don't include the retn's count
		}
		void WriteRetn(Opcode::s_call_ret& call, void* address)
		{
			memcpy(address, &call, sizeof(call));
		}

		void WriteJmp(void* jmp_buffer, void* address, const void* target)
		{
			Opcode::s_call* jmp = CAST_PTR(Opcode::s_call*, jmp_buffer);
			Opcode::s_call* jmp_address = CAST_PTR(Opcode::s_call*, address);

			jmp->Op = jmp_address->Op;						// copy the old
			jmp_address->Op = Enums::_x86_opcode_jmp_near;	// set the new
			jmp->Address = jmp_address->Address;			// copy the old
			jmp_address->Address =							// set the new
				CAST_PTR(uint32, target) -		// cast the pointer to a number to perform math on
				(
					CAST_PTR(uint32, address) + sizeof(Opcode::s_call)
				);
		}

		void OverwriteJmp(void* jmp_buffer, void* address, const void* target)
		{
			Opcode::s_call* jmp_address = CAST_PTR(Opcode::s_call*, address);

			CAST_PTR(Opcode::s_call*, jmp_buffer)->Address = jmp_address->Address; // copy the old
			jmp_address->Address =				// set the new
				CAST_PTR(uint32, target) -		// cast the pointer to a number to perform math on
				(
					CAST_PTR(uint32, address) + sizeof(Opcode::s_call)
				);
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

#if PLATFORM_TARGET == PLATFORM_TARGET_XBOX
	#undef memcpy
	#undef memcpy_s
#endif