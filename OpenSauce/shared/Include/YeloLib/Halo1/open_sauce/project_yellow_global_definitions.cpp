/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"

#include <blamlib/Halo1/cseries/errors.hpp>
#include <blamlib/Halo1/units/unit_definitions.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <YeloLib/Halo1/hs/hs_yelo.hpp>
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

		int32 project_yellow_globals_cv::FindUnitExternalUpgradeIndex(datum_index unit_tag_index) const
		{
			for (int32 x = 0; x < unit_external_upgrades.Count; x++)
			{
				if (unit_tag_index == unit_external_upgrades[x].unit.tag_index)
					return x;
			}
			return NONE;
		}

		int32 project_yellow_globals_cv::FindUnitExternalUpgradeBoardingSeatIndex(datum_index unit_tag_index, int16 seat_index) const
		{
			TagGroups::s_unit_external_upgrades const* unit_external_upgrades = 
				FindUnitExternalUpgradeBlock(unit_tag_index);

			if (unit_external_upgrades == nullptr)
				return NONE;
			
			for (int32 x = 0; x < unit_external_upgrades->boarding_seats.Count; x++)
			{
				if (seat_index == unit_external_upgrades->boarding_seats[x].seat_index)
					return x;
			}
			return NONE;
		}

		TagGroups::s_unit_external_upgrades const* project_yellow_globals_cv::FindUnitExternalUpgradeBlock(datum_index unit_tag_index) const
		{
			for(const auto& upgrade : unit_external_upgrades)
				if (unit_tag_index == upgrade.unit.tag_index)
					return &upgrade;

			return nullptr;
		}

		TagGroups::s_unit_boarding_seat const* project_yellow_globals_cv::FindUnitExternalUpgradeBoardingSeatBlock(datum_index unit_tag_index, int16 seat_index) const
		{
			TagGroups::s_unit_external_upgrades const* unit_external_upgrades = 
				FindUnitExternalUpgradeBlock(unit_tag_index);

			if (unit_external_upgrades == nullptr)
				return nullptr;

			for(const auto& boarding_seat : unit_external_upgrades->boarding_seats)
				if (seat_index == boarding_seat.seat_index)
					return &boarding_seat;

			return nullptr;
		}

#if PLATFORM_IS_EDITOR
		void project_yellow_globals::CullInvalidNetworkPlayerUnits()
		{
			for(int32 x = networking.player_units.Count-1; x >= 0; x--)
			{
				const TagGroups::s_network_game_player_unit& player_unit = networking.player_units[x];
				bool remove_element = true;

				if( player_unit.name[0] == '\0' )
					YELO_WARN("CheApe: Culling unnamed network_game_player_unit element #%n\n", x);
				else if( player_unit.definition.tag_index.IsNull() )
					YELO_WARN("CheApe: Culling invalid network_game_player_unit element #%n (%s)\n", x, player_unit.name);
				else
					remove_element = false;

				if (remove_element)
					blam::tag_block_delete_element(networking.player_units, x);
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

		bool project_yellow_globals::HasCvGlobals() const
		{
			return	cv_globals.group_tag == project_yellow_globals_cv::k_group_tag &&
					cv_globals.tag_index.IsNull() == false;
		}

#if PLATFORM_IS_EDITOR
		//////////////////////////////////////////////////////////////////////////
		// project_yellow_globals_cv
#if FALSE
		cstring PLATFORM_API s_yelo_preprocess_maplist_entry::FormatBlockName(datum_index tag_index, tag_block* block, int32 element, char formatted_buffer[Enums::k_tag_block_format_buffer_size])
		{
			auto* elem = TAG_BLOCK_GET_ELEMENT(block, element, s_yelo_preprocess_maplist_entry);

			cstring value = elem->name;
			if( !strcmp(value, "") )
			{
				if( elem->scenario.name_length > 0 && elem->scenario.name != nullptr)
					value = elem->scenario.name;
				else
					return formatted_buffer; // if name is empty and there is no scenario reference, no formatting is done
			}

			strncpy_s(formatted_buffer, Enums::k_tag_block_format_buffer_size, value, Enums::k_tag_string_length);

			return formatted_buffer;
		}
#endif

		bool PLATFORM_API project_yellow_globals::GroupPostprocess(datum_index tag_index, Enums::tag_postprocess_mode mode)
		{
			auto* def = blam::tag_get<project_yellow_globals>(tag_index);

			def->version = project_yellow_globals::k_version;

			Scripting::ScriptingBlockClear(def->yelo_scripting);
			if (mode == Enums::_tag_postprocess_mode_for_runtime)
			{
				Scripting::ScriptingBlockAddDefinitions(def->yelo_scripting, true);
			}

			return true;
		}
		//////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////
		// scripting_block
		cstring PLATFORM_API s_script_construct_definition::FormatBlockName(datum_index tag_index, tag_block* block, int32 element, char formatted_buffer[Enums::k_tag_block_format_buffer_size])
		{
			auto* elem = TAG_BLOCK_GET_ELEMENT(block, element, s_script_construct_definition);

			if (elem->name[0][0] != '\0')
				strncpy_s(formatted_buffer, Enums::k_tag_block_format_buffer_size, elem->name[0], Enums::k_tag_string_length);
			if (elem->name[1][0] != '\0')
				strncat_s(formatted_buffer, Enums::k_tag_block_format_buffer_size, elem->name[1], Enums::k_tag_string_length);

			return formatted_buffer;
		}
		//////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////
		// unit_external_upgrades_block
		static int UnitGetSeatIndexFromLabel(s_unit_definition* unit_def, cstring seat_label)
		{
			for (int i = 0; i < unit_def->unit.seats.Count; i++)
			{
				if (strcmp(seat_label, unit_def->unit.seats[i].label) == 0)
					return i;
			}
			return NONE;
		}

		static void UnitExternalUpgradesBlockPostprocess(TAG_TBLOCK(&unit_external_upgrades_def, s_unit_external_upgrades),
			Enums::tag_postprocess_mode mode)
		{
			if (mode != Enums::_tag_postprocess_mode_for_runtime)
				return;

			for (auto& upgrade : unit_external_upgrades_def)
			{
				datum_index tag_index = blam::tag_load(upgrade.unit.group_tag, upgrade.unit.name,
					FLAG(Flags::_tag_load_non_resolving_references_bit));

				if (!tag_index.IsNull())
				{
					auto* unit_def = blam::tag_get<s_unit_definition>(tag_index);

					for (auto& seat : upgrade.boarding_seats)
					{
						cstring seat_label = seat.seat_label;
						cstring target_seat_label = seat.target_seat_label;

						seat.seat_index = UnitGetSeatIndexFromLabel(unit_def, seat_label);
						seat.target_seat_index = UnitGetSeatIndexFromLabel(unit_def, target_seat_label);
					}
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// project_yellow_globals_cv
		bool PLATFORM_API project_yellow_globals_cv::GroupPostprocess(datum_index tag_index, Enums::tag_postprocess_mode mode)
		{
			auto* def = blam::tag_get<project_yellow_globals_cv>(tag_index);

			def->version = project_yellow_globals_cv::k_version;
			UnitExternalUpgradesBlockPostprocess(def->unit_external_upgrades, mode);

			return true;
		}
		//////////////////////////////////////////////////////////////////////////
#endif
	};
};