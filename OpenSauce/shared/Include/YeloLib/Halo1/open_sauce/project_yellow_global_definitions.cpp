/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/open_sauce/project_yellow_global_definitions.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		cstring project_yellow_globals::k_default_name = "there they are all standing in a row";

		int32 s_unit_infections_definition::FindInfectableUnitIndex(int32 infection_unit_index, datum_index infectable_unit_definition_index) const
		{
			for(int32 x = 0; x < infectable_units.Count; x++)
			{
				const TagGroups::s_unit_infection& unit_infection = infectable_units[x];

				if(	infection_unit_index == unit_infection.infection_unit &&
					infectable_unit_definition_index == unit_infection.unit)
					return x;
			}
			return NONE;
		}

		int32 s_unit_infections_definition::FindInfectionUnitIndex(datum_index unit_definition_index) const
		{
			for(int32 x = 0; x < infection_units.Count; x++)
			{
				if(unit_definition_index == infection_units[x].tag_index)
					return x;
			}
			return NONE;
		}

		int32 s_unit_infections_definition::LookupUnitInfectionIndex(datum_index infection_unit_definition_index, datum_index target_unit_definition_index) const
		{
			return FindInfectableUnitIndex(
				FindInfectionUnitIndex(infection_unit_definition_index),
				target_unit_definition_index);
		}

#if PLATFORM_IS_EDITOR
		void project_yellow_globals::CullInvalidNetworkPlayerUnits()
		{
			if(networking.player_units.Count == 0) return;

			for(int32 x = networking.player_units.Count; x >= 0; x--)
			{
				const TagGroups::s_network_game_player_unit& player_unit = networking.player_units[x];
				bool remove_element = true;

				if( player_unit.name[0] == '\0' )
					printf_s("CheApe: Culling unnamed network_game_player_unit element #%n\n", x);
				else if( player_unit.definition.tag_index.IsNull() )
					printf_s("CheApe: Culling invalid network_game_player_unit element #%n (%s)\n", x, player_unit.name);
				else remove_element = false;

				if(remove_element) tag_block_delete_element(networking.player_units, x);
			}
		}
		/*!
		 * \brief
		 * Removes "preprocess" related and otherwise invalid tag data from the PY globals tag
		 */
		void project_yellow_globals::Cull()
		{
	#if FALSE
			tag_block_delete_all_elements(preprocess);
	#endif

			CullInvalidNetworkPlayerUnits();
		}
#endif
	};
};