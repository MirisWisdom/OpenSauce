/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/hs/hs_types.hpp>

#include <blamlib/Halo1/ai/actor_types.hpp>						// for g_actor_type_names
#include <blamlib/Halo1/ai/actor_definitions.hpp>				// for k_hs_tag_reference_type_group_tags
#include <blamlib/Halo1/ai/actors.hpp>							// for g_ai_default_state_names
#include <blamlib/Halo1/effects/damage_effect_definitions.hpp>	// for k_hs_tag_reference_type_group_tags
#include <blamlib/Halo1/effects/effect_definitions.hpp>			// for k_hs_tag_reference_type_group_tags
#include <blamlib/Halo1/game/game.hpp>							// for g_game_difficulty_level_names
#include <blamlib/Halo1/game/game_allegiance.hpp>				// for g_game_team_names
#include <blamlib/Halo1/hs/hs_types_traits.hpp>
#include <blamlib/Halo1/interface/hud_definitions.hpp>			// for g_hud_anchor_names
#include <blamlib/Halo1/models/model_animation_definitions.hpp>	// for k_hs_tag_reference_type_group_tags
#include <blamlib/Halo1/objects/object_definitions.hpp>			// for k_hs_tag_reference_type_group_tags
#include <blamlib/Halo1/objects/object_types.hpp>
#include <blamlib/Halo1/sound/sound_definitions.hpp>			// for k_hs_tag_reference_type_group_tags

#include <blamlib/Halo1/hs/hs_types_compile.inl>
#include <blamlib/Halo1/hs/hs_types_runtime.inl>

namespace Yelo
{
	namespace Scripting
	{
		proc_hs_typecast k_hs_typecasting_procedures[Enums::k_number_of_hs_types][Enums::k_number_of_hs_types];
		static void HsTypesInitializeTypeCastingProcedures()
		{
			using namespace Enums;
			using namespace blam;

			typedef proc_hs_typecast (&typecasting_procedures_t)[k_number_of_hs_types];

			typecasting_procedures_t
				void_procedures = k_hs_typecasting_procedures[_hs_type_void],
				bool_procedures = k_hs_typecasting_procedures[_hs_type_bool],
				real_procedures = k_hs_typecasting_procedures[_hs_type_real],
				short_procedures = k_hs_typecasting_procedures[_hs_type_short],
				long_procedures = k_hs_typecasting_procedures[_hs_type_long],
				string_procedures = k_hs_typecasting_procedures[_hs_type_string],

				object_list_procedures = k_hs_typecasting_procedures[_hs_type_object_list]
				;

#if 0
			// NOTE: non-standard
			for (_enum type = _hs_type_enum__first; type < _hs_type_enum__last+1; type++)
				k_hs_typecasting_procedures[type][_hs_type_real] = hs_real_to_enum;
#endif

			// _hs_type_void
			for (_enum type = _hs_type_data__first; type < _hs_type_data__last+1; type++)
				void_procedures[type] = hs_value_to_void;
			
			// _hs_type_bool
			bool_procedures[_hs_type_real] =	hs_long_to_boolean; // yes long, (this is what the engine does)
			bool_procedures[_hs_type_short] =	hs_short_to_boolean;
			bool_procedures[_hs_type_long] =	hs_long_to_boolean;
			bool_procedures[_hs_type_string] =	hs_string_to_boolean;
			
			// _hs_type_real
			real_procedures[_hs_type_bool] =	hs_bool_to_real;
			real_procedures[_hs_type_short] =	hs_short_to_real;
			real_procedures[_hs_type_long] =	hs_long_to_real;
//			real_procedures[_hs_type_string] =	hs_string_to_real;
			for (_enum type = _hs_type_enum__first; type < _hs_type_enum__last+1; type++)
				real_procedures[type] = hs_enum_to_real;

			// _hs_type_short
			short_procedures[_hs_type_bool] =	hs_bool_to_short;
			short_procedures[_hs_type_real] =	hs_real_to_short;
			short_procedures[_hs_type_long] =	hs_long_to_short;
//			short_procedures[_hs_type_string] =	hs_string_to_short;

			// _hs_type_long
			long_procedures[_hs_type_bool] =	hs_bool_to_long;
			long_procedures[_hs_type_real] =	hs_real_to_long;
			// NOTE: due to a typo, the engine actually didn't support casting from short to long
			long_procedures[_hs_type_short] =	hs_short_to_long;
//			long_procedures[_hs_type_string] =	hs_string_to_long;

			// _hs_type_string
			
			// _hs_type_object_list
			object_list_procedures[_hs_type_ai] = object_list_from_ai_reference;
			for (_enum type = _hs_type_object__first; type < _hs_type_object__last+1; type++)
				object_list_procedures[type] = hs_object_to_object_list;
			for (_enum type = _hs_type_object_name__first; type < _hs_type_object_name__last+1; type++)
				object_list_procedures[type] = hs_object_name_to_object_list;
		}

		void HsTypesInitialize()
		{
			HsTypesInitializeTypeCastingProcedures();
		}
		void HsTypesDispose()
		{
		}

#define HS_TYPE_ABI_NAME(_hs_type)				k##_hs_type##_abi

#define HS_TYPE_ABI(abi_type, _hs_type, ...)	static const abi_type HS_TYPE_ABI_NAME(_hs_type) = { Enums::_hs_type, __VA_ARGS__ };
#include <blamlib/Halo1/hs/hs_types_definitions.inl>
#undef HS_TYPE_ABI

		const c_hs_type_abi* c_hs_type_abi::Get(Enums::hs_type type)
		{
			static std::array<const c_hs_type_abi*, Enums::k_number_of_hs_types> hs_type_abis = {

#define HS_TYPE_ABI(abi_type, _hs_type, ...)	&HS_TYPE_ABI_NAME(_hs_type),
#include <blamlib/Halo1/hs/hs_types_definitions.inl>
#undef HS_TYPE_ABI

			};

			return hs_type_abis[type];
		}
#undef HS_TYPE_ABI_NAME

		cstring c_hs_type_abi::GetTypeName() const
		{
			return blam::hs_type_names[m_type];
		}
	};

	namespace blam
	{
		using namespace Scripting;

		bool hs_type_valid(_enum type)
		{
			return type >= Enums::_hs_type_void && type < Enums::k_number_of_hs_types;
		}
		bool hs_type_is_enum(_enum type)
		{
			return type >= Enums::_hs_type_enum__first && type < Enums::_hs_type_enum__last;
		}
		bool hs_type_is_tag_reference(_enum type)
		{
			return type >= Enums::_hs_type_tag_reference__first && type < Enums::_hs_type_tag_reference__last;
		}
		bool hs_type_is_object(_enum type)
		{
			return type >= Enums::_hs_type_object__first && type < Enums::_hs_type_object__last;
		}
		bool hs_type_is_object_name(_enum type)
		{
			return type >= Enums::_hs_type_object_name__first && type < Enums::_hs_type_object_name__last;
		}


		const std::array<word_flags, Enums::k_number_of_hs_object_types> k_hs_object_type_masks = {
			Enums::_object_type_mask_all, // engine actually uses 0xFFFF here
			Enums::_object_type_mask_unit,
			FLAG(Enums::_object_type_vehicle),
			FLAG(Enums::_object_type_weapon),
			Enums::_object_type_mask_device,
			FLAG(Enums::_object_type_scenery),
		};
		const std::array<tag, Enums::_hs_type_tag_reference__count> k_hs_tag_reference_type_group_tags = {
			TagGroups::sound_definition::k_group_tag,
			TagGroups::effect_definition::k_group_tag,
			TagGroups::s_damage_effect_definition::k_group_tag,	// damage
			TagGroups::looping_sound_definition::k_group_tag,
			TagGroups::model_animation_graph::k_group_tag,
			TagGroups::s_actor_variant_definition::k_group_tag,
			TagGroups::s_damage_effect_definition::k_group_tag,	// damage_effect
			TagGroups::s_object_definition::k_group_tag,
		};

		std::array<cstring, Enums::k_number_of_hs_types> hs_type_names = {
			"unparsed",
			"special_form",		// NOTE: changed spaces to underscores
			"function_name",	// NOTE: changed spaces to underscores
			"passthrough",
			"void",

			"boolean",
			"real",
			"short",
			"long",
			"string",
			"script",

			"trigger_volume",
			"cutscene_flag",
			"cutscene_camera_point",
			"cutscene_title",
			"cutscene_recording",
			"device_group",
			"ai",
			"ai_command_list",
			"starting_profile",
			"conversation",

			"navpoint",
			"hud_message",

			"object_list",

			"sound",
			"effect",
			"damage",
			"looping_sound",
			"animation_graph",
			"actor_variant",
			"damage_effect",
			"object_definition",

			"game_difficulty",
			"team",
			"ai_default_state",
			"actor_type",
			"hud_corner",

			"object",
			"unit",
			"vehicle",
			"weapon",
			"device",
			"scenery",

			"object_name",
			"unit_name",
			"vehicle_name",
			"weapon_name",
			"device_name",
			"scenery_name",
		};

		std::array<cstring, Enums::k_number_of_hs_script_types> hs_script_type_names = {
			"startup",
			"dormant",
			"continuous",
			"static",
			"stub",
		};

		// should be in game/game.cpp
		cstring g_game_difficulty_level_names[Enums::k_number_of_game_difficulty_levels] = {
			"easy",
			"normal",
			"hard",
			"impossible",
		};
		// should be in game/game_allegiance.cpp
		cstring g_game_team_names[Enums::k_number_of_game_teams] = {
			"default",
			"player",
			"human",
			"covenant",
			"flood",
			"sentinel",
			"unused6",
			"unused7",
			"unused8",
			"unused9",
		};
		// should be in ai/ai_scenario_definitions.cpp
		cstring g_ai_default_state_names[Enums::k_number_of_actor_default_states] = {
			"none",
			"sleep",
			"alert",
			"move_repeat",
			"move_loop",
			"move_loop_back_and_forth",
			"move_loop_random",
			"move_random",
			"guard",
			"guard_at_position",
			"search",
			"flee",
		};
		// should be in ai/actor_types.cpp
		cstring g_actor_type_names[Enums::k_number_of_actor_types] = {
			"elite",
			"jackal",
			"grunt",
			"hunter",
			"engineer",
			"assassin",
			"player",
			"marine",
			"crew",
			"combat_form",
			"infection_form",
			"carrier_form",
			"monitor",
			"sentinel",
			"none",
			"mounted_weapon",
		};
		// should be in interface/hud_definitions.cpp
		cstring g_hud_anchor_names[Enums::k_number_of_hud_anchors] = {
			"top_left",
			"top_right",
			"bottom_left",
			"bottom_right",
			"center",
		};
		std::array<const string_list, Enums::_hs_type_enum__count> hs_enum_table = {
			string_list { NUMBEROF(g_game_difficulty_level_names),	&g_game_difficulty_level_names[0] },
			string_list { NUMBEROF(g_game_team_names),				&g_game_team_names[0] },
			string_list { NUMBEROF(g_ai_default_state_names),		&g_ai_default_state_names[0] },
			string_list { NUMBEROF(g_actor_type_names),				&g_actor_type_names[0] },
			string_list { NUMBEROF(g_hud_anchor_names),				&g_hud_anchor_names[0] },
		};
	};
};