/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum unit_grenade_type : _enum
		{
			_unit_grenade_type_frag,
			_unit_grenade_type_plasma,

			// Stock number of grenade types
			k_unit_grenade_types_count,

			_unit_grenade_type_yelo2 = k_unit_grenade_types_count,	// ie, "custom 2"
			_unit_grenade_type_yelo3,								// ie, "custom 3"

			// Number of supported grenade types in OS-modified game states
			k_unit_grenade_types_count_yelo
		};

		enum game_difficulty_value
		{
			//////////////////////////////////////////////////////////////////////////
			// health
			_game_difficulty_value_enemy_damage,
			_game_difficulty_value_enemy_body_vitality,
			_game_difficulty_value_enemy_shield_vitality,
			_game_difficulty_value_enemy_shield_recharge,
			_game_difficulty_value_friend_damage,
			_game_difficulty_value_friend_body_vitality,
			_game_difficulty_value_friend_shield_vitality,
			_game_difficulty_value_friend_shield_recharge,
			_game_difficulty_value_infection_forms_toughness,
			_game_difficulty_value_health_unused9,
			//////////////////////////////////////////////////////////////////////////
			// ranged fire
			_game_difficulty_value_rate_of_fire,
			_game_difficulty_value_projectile_error,
			_game_difficulty_value_burst_error,
			_game_difficulty_value_new_target_delay,
			_game_difficulty_value_burst_separation,
			_game_difficulty_value_target_tracking,
			_game_difficulty_value_target_leading,
			_game_difficulty_value_overcharge_chance,
			_game_difficulty_value_special_fire_delay,
			_game_difficulty_value_projectile_guidance_velocity_scale,
			_game_difficulty_value_melee_delay_scale,
			_game_difficulty_value_combat_unused21, // 0x15
			//////////////////////////////////////////////////////////////////////////
			// grenades
			_game_difficulty_value_grenade_chance_scale,
			_game_difficulty_value_grenade_timer_scale,
			_game_difficulty_value_grenades_unused24, // 0x18
			_game_difficulty_value_grenades_unused25, // 0x19
			_game_difficulty_value_grenades_unused26, // 0x1A
			//////////////////////////////////////////////////////////////////////////
			// placement
			_game_difficulty_value_actor_major_upgrade_normal,
			_game_difficulty_value_actor_major_upgrade_few,
			_game_difficulty_value_actor_major_upgrade_many,

			_game_difficulty_value_unused30, // 0x1E, in Halo2, this is the vehicle ram chance
			_game_difficulty_value_unused31, // 0x1F
			_game_difficulty_value_unused32, // 0x20
			_game_difficulty_value_unused33, // 0x21
			_game_difficulty_value_unused34, // 0x22

			k_number_of_game_difficulty_values
		};
	};

	namespace GameState
	{
		struct s_game_globals;

		s_game_globals*				GameGlobals();
	};
};