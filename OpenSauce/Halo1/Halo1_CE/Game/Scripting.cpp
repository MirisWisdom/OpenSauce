/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
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
#include "Common/Precompile.hpp"
#include "Game/Scripting.hpp"

#include <Blam/Halo1/BlamMemoryUpgrades.hpp>
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
			//hs_allocate
			//-1x k_maximum_hs_syntax_nodes_per_scenario_upgrade
			//-2x k_total_scenario_hs_syntax_data_upgrade

			for(int32 x = 0; x < NUMBEROF(K_MAX_HS_SYNTAX_NODES_PER_SCENARIO_UPGRADE_ADDRESS_LIST); x++)
				*K_MAX_HS_SYNTAX_NODES_PER_SCENARIO_UPGRADE_ADDRESS_LIST[x] = Enums::k_maximum_hs_syntax_nodes_per_scenario_upgrade;
			for(int32 x = 0; x < NUMBEROF(K_TOTAL_SCENARIO_HS_SYNTAX_DATA_UPGRADE_ADDRESS_LIST); x++)
				*K_TOTAL_SCENARIO_HS_SYNTAX_DATA_UPGRADE_ADDRESS_LIST[x] = Enums::k_total_scenario_hs_syntax_data_upgrade;

			// change from 'jz' (0x74) to 'jge' (0x7D)
			// This allows us to support scenarios with original script nodes, or with
			// Yelo based script nodes, which are larger (because of memory upgrades, duh)
			*K_ADDRESS_OF_SCENARIO_HS_SYNTAX_DATA_SIZE_CHECK = 0x7D;
		}

		static void MemoryUpgradesDispose()
		{
		}

		t_recorded_animations_data* RecordedAnimations()					DPTR_IMP_GET(recorded_animations);
		t_hs_syntax_data* HSSyntax()										DPTR_IMP_GET(hs_syntax);
		t_object_list_header_data* ObjectListHeader()						DPTR_IMP_GET(object_list_header);
		t_list_object_reference_data* ListObjectReference()					DPTR_IMP_GET(list_object_reference);
		t_hs_thread_data* HSThreads()										DPTR_IMP_GET(hs_threads);
		t_hs_globals_data* HSGlobals()										DPTR_IMP_GET(hs_globals);

		void Initialize()
		{
			MemoryUpgradesInitialize();

			InitializeLibrary();

			// Currently, no code is ran in Update, so don't bother hooking
			//Memory::CreateHookRelativeCall(&Scripting::Update, GET_FUNC_VPTR(HS_UPDATE_HOOK), Enums::_x86_opcode_retn);
		}

		void Dispose()
		{
			Scripting::DisposeLibrary();

			MemoryUpgradesDispose();
		}

		void PLATFORM_API Update()
		{
		}

		datum_index ObjectListNew()
		{
			datum_index object_list_index = Engine::Memory::DatumNew(&Scripting::ObjectListHeader()->Header);

			if(!object_list_index.IsNull())
			{
				s_object_list_header_datum* object_list = (*Scripting::ObjectListHeader())[object_list_index];

				object_list->count = 0;
				object_list->first = datum_index::null;
			}

			return object_list_index;
		}
	};
};