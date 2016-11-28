/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/effects/material_effect_definitions.hpp>
#include <blamlib/Halo1/game/game_allegiance.hpp>

namespace Yelo
{
	namespace Flags
	{
		enum object_damage_flags
		{
			_object_damage_flag_body_depleted		= 1 << 0,
			_object_damage_flag_destroyed_a_region	= 1 << 1,
			_object_damage_flag_destroyed_object	= 1 << 2,
			_object_damage_flag_shield_depleted		= 1 << 3,
			_object_damage_flag_is_enemy_team		= 1 << 4,
			_object_damage_flag_unknown5			= 1 << 5,
			_object_damage_flag_unknown6			= 1 << 6,
			_object_damage_flag_unknown7			= 1 << 7,

			_object_damage_flags
		};

		enum damage_data_flags : _enum
		{
			// Incomplete
			_damage_data_flags_affect_target_only_bit,

			_damage_data_flags
		};
	};

	namespace Objects
	{
		struct s_damage_data
		{
			datum_index effect_definition_index;			// 0x0
			long_flags flags;								// 0x4
			datum_index responsible_player_index;			// 0x8
			datum_index responsible_unit_index;				// 0xC
			Enums::game_team responsible_units_team;		// 0x10
			PAD16;											// 0x12
			s_scenario_location location;					// 0x14
			real_point3d damage_position;					// 0x1C
			UNKNOWN_TYPE(real_vector3d); // position		// 0x28
			UNKNOWN_TYPE(real_vector3d); // direction?		// 0x34
			// scale examples:
			// AOE distance scale (via radius)
			// unit blip type (medium=.5, small=.25, large=1)
			real scale;										// 0x40
			real damage_multiplier;							// 0x44
			UNKNOWN_TYPE(real);								// 0x48 shield or body amount prior to the damage was applied
			Enums::material_type material;					// 0x4C
			PAD16;											// 0x4E
			UNKNOWN_TYPE(int32); // only seen this written to, and when it was it was set to a s_projectile_material_response_definition*
		}; BOOST_STATIC_ASSERT( sizeof(s_damage_data) == 0x54 );
	};

	namespace blam
	{
		using namespace Yelo::Objects;

		// TODO: move into damage.cpp
		API_INLINE void damage_data_new(s_damage_data& data, datum_index damage_effect_definiton_index)
		{
			memset(&data, 0, sizeof(data));
			data.effect_definition_index = damage_effect_definiton_index;
			data.material = Enums::_material_type_none;
			data.responsible_player_index = datum_index::null;
			data.responsible_unit_index = datum_index::null;
			data.responsible_units_team = Enums::_game_team_none;
			data.location.cluster_index = NONE;
			data.scale = 1.0f;
			data.damage_multiplier = 1.0f;
		}

		bool PLATFORM_API object_restore_body(datum_index object_index);

		void PLATFORM_API object_deplete_body(datum_index object_index);

		void PLATFORM_API object_deplete_shield(datum_index object_index);

		void PLATFORM_API object_double_charge_shield(datum_index object_index);

		void PLATFORM_API area_of_effect_cause_damage(s_damage_data& data, datum_index unused_object_index = datum_index::null);

		void PLATFORM_API object_cause_damage(s_damage_data& data, datum_index damaged_object_index, 
			int16 node_index = NONE, int16 region_index = NONE, int16 damage_materials_element_index = NONE, 
			real_vector3d* normal = NULL);
	};
};