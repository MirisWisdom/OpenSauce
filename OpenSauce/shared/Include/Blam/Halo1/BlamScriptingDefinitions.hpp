/*
    Yelo: Open Sauce SDK

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
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum hs_type : _enum
		{
			_hs_type_unparsed,
			_hs_type_special_form,
			_hs_type_function_name,
			_hs_type_passthrough,
			_hs_type_void,
			_hs_type_bool,
			_hs_type_real,
			_hs_type_short,
			_hs_type_long,
			_hs_type_string,
			_hs_type_script,
			_hs_type_trigger_volume,
			_hs_type_cutscene_flag,
			_hs_type_cutscene_camera_point,
			_hs_type_cutscene_title,
			_hs_type_cutscene_recording,
			_hs_type_device_group,
			_hs_type_ai,
			_hs_type_ai_command_list,
			_hs_type_starting_profile,
			_hs_type_conversation,
			_hs_type_navpoint,
			_hs_type_hud_message,
			_hs_type_object_list,
			_hs_type_sound,
			_hs_type_effect,
			_hs_type_damage,
			_hs_type_looping_sound,
			_hs_type_animation_graph,
			_hs_type_actor_variant,
			_hs_type_damage_effect,
			_hs_type_object_definition,
			_hs_type_game_difficulty,
			_hs_type_team,
			_hs_type_ai_default_state,
			_hs_type_actor_type,
			_hs_type_hud_corner,
			_hs_type_object,
			_hs_type_unit,
			_hs_type_vehicle,
			_hs_type_weapon,
			_hs_type_device,
			_hs_type_scenery,
			_hs_type_object_name,
			_hs_type_unit_name,
			_hs_type_vehicle_name,
			_hs_type_weapon_name,
			_hs_type_device_name,
			_hs_type_scenery_name,
			_hs_type,
		};
#define HS_TYPE(hstype) BOOST_JOIN(Yelo::Enums::_hs_type_,hstype)

	};

	namespace Flags
	{
		enum hs_yelo_definition_flags : word_flags
		{
			_hs_yelo_definition_internal_bit,
			_hs_yelo_definition_is_synchronized_bit,

			_hs_yelo_definition_internal_flag =	FLAG(_hs_yelo_definition_internal_bit),
			// changes to the global are sync'd
			_hs_yelo_definition_is_synchronized = FLAG(_hs_yelo_definition_is_synchronized_bit),
		};

		enum hs_access_flag : word_flags
		{
			_hs_access_flag_none,
			_hs_access_flag_enabled =		FLAG(0),
			_hs_access_flag_sent_to_server =FLAG(1), ///< automatically sent to server
			_hs_access_flag_rconable =		FLAG(2), ///< can be executed from an rcon command
			_hs_access_flag_client =		FLAG(3), ///< only a client connection can execute it
			_hs_access_flag_server =		FLAG(4), ///< only a server connection can execute it
		};
	};

	namespace Scripting
	{
		typedef void (PLATFORM_API* hs_parse_proc)(int32 function_index, datum_index script_datum);
		typedef void (PLATFORM_API* hs_evaluate_proc)(int32 function_index, datum_index script_datum, bool* execute);

		typedef void* (API_FUNC* hs_yelo_function_proc)();
		typedef void* (API_FUNC* hs_yelo_function_with_params_proc)(void** arguments);


		// halo script function definition
		struct hs_function_definition
		{
			Enums::hs_type return_type;
			word_flags flags; // padding in halo, special flags in project yellow
			cstring name;
			hs_parse_proc parse;
			hs_evaluate_proc evaluate;
			cstring info;
			cstring param_info;
			Flags::hs_access_flag access;
			int16 paramc;
#pragma warning( push )
#pragma warning( disable : 4200 ) // nonstandard extension used : zero-sized array in struct/union, Cannot generate copy-ctor or copy-assignment operator when UDT contains a zero-sized array
			Enums::hs_type params[];
#pragma warning( pop )
		}; BOOST_STATIC_ASSERT( sizeof(hs_function_definition) == 0x1C ); // size doesn't include [params]

		// halo script accessible value
		struct hs_global_definition
		{
			cstring name;
			Enums::hs_type type;
			word_flags flags; // padding in halo, special flags in project yellow
			union {
				void* address;

				union {
					bool* _bool;
					real* _real;
					int16* _short;
					int32* _long;
					char* _string;
					datum_index _datum;
				}Value;
			};
			Flags::hs_access_flag access;
			PAD16;
		}; BOOST_STATIC_ASSERT( sizeof(hs_global_definition) == 0x10 );

		// This is the name of a blam global which isn't used in release builds of the game.
		// We use this global to expose the build version to scripts without causing harm when not using OS.
		// It was chosen due to the hs_type being a 'real', which allows us to specify the version number as "MAJ.MIN"
		// See: K_OPENSAUCE_VERSION
		const cstring k_external_global_opensauce_override_name = "ai_debug_path_maximum_radius";
	};
};