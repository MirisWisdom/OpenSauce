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
#if FALSE
		void StorePushRet(void* address, void* target)
		{
			Opcode::CallRet* store = (Opcode::CallRet*)address;

			store->Op = Enums::_opcode_push;
			store->Address = CAST_PTR(uint32, target);
			store->Ret = Enums::_opcode_ret;
		}
#endif
	};
};