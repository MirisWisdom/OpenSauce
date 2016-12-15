/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "Memory/MemoryInterface.hpp"

#include <blamlib/Halo2/memory/data.hpp>

#include "Game/EngineFunctions.hpp"

namespace Yelo
{
	namespace Memory
	{
		void* s_data_array::IteratorNext(void* iter)
		{
			return Engine::Memory::DataIteratorNext(CAST_PTR(s_data_array_iterator*,iter));
		}
	}
};