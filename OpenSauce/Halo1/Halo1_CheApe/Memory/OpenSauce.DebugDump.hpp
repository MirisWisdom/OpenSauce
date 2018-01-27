/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <Memory/MemoryInterface.hpp>
#include <yelolib/memory/memory_interface_base.hpp>

namespace Yelo
{
	namespace Debug
	{
		FUNC_PTR(GENERIC_EXCEPTION_FILTER_CALL, FUNC_PTR_NULL, 0x415456, 0x4A8278);
		FUNC_PTR(GENERIC_EXCEPTION_FILTER, 0x42DA20, 0x437090, 0x4F81E0);

		ENGINE_PTR(void*, CWINAPP_RUN_HOOK, 0x89AD54, FUNC_PTR_NULL, FUNC_PTR_NULL);
		FUNC_PTR(CWINAPP_RUN, 0x84EBE1, FUNC_PTR_NULL, FUNC_PTR_NULL);
	}
}