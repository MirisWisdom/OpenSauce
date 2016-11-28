/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/players.hpp>
#include <blamlib/Halo1/objects/vehicle_remapper.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum game_variant_goal_radar : long_enum
		{
			_game_variant_goal_radar_motion_tracker,
			_game_variant_goal_radar_navpoints,
			_game_variant_goal_radar_none,

			_game_variant_goal_radar,
		};

		enum game_variant_weapon_set : long_enum
		{
			_game_variant_weapon_set_normal,
			_game_variant_weapon_set_pistols,
			_game_variant_weapon_set_rifles,
			_game_variant_weapon_set_plasma,
			_game_variant_weapon_set_sniper,
			_game_variant_weapon_set_sans_snipers,
			_game_variant_weapon_set_rockets,
			_game_variant_weapon_set_shotguns,
			_game_variant_weapon_set_short_range,
			_game_variant_weapon_set_human,
			_game_variant_weapon_set_covenant,
			_game_variant_weapon_set_classic,
			_game_variant_weapon_set_heavies,

			_game_variant_weapon_set,
		};

		enum game_trait : long_enum
		{
			_game_trait_none,
			_game_trait_invisible,
			_game_trait_extra_damage,
			_game_trait_damage_resistant,
			_game_trait,
		};

		enum oddball_carrier_speed : long_enum
		{
			_oddball_carrier_speed_normal,
			_oddball_carrier_speed_faster,
			_oddball_carrier_speed_slower,

			_oddball_carrier_speed,
		};

		enum oddball_ball_type : long_enum
		{
			_oddball_ball_type_normal,
			_oddball_ball_type_reverse_tag,
			_oddball_ball_type_juggernaut,

			_oddball_ball_type,
		};

		enum race_type : long_enum
		{
			_race_type_normal,
		};
	};

	namespace Flags
	{
		enum game_variant_flags : long_flags
		{
			_game_variant_show_players_on_radar_bit,
			_game_variant_show_homies_bit,
			_game_variant_infinite_grenades_bit,
			_game_variant_players_have_shields_bit,
			_game_variant_invisible_players_bit,
			_game_variant_use_custom_start_equipment_bit,
		};
	};

	namespace GameEngine
	{
		struct s_universal_variant
		{
			bool							teams;
			PAD24;										// these three pad bytes are encoded in game settings update messages...
			Flags::game_variant_flags		flags;
			Enums::game_variant_goal_radar	goal_rader;
			bool odd_man_out;
			PAD24;										// these three pad bytes are encoded in game settings update messages...

			int32							respawn_time_growth;
			int32							respawn_time;
			int32							suicide_penalty;
			int32							lives;

			real							health; // I thought this was damage_multiplier...
			int32							score_to_win;
			Enums::game_variant_weapon_set	weapon_set;

			Objects::s_vehicle_set			vehicle_set[Enums::k_number_of_multiplayer_teams];
			int32							vehicles_respawn;

			byte							friendly_fire; // need an enum for this
			PAD24;
			int32							friendly_fire_penalty;
			bool							team_autobalance;
			PAD24;

			int32							time_limit;
		}; BOOST_STATIC_ASSERT( sizeof(s_universal_variant) == 0x48 );

		union s_game_engine_variant
		{
			// The message delta definition for this struct encodes this as a byte[], so everything is copied across the wire
			enum { k_max_variant_size = 0x38 };

			PAD(0, k_max_variant_size);

			struct s_ctf {
				bool assault;
				PAD8; // unused...

				struct {
					bool reset;
					bool at_home_to_score;
					int32 flag_type; // 1 = single
				}flag;
			}ctf;
			BOOST_STATIC_ASSERT( sizeof(s_ctf) == 0x8 );

			struct s_slayer {
				bool death_handicap; // True: killed player's speed increases in magnitudes of one
				bool kill_handicap; // True: killer's speed decreases in magnitudes of two
				bool kill_in_order;
			}slayer;
			BOOST_STATIC_ASSERT( sizeof(s_slayer) == 0x3 );

			struct s_oddball {
				bool random_start;
				PAD24;
				Enums::oddball_carrier_speed ball_speed;

				Enums::game_trait nonball_trait;
				Enums::game_trait ball_trait;
				Enums::oddball_ball_type type;
				int32 ball_count;
			}oddball;
			BOOST_STATIC_ASSERT( sizeof(s_oddball) == 0x18 );

			struct s_king {
				bool moving_hill;
			}king;
			BOOST_STATIC_ASSERT( sizeof(s_king) == 0x1 );

			struct s_race {
				Enums::race_type race_type;
				long_enum team_scoring;
			}race;
			BOOST_STATIC_ASSERT( sizeof(s_race) == 0x8 );

		}; BOOST_STATIC_ASSERT( sizeof(s_game_engine_variant) == s_game_engine_variant::k_max_variant_size );

		struct s_game_variant
		{
			wchar_t name[24];							// 0x0
			long_enum game_engine_index;				// 0x30 Enums::game_engine
			s_universal_variant universal_variant;		// 0x34
			PAD64 PAD64 PAD64 PAD64;					// 0x7C, unused...
			s_game_engine_variant game_engine_variant;	// 0x9C
			long_flags flags;							// 0xD4
			//uint32 crc;								// 0xD8, not actually part of the structure, only "used" when presisting to/from files
		}; BOOST_STATIC_ASSERT( sizeof(s_game_variant) == 0xD8/*0xDC*/ );
	};
};