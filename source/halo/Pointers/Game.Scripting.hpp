/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Pointers/1.10/HaloCE_110_Runtime.Game.HS.inl"
#include "Pointers/1.10/HaloCE_110_Runtime_Manual.Game.HS.inl"
#else
#include "Pointers/1.10/HaloCE_110_Dedi.Game.HS.inl"
#include "Pointers/1.10/HaloCE_110_Dedi_Manual.Game.HS.inl"
#endif

#include <blam/cutscene/recorded_animations.hpp>
#include <blam/hs/hs_types_traits.hpp>
#include <blam/hs/object_lists.hpp>
#include <Memory/MemoryInterface.hpp>
#include <yelo/memory/memory_interface_base.hpp>

namespace Yelo
{
	namespace Scripting
	{
		ENGINE_DPTR(recorded_animations_data_t, recorded_animations, DUO_PTR(K_RECORDED_ANIMATIONS));
		ENGINE_DPTR(hs_syntax_data_t, hs_syntax, DUO_PTR(K_HS_SYNTAX));
		ENGINE_DPTR(object_list_header_data_t, object_list_header, DUO_PTR(K_OBJECT_LIST_HEADER));
		ENGINE_DPTR(list_object_reference_data_t, list_object_reference, DUO_PTR(K_LIST_OBJECT_REFERENCE));
		ENGINE_DPTR(hs_globals_data_t, hs_globals, DUO_PTR(K_HS_GLOBALS));
		ENGINE_DPTR(hs_thread_data_t, hs_threads, DUO_PTR(K_HS_THREADS));

		FUNC_PTR(HS_UPDATE_HOOK, DUO_PTR(K_HS_UPDATE_HOOK));

		static uint32* K_MAX_HS_SYNTAX_NODES_PER_SCENARIO_UPGRADE_ADDRESS_LIST[] = {
			CAST_PTR(uint32*, K_MAX_HS_SYNTAX_NODES_PER_SCENARIO_UPGRADE_ADDRESS_LIST_0),
		};

		static uint32* K_TOTAL_SCENARIO_HS_SYNTAX_DATA_UPGRADE_ADDRESS_LIST[] = {
			//CAST_PTR(uint32*, PLATFORM_VALUE(0x485E93, 0x47D783)), // don't modify this one, modify the size check using the address below
			CAST_PTR(uint32*, K_TOTAL_SCENARIO_HS_SYNTAX_DATA_UPGRADE_ADDRESS_LIST_0),
		};

		static byte* ADDRESS_OF_SCENARIO_HS_SYNTAX_DATA_SIZE_CHECK = CAST_PTR(byte*, DUO_PTR(K_ADDRESS_OF_SCENARIO_HS_SYNTAX_DATA_SIZE_CHECK));
	}
}
