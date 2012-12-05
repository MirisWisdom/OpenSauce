/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/memory/data_base.hpp>
#include <YeloLib/Halo1/hs/hs_yelo.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum hs_type : _enum
		{
			_hs_unparsed,
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
			_hs_type,

			// faux HS type, only for the sake of OS code which exposes 8-bit integers
			// NOTE: we treat sbyte as a byte still!
			_hs_type_byte = _hs_type_bool,
		};
	};

	namespace Flags
	{
		enum hs_syntax_node_flags : word_flags
		{
			_hs_syntax_node_primitive_bit,

			_hs_syntax_node_primitive_flag = FLAG(_hs_syntax_node_primitive_bit),
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
		typedef void (PLATFORM_API* hs_parse_proc)(int32 function_index, datum_index expression_index);
		typedef void (PLATFORM_API* hs_evaluate_proc)(int32 function_index, datum_index expression_index, bool* execute);

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
					datum_index* _datum;
				}Value;
			};
			Flags::hs_access_flag access;
			PAD16;
		}; BOOST_STATIC_ASSERT( sizeof(hs_global_definition) == 0x10 );


		struct hs_syntax_node : Memory::s_datum_base
		{
			union {
				int16 constant_type;
				int16 function_index;
			};
			int16 type;
			union {
				word_flags flags;
				int16 pointer_type;
			};
			datum_index next_expression;
			int32 pointer;
			union {
				void* address;

				union {
					bool _bool;
					real _real;
					int16 _short;
					int32 _long;
					datum_index _datum;
				}Value;
			};
		}; BOOST_STATIC_ASSERT( sizeof(hs_syntax_node) == 0x14 );
	};
};