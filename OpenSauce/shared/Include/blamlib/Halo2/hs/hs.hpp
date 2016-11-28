/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/memory/data_base.hpp>
#include <YeloLib/Halo2/hs/hs_yelo.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum hs_script_type
		{
			_hs_script_type_startup,
			_hs_script_type_dormant,
			_hs_script_type_continuous,
			_hs_script_type_static,
			_hs_script_type_stub,
			_hs_script_type_command_script,
			_hs_script_type,
		};

		enum hs_type : _enum
		{
			_hs_unparsed,
			_hs_special_form,
			_hs_function_name,
			_hs_passthrough,
			_hs_type_void,
			_hs_type_boolean,
			_hs_type_real,
			_hs_type_short,
			_hs_type_long,
			_hs_type_string,
			_hs_type_script,
			_hs_type_string_id,
			_hs_type_unit_seat_mapping,
			_hs_type_trigger_volume,
			_hs_type_cutscene_flag,
			_hs_type_cutscene_camera_point,
			_hs_type_cutscene_title,
			_hs_type_cutscene_recording,
			_hs_type_device_group,
			_hs_type_ai,
			_hs_type_ai_command_list,
			_hs_type_ai_command_script,
			_hs_type_ai_behavior,
			_hs_type_ai_orders,
			_hs_type_starting_profile,
			_hs_type_conversation,
			_hs_type_structure_bsp,
			_hs_type_navpoint,
			_hs_type_point_reference,
			_hs_type_style,
			_hs_type_hud_message,
			_hs_type_object_list,
			_hs_type_sound,
			_hs_type_effect,
			_hs_type_damage,
			_hs_type_looping_sound,
			_hs_type_animation_graph,
			_hs_type_object_definition,
			_hs_type_bitmap,
			_hs_type_shader,
			_hs_type_render_model,
			_hs_type_structure_definition,
			_hs_type_lightmap_definition,
			_hs_type_game_difficulty,
			_hs_type_team,
			_hs_type_actor_type,
			_hs_type_hud_corner,
			_hs_type_model_state,
			_hs_type_network_event,
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
	};

	namespace Scripting
	{
		typedef void (PLATFORM_API* hs_parse_proc)(int32 function_index, datum_index expression_index);
		typedef void (PLATFORM_API* hs_evaluate_proc)(int32 function_index, datum_index expression_index, bool* execute);

		// halo script function definition
		struct hs_function_definition
		{
			_enum return_type;
			uint16 flags; // padding in halo, special flags in project yellow
#if PLATFORM_IS_EDITOR || PLATFORM_ID == PLATFORM_H2_ALPHA
			cstring name;
#endif
			long_flags unknown; // TODO: was this in alpha?
#if PLATFORM_IS_EDITOR
			hs_parse_proc parse;
#endif
			hs_evaluate_proc evaluate;
#if PLATFORM_IS_EDITOR
			cstring info;
#endif
			cstring param_info;
			int16 paramc;
#pragma warning( push )
#pragma warning( disable : 4200 ) // nonstandard extension used : zero-sized array in struct/union, Cannot generate copy-ctor or copy-assignment operator when UDT contains a zero-sized array
			int16 params[];
#pragma warning( pop )
		};

		// halo script accessible value
		struct hs_global_definition
		{
#if PLATFORM_IS_EDITOR
			cstring name;
#endif
			_enum type;
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
		};


		struct hs_syntax_node : Memory::s_datum_base
		{
			union {
				int16 ConstantType;
				int16 FunctionIndex;
			};
			int16 Type;
			union {
				uint16 Flags;
				int16 PointerType;
			};
			datum_index NextExpression;
			int32 Pointer;
			union {
				void* Address;

				struct {
					bool _bool;
					real _real;
					int16 _short;
					int32 _long;
					datum_index _datum;
				}Value;
			};
		};
	};
};