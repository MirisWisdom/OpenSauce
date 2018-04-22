/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Pointers/1.10/HaloCE_110_Runtime.OpenSauce.inl"
#include "Pointers/1.10/HaloCE_110_Runtime_Manual.OpenSauce.inl"
#else
#include "Pointers/1.10/HaloCE_110_Dedi.OpenSauce.inl"
#endif

#include <Memory/MemoryInterface.hpp>
#include <yelo/memory/memory_interface_base.hpp>

namespace Yelo
{
	namespace Main
	{
		FUNC_PTR(QUERY_EXITFLAG_REG_CALL, DUO_PTR(K_QUERY_EXITFLAG_REG_CALL));
		FUNC_PTR(QUERY_EXITFLAG_REG, DUO_PTR(K_QUERY_EXITFLAG_REG));
		FUNC_PTR(RELEASE_RESOURCES_ON_EXIT_CALL, DUO_PTR(K_RELEASE_RESOURCES_ON_EXIT_CALL));
		FUNC_PTR(RELEASE_RESOURCES_ON_EXIT, DUO_PTR(K_RELEASE_RESOURCES_ON_EXIT));

		DATA_PTR(PE_DATA_SIZE, 0x215000, 0x171000);
	}
}
