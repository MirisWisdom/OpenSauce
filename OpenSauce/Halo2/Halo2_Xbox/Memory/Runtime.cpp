/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "Memory/Runtime.hpp"

namespace Yelo
{
	namespace Memory
	{
		void Write(Opcode::Call& call, void* address)
		{
			memcpy(address, &call, sizeof(call));
		}

		void WriteCall(void* call_buffer, void* address, void* target)
		{
			Opcode::Call* call = CAST_PTR(Opcode::Call*, call_buffer);
			Opcode::Call* call_address = CAST_PTR(Opcode::Call*, address);

			call->Op = call_address->Op;					// copy the old
			call_address->Op = Enums::_opcode_call_near;	// set the new
			call->Address = call_address->Address;			// copy the old
			call_address->Address =							// set the new
				CAST_PTR(uint32, target) -		// cast the pointer to a number to perform math on
				(
					CAST_PTR(uint32, address) + sizeof(Opcode::Call)
				);
		}

		void WriteCallRet(void* call_ret_buffer, void* address, void* target)
		{	
			CAST_PTR(Opcode::CallRet*, call_ret_buffer)->Ret = 
				CAST_PTR(Opcode::CallRet*, address)->Ret;					// copy the old
			WriteCall(call_ret_buffer, address, target);
			CAST_PTR(Opcode::CallRet*, address)->Ret = Enums::_opcode_ret;	// set the new
		}

		void WriteCallRet(void* call_ret_buffer, void* address, void* target, uint16 count)
		{
			CAST_PTR(Opcode::CallRet*, call_ret_buffer)->Ret = 
				CAST_PTR(Opcode::CallRet*, address)->Ret;					// copy the old
			WriteCall(call_ret_buffer, address, target);
			CAST_PTR(Opcode::CallRet*, address)->Ret = Enums::_opcode_retn;	// set the new
			CAST_PTR(Opcode::CallRet*, call_ret_buffer)->Count = 
				CAST_PTR(Opcode::CallRet*, address)->Count;					// copy the old
			CAST_PTR(Opcode::CallRet*, address)->Count = (count * 4);		// set the new
		}

		void WriteRet(Opcode::CallRet& call, void* address)
		{
			memcpy(address, &call, sizeof(call) - sizeof(uint16)); // don't include the retn's count
		}

		void WriteRetn(Opcode::CallRet& call, void* address)
		{
			memcpy(address, &call, sizeof(call));
		}

		void WriteJmp(void* jmp_buffer, void* address, void* target)
		{
			Opcode::Call* jmp = CAST_PTR(Opcode::Call*, jmp_buffer);
			Opcode::Call* jmp_address = CAST_PTR(Opcode::Call*, address);

			jmp->Op = jmp_address->Op;					// copy the old
			jmp_address->Op = Enums::_opcode_jmp_near;	// set the new
			jmp->Address = jmp_address->Address;		// copy the old
			jmp_address->Address =						// set the new
				CAST_PTR(uint32, target) -		// cast the pointer to a number to perform math on
				(
					CAST_PTR(uint32, address) + sizeof(Opcode::Call)
				);
		}

		void OverwriteJmp(void* jmp_buffer, void* address, void* target)
		{
			Opcode::Call* jmp_address = CAST_PTR(Opcode::Call*, address);

			CAST_PTR(Opcode::Call*, jmp_buffer)->Address = jmp_address->Address; // copy the old
			jmp_address->Address = // set the new
				CAST_PTR(uint32, target) -		// cast the pointer to a number to perform math on
				(
					CAST_PTR(uint32, address) + sizeof(Opcode::Call)
				);
		}

		void StorePushRet(void* address, void* target)
		{
			Opcode::CallRet* store = (Opcode::CallRet*)address;

			store->Op = Enums::_opcode_push;
			store->Address = CAST_PTR(uint32, target);
			store->Ret = Enums::_opcode_ret;
		}
	};
};