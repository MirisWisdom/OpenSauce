/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/Scripting.hpp"

#include <blamlib/Halo1/cutscene/recorded_animations_structures.hpp>
#include <blamlib/Halo1/hs/hs_runtime_structures.hpp>
#include <blamlib/Halo1/hs/hs_structures.hpp>
#include <YeloLib/Halo1/open_sauce/blam_memory_upgrades.hpp>

#include "Game/EngineFunctions.hpp"
#include "Memory/MemoryInterface.hpp"
#include "Game/ScriptLibrary.hpp"

namespace Yelo
{
	namespace Enums
	{
		enum {

			k_total_scenario_hs_syntax_data_upgrade = sizeof(Memory::s_data_array) + 
				(sizeof(Scripting::hs_syntax_node) * k_maximum_hs_syntax_nodes_per_scenario_upgrade),
		};
	};

	namespace Scripting
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_SCRIPTING
#include "Memory/_EngineLayout.inl"

		static void MemoryUpgradesInitialize()
		{
			for(auto ptr : K_MAX_HS_SYNTAX_NODES_PER_SCENARIO_UPGRADE_ADDRESS_LIST)
				*ptr = Enums::k_maximum_hs_syntax_nodes_per_scenario_upgrade;
			for(auto ptr : K_TOTAL_SCENARIO_HS_SYNTAX_DATA_UPGRADE_ADDRESS_LIST)
				*ptr = Enums::k_total_scenario_hs_syntax_data_upgrade;

			// change from 'jz' (0x74) to 'jge' (0x7D)
			// This allows us to support scenarios with original script nodes, or with
			// Yelo based script nodes, which are larger (because of memory upgrades, duh)
			*K_ADDRESS_OF_SCENARIO_HS_SYNTAX_DATA_SIZE_CHECK = Enums::_x86_opcode_jge_short;
		}

		static void MemoryUpgradesDispose()
		{
		}

		recorded_animations_data_t& RecordedAnimations()					DPTR_IMP_GET_BYREF(recorded_animations);
		hs_syntax_data_t& HSSyntax()										DPTR_IMP_GET_BYREF(hs_syntax);
		object_list_header_data_t& ObjectListHeader()						DPTR_IMP_GET_BYREF(object_list_header);
		list_object_reference_data_t& ListObjectReference()					DPTR_IMP_GET_BYREF(list_object_reference);
		hs_thread_data_t& HSThreads()										DPTR_IMP_GET_BYREF(hs_threads);
		hs_globals_data_t& HSGlobals()										DPTR_IMP_GET_BYREF(hs_globals);

		void Initialize()
		{
			MemoryUpgradesInitialize();

			InitializeLibrary();

#if !PLATFORM_DISABLE_UNUSED_CODE
			// Currently, no code is ran in Update, so don't bother hooking
			Memory::CreateHookRelativeCall(&Scripting::Update, GET_FUNC_VPTR(HS_UPDATE_HOOK), Enums::_x86_opcode_retn);
#endif
		}

		void Dispose()
		{
			Scripting::DisposeLibrary();

			MemoryUpgradesDispose();
		}

		void PLATFORM_API Update()
		{
		}
	};

	namespace blam
	{
		using namespace Scripting;

		datum_index object_list_new()
		{
			datum_index object_list_index = blam::datum_new( ObjectListHeader() );

			if(!object_list_index.IsNull())
			{
				s_object_list_header_datum* object_list = ObjectListHeader()[object_list_index];

				object_list->count = 0;
				object_list->first = datum_index::null;
			}

			return object_list_index;
		}

		datum_index object_list_get_first(datum_index list_index, __out datum_index& list_reference)
		{
			if(!list_index.IsNull())
			{
				s_object_list_header_datum* object_list = ObjectListHeader()[list_index];
				list_reference = object_list->first;

				return object_list_get_next(list_index, list_reference);
			}

			return datum_index::null;
		}

		datum_index object_list_get_next(datum_index list_index, __inout datum_index& list_reference)
		{
			if(!list_reference.IsNull())
			{
				s_list_object_reference_datum* object_reference = ListObjectReference()[list_reference];
				list_reference = object_reference->next_reference;

				return object_reference->object_index;
			}

			return datum_index::null;
		}
	};
};