/*
    Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition
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
#include "Yelo/ScenarioResources.hpp"
#include <TagGroups/Halo1/scenario_definitions.hpp>

#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace ScenarioResources
	{
		enum scenario_resource {
			_scenario_resource_scenery,
			_scenario_resource_bipeds,
			_scenario_resource_vehicles,
			_scenario_resource_equipment,
			_scenario_resource_weapons,
			_scenario_resource_sound_scenery,
			_scenario_resource_devices,
			_scenario_resource_decals,
			_scenario_resource_cinematics,
			_scenario_resource_trigger_volumes,
			_scenario_resource_cluster_data,
			_scenario_resource_hs_source_file,
			_scenario_resource_ai,
			_scenario_resource_comments,
			_scenario_resource,
		};

		struct scenario_field
		{
			enum {
				_dont_use,
				_wont_use,
				_cant_use,

				_skies,

				_scenario_type,
				_scenario_flags,

				_child_scenarios,

				_local_north,
				pad1,
				pad2,

				_predicted_resources,
				_functions,
				_editor_scenario_data,
				_comments,
				pad3,

				_object_names,
				_scenery,
				_scenery_palette,
				_bipeds,
				_bipeds_palette,
				_vehicles,
				_vehicles_palette,
				_equipment,
				_equipment_palette,
				_weapons,
				_weapons_palette,
				_device_groups,
				_machines,
				_machines_palette,
				_controls,
				_controls_palette,
				_light_fixtures,
				_light_fixtures_palette,
				_sound_scenery,
				_sound_scenery_palette,
				pad4,

				_player_starting_profile,
				_player_starting_locations,

				_trigger_volumes,
				_recorded_animations,

				_netgame_flags,
				_netgame_equipment,
				_starting_equipment,
				_bsp_switch_trigger_volumes,

				_decals,
				_decals_palette,
				_detail_object_collection_palette,
				pad5,

				_actor_palette,
				_encounters,
				_command_lists,
				_ai_animation_references,
				_ai_script_references,
				_ai_recording_references,
				_ai_conversations,

				_script_syntax_data,
				_script_string_data,
				_scripts,
				_globals,
				_references,
				_source_files,
				pad6,

				_cutscene_flags,
				_cutscene_camera_points,
				_cutscene_titles,
				pad7,

				_custom_object_names,
				_ingame_help_text,
				_hud_messages,
				_structure_bsps,

				terminator,
				k_count,
			}; BOOST_STATIC_ASSERT( k_count == 71 );
		};

		static const int32 k_scenario_resources_scenery[] = {
			scenario_field::_object_names,
			scenario_field::_scenery,
			scenario_field::_scenery_palette,
			NONE,
		};

		static const int32 k_scenario_resources_bipeds[] = {
			scenario_field::_object_names,
			scenario_field::_bipeds,
			scenario_field::_bipeds_palette,
			NONE,
		};

		static const int32 k_scenario_resources_vehicles[] = {
			scenario_field::_object_names,
			scenario_field::_vehicles,
			scenario_field::_vehicles_palette,
			NONE,
		};

		static const int32 k_scenario_resources_equipment[] = {
			scenario_field::_object_names,
			scenario_field::_equipment,
			scenario_field::_equipment_palette,
			NONE,
		};

		static const int32 k_scenario_resources_weapons[] = {
			scenario_field::_object_names,
			scenario_field::_weapons,
			scenario_field::_weapons_palette,
			NONE,
		};

		static const int32 k_scenario_resources_sound_scenery[] = {
			scenario_field::_object_names,
			scenario_field::_sound_scenery,
			scenario_field::_sound_scenery_palette,
			NONE,
		};

		static const int32 k_scenario_resources_devices[] = {
			NONE,
		};

		static const int32 k_scenario_resources_decals[] = {
			scenario_field::_decals_palette,
			scenario_field::_decals,
			NONE,
		};

		static const int32 k_scenario_resources_cinematics[] = {
			NONE,
		};

		static const int32 k_scenario_resources_trigger_volumes[] = {
			NONE,
		};

		static const int32 k_scenario_resources_cluster_data[] = {
			NONE,
		};

		static const int32 k_scenario_resources_hs_source_file[] = {
			NONE,
		};

		static const int32 k_scenario_resources_ai[] = {
			NONE,
		};

		static const int32 k_scenario_resources_comments[] = {
			scenario_field::_comments,
			NONE,
		};

		static const int32* k_scenario_resources[] = {
			&k_scenario_resources_scenery[0],
			&k_scenario_resources_bipeds[0],
			&k_scenario_resources_vehicles[0],
			&k_scenario_resources_equipment[0],
			&k_scenario_resources_weapons[0],
			&k_scenario_resources_sound_scenery[0],
			&k_scenario_resources_devices[0],
			&k_scenario_resources_decals[0],
			&k_scenario_resources_cinematics[0],
			&k_scenario_resources_trigger_volumes[0],
			&k_scenario_resources_cluster_data[0],
			&k_scenario_resources_hs_source_file[0],
			&k_scenario_resources_ai[0],
			&k_scenario_resources_comments[0],
		};

		void Initialize()
		{
			tag_group_definition* scnr = Yelo::tag_group_get<TagGroups::scenario>();
		}

		void Dispose()
		{
		}
	};
};