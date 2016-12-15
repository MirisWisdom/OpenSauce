/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/memory/data_base.hpp>
#include <blamlib/Halo1/hs/hs_scenario_definitions.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum game_difficulty_level : _enum;
		enum game_team : _enum;
		enum actor_default_state : _enum;
		enum actor_type : _enum;
		enum hud_anchor : _enum;

		enum {
			k_maximum_number_of_hs_function_parameters = 32,
		};

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

			_hs_type_enum_game_difficulty,
			_hs_type_enum_team,
			_hs_type_enum_ai_default_state,
			_hs_type_enum_actor_type,
			_hs_type_enum_hud_corner,

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

			_hs_type_data__first = _hs_type_bool,
			_hs_type_data__last = k_number_of_hs_types - 1,
			_hs_type_data__count = (_hs_type_data__last - _hs_type_data__first) + 1,

			_hs_type_tag_reference__first = _hs_type_sound,
			_hs_type_tag_reference__last = _hs_type_object_definition,
			_hs_type_tag_reference__count = (_hs_type_tag_reference__last - _hs_type_tag_reference__first) + 1,

			_hs_type_enum__first = _hs_type_enum_game_difficulty,
			_hs_type_enum__last = _hs_type_enum_hud_corner,
			_hs_type_enum__count = (_hs_type_enum__last - _hs_type_enum__first) + 1,

			_hs_type_object__first = _hs_type_object,
			_hs_type_object__last = _hs_type_scenery,
			_hs_type_object__count = (_hs_type_object__last - _hs_type_object__first) + 1,

			_hs_type_object_name__first = _hs_type_object_name,
			_hs_type_object_name__last = _hs_type_scenery_name,
			_hs_type_object_name__count = (_hs_type_object_name__last - _hs_type_object_name__first) + 1,
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

		typedef Memory::DataArray<	hs_syntax_node, 
									Enums::k_maximum_hs_syntax_nodes_per_scenario, 
									Enums::k_maximum_hs_syntax_nodes_per_scenario_upgrade> 
			hs_syntax_data_t;

		struct s_hs_script_container_datum_index
		{
			union {
				int16 script_index;
				int16 global_index;
			};
//			int16 script_container_index;

			bool IsNull() const
			{
				return HandleIsNone(*this);
			}

			static s_hs_script_container_datum_index CreateIndex(int16 index,
				int16 script_container_index = 0); // TODO: remove default parameter when containers come online

			static const s_hs_script_container_datum_index k_null;
		}; BOOST_STATIC_ASSERT( sizeof(s_hs_script_container_datum_index) == 2 );

		typedef s_hs_script_container_datum_index hs_script_index_t;
		typedef s_hs_script_container_datum_index hs_global_index_t;

		union s_hs_value_union
		{
			uintptr_t bits;
			void* pointer;

			bool boolean;
			real real;
			int16 int16;
			int32 int32;
			cstring string;

			hs_script_index_t script;
//			hs_global_index_t global;

			Enums::game_difficulty_level difficulty_level;
			Enums::game_team team;
			Enums::actor_default_state ai_default_state;
			Enums::actor_type actor_type;
			Enums::hud_anchor hud_corner;

			datum_index tag_index;
			datum_index datum;

			string_id string_id;

			datum_index ToExpressionIndex() const	{ return datum; }
			datum_index ToTagIndex() const			{ return datum; }
			datum_index ToObjectIndex() const		{ return datum; }
		}; BOOST_STATIC_ASSERT( sizeof(s_hs_value_union) == sizeof(void*) );
		extern const s_hs_value_union k_null_value_union;
		extern const s_hs_value_union k_none_value_union;


		hs_syntax_data_t&			HsSyntax();
	};

	namespace blam
	{
		void hs_nodes_garbage_collect();

		Scripting::hs_function_definition* hs_function_get(int16 function_index);
		Scripting::hs_global_definition* hs_global_external_get(int16 global_index);

		Enums::hs_type hs_global_get_type(int16 global_index);
		cstring hs_global_get_name(int16 global_index);

		int16 hs_find_global_by_name(cstring name);

		int16 hs_find_function_by_name(cstring name);

		int16 hs_find_tag_reference_by_index(datum_index tag_index);

		Scripting::hs_script_index_t hs_find_script_by_name(cstring name);

		bool hs_evaluate_by_script_name(cstring name);


		Scripting::hs_syntax_node* hs_syntax_get(datum_index expression_index);
	};

	// these are now defined in hs_types.cpp
	namespace blam
	{
		extern const std::array<word_flags, Enums::k_number_of_hs_object_types> k_hs_object_type_masks;
		extern const std::array<tag, Enums::_hs_type_tag_reference__count> k_hs_tag_reference_type_group_tags;

		extern std::array<cstring, Enums::k_number_of_hs_types> hs_type_names;
		extern std::array<cstring, Enums::k_number_of_hs_script_types> hs_script_type_names;
		extern std::array<const string_list, Enums::_hs_type_enum__count> hs_enum_table;

		bool hs_type_valid(_enum type);
		bool hs_type_is_enum(_enum type);
		bool hs_type_is_tag_reference(_enum type);
		bool hs_type_is_object(_enum type);
		bool hs_type_is_object_name(_enum type);
	};
};

#include <YeloLib/Halo1/hs/hs_yelo.hpp>