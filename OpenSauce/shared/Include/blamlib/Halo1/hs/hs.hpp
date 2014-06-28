/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/hs/hs_scenario_definitions.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum hs_type : _enum
		{
			_hs_unparsed,
			// script or global
			_hs_special_form,
			_hs_function_name,
			_hs_passthrough,
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
			
			k_number_of_hs_types,

			//_hs_type_string_id = k_number_of_hs_types,
			//_hs_type_any_tag,
			//_hs_type_any_tag_not_resolving,
			k_number_of_hs_types_yelo = k_number_of_hs_types,

			// faux HS type, only for the sake of OS code which exposes 8-bit integers
			// NOTE: we treat sbyte as a byte still!
			_hs_type_byte = _hs_type_bool,

			_hs_type_tag_reference__first = _hs_type_sound,
			_hs_type_tag_reference__last = _hs_type_object_definition,

			_hs_type_enum__first = _hs_type_game_difficulty,
			_hs_type_enum__last = _hs_type_hud_corner,

			_hs_type_object__first = _hs_type_object,
			_hs_type_object__last = _hs_type_scenery,

			_hs_type_object_name__first = _hs_type_object_name,
			_hs_type_object_name__last = _hs_type_scenery_name,
		};

		enum hs_object_type : _enum
		{
			_hs_object_type_any =		_hs_type_object - _hs_type_object__first,
			_hs_object_type_unit =		_hs_type_unit - _hs_type_object__first,
			_hs_object_type_vehicle =	_hs_type_vehicle - _hs_type_object__first,
			_hs_object_type_weapon =	_hs_type_weapon - _hs_type_object__first,
			_hs_object_type_device =	_hs_type_device - _hs_type_object__first,
			_hs_object_type_scenery =	_hs_type_scenery - _hs_type_object__first,

			k_number_of_hs_object_types
		};

		enum hs_script_type : _enum
		{
			_hs_script_startup,
			_hs_script_dormant,
			_hs_script_continuous,
			_hs_script_static,
			_hs_script_stub,

			k_number_of_hs_script_types,
		};
	};

	namespace Scripting
	{
		struct hs_function_definition;
		struct hs_global_definition;

		struct hs_syntax_node;
	};

	namespace blam
	{
		extern const std::array<word_flags, Enums::k_number_of_hs_object_types> k_hs_object_type_masks;

		bool hs_type_valid(_enum type);
		bool hs_type_is_enum(_enum type);
		bool hs_type_is_tag_reference(_enum type);
		bool hs_type_is_object(_enum type);
		bool hs_type_is_object_name(_enum type);

		void hs_nodes_garbage_collect();

		Scripting::hs_function_definition* hs_function_get(int16 function_index);
		Scripting::hs_global_definition* hs_global_external_get(int16 global_index);

		Enums::hs_type hs_global_get_type(int32 global_index);
		cstring hs_global_get_name(int32 global_index);

		int16 hs_find_global_by_name(cstring name);

		int16 hs_find_function_by_name(cstring name);

		int16 hs_find_tag_reference_by_index(datum_index tag_index);

		int16 hs_find_script_by_name(cstring name);


		Scripting::hs_syntax_node* hs_syntax_get(datum_index expression_index);
	};
};

#include <YeloLib/Halo1/hs/hs_yelo.hpp>