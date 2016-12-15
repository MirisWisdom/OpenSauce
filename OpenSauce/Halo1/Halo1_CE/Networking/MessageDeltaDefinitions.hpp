/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/networking/message_delta_definitions.hpp>

namespace Yelo
{
	namespace MessageDeltas
	{
#pragma region packet field types
		typedef unsigned char	integer_small;
		typedef short			integer_medium;
		typedef long			integer_large;
//		typedef float			real;
		typedef bool			boolean;
		typedef unsigned char	ascii_character;
		typedef wchar_t			wide_character;
		typedef unsigned char	ascii_string_t;
		typedef wchar_t			wide_string_t;
		typedef unsigned char	arbitary_data_t;
		

		typedef const void*		pointer_t;
		typedef short			enumeration_t;
		
		typedef long			translated_index_t;

		struct					point2d
		{
			real x;
			real y;
		};
		struct					point3d
		{
			real x;
			real y;
			real z;
		};

		struct					vector2d
		{
			real i;
			real j;
		};
		struct					vector3d
		{
			real i;
			real j;
			real k;
		};
//		typedef float			angle;
//		typedef unsigned long	time_t;
		struct					grenade_counts
		{
			integer_small plasma;
			integer_small frags;
		};

		typedef real			fixed_width_1bit;
		typedef real			fixed_width_3bits;
		typedef real			fixed_width_6bits;

		typedef struct			t_fixed_width_normal_4bit
		{
			real i;
			real j;
			real k;
		}fixed_width_normal_4bit_t, fixed_width_normal_8bit_t;
		typedef struct			t_fixed_width_normal_16bit
		{
			real i;
			real j;
			real k;
			real w;
		}fixed_width_normal_16bit_t;
		struct					locality_reference_position
		{
			real x;
			real y;
			real z;
		};
		struct					digital_throttle
		{
			real unknown1;
			real unknown2;
		};
		typedef short			fixed_width_weapon_index_t;
		typedef short			fixed_width_grenade_index_t;
		typedef struct			t_smart_vector
		{
			real i;
			real j;
			real k;
		}smart_vector_t, angular_velocity_t, translational_velocity_t;
		struct					item_placement_position
		{
			real x;
			real y;
			real z;
		};
#pragma endregion


#pragma region packet non generic types
		typedef unsigned char	local_player_update_sequence_number;
		typedef unsigned char	remote_player_update_sequence_number;
		typedef unsigned char	remote_player_action_update_baseline_id;
		typedef long			update_tick_count;
		typedef unsigned char	update_id_t;
		typedef datum_index		object_index_t;
		typedef datum_index		player_index_t;
		typedef datum_index		definition_index_t;
		struct					control_flags
		{
			unsigned long flag1 : 1;
			unsigned long flag2 : 1;
			unsigned long flag3 : 1;
			unsigned long flag4 : 1;
			unsigned long flag5 : 1;
			unsigned long flag6 : 1;
			unsigned long flag7 : 1;
			unsigned long flag8 : 1;
			unsigned long flag9 : 1;
			unsigned long flagA : 1;
			unsigned long flagB : 1;
			unsigned long flagC : 1;
			unsigned long flagD : 1;
			unsigned long flagE : 1;
			unsigned long flagF : 1;
		}; BOOST_STATIC_ASSERT( sizeof(control_flags) == 0x4 );

		typedef ascii_character	map_name[128];
		struct					network_map
		{
			integer_large		version;
			map_name			name;
		};
		typedef wide_character	network_player_name[12];
		struct					network_player
		{
			network_player_name	name[2];
			integer_medium		primary_color_index;
			integer_medium		icon_index;
			integer_small		machine_index;
			integer_small		controller_index;
			integer_small		team_index;
			integer_small		player_list_index;
		};
		typedef wide_character	network_game_name[64];
		typedef network_player	network_game_players[16];
		typedef wide_character	game_variant_human_readable_description[24];
		struct					universal_variant_flags
		{
			unsigned short flag1 : 1;
			unsigned short flag2 : 1;
			unsigned short flag3 : 1;
			unsigned short flag4 : 1;
			unsigned short flag5 : 1;
			unsigned short flag6 : 1;
			unsigned short flag7 : 1;
			unsigned short flag8 : 1;
			unsigned short flag9 : 1;
			PAD16;
		}; BOOST_STATIC_ASSERT( sizeof(universal_variant_flags) == 0x4 );
		struct					game_variant_flags
		{
			unsigned char flag1 : 1;
			PAD24;
		}; BOOST_STATIC_ASSERT( sizeof(game_variant_flags) == 0x4 );
		typedef integer_small	game_engine_variant[56];
		struct					universal_variant
		{
			boolean					teams;
			integer_small			pad0;
			integer_small			pad1;
			integer_small			pad2;
			universal_variant_flags flags;
			integer_large			goal_radar;
			boolean					odd_man_out;
			integer_small			pad4;
			integer_small			pad5;
			integer_small			pad6;
			integer_large			respawn_time_growth;
			integer_large			respawn_time;
			integer_large			suicide_penalty;
			integer_large			lives;
			real					health;
			integer_large			score_to_win;
			integer_large			weapon_set;
			integer_large			vehicle_set;
			integer_large			vehicle_set2;
			integer_large			vehicles_respawn;
			integer_small			friendly_fire;
			PAD24;
			integer_large			friendly_fire_penalty;
			boolean					team_autobalance;
			PAD24;
			integer_large			time_limit;
		};
		struct						game_variant
		{
			game_variant_human_readable_description game_varient;
			integer_large							game_engine_index;
			universal_variant						universal_variant;
			game_engine_variant						game_engine_variant;
			game_variant_flags						flags;
		};
		typedef integer_large	parameters_protocol_array[64];
		typedef wide_character	hud_chat_message[255+1];
		typedef pointer_t		hud_chat_message_ptr;
		typedef real_rgb_color	object_change_colors[4];
		typedef integer_large	ctf_score_array[2];
		typedef integer_large	race_score_array[16];
		typedef integer_large	king_score_array[16];
		typedef integer_large	oddball_score_array[16];
		typedef integer_large	oddball_owner_array[16];
		typedef integer_large	slayer_score_array[16];
		struct					damage_data_flags
		{
			unsigned char flag1 : 1;
			unsigned char flag2 : 1;
			unsigned char flag3 : 1;
			unsigned char flag4 : 1;
			unsigned char flag5 : 1;
			unsigned char flag6 : 1;
			unsigned char flag7 : 1;
			unsigned char flag8 : 1;
			PAD24;
		}; BOOST_STATIC_ASSERT( sizeof(damage_data_flags) == 0x4 );
		typedef ascii_character motd_text[255+1];
		typedef ascii_character rcon_response[80+1];
		typedef ascii_character rcon_password[8+1];
		typedef ascii_character rcon_command[64+1];
#pragma endregion

		struct message_delta_guaranteed_object_header
		{
			object_index_t	_object_index;
		};

		struct message_delta_game_object_header
		{
			object_index_t	_object_index;
			integer_small	baseline_index;
			integer_small	message_index;
			boolean			update_baseline;
			PAD8;
		};

		struct message_delta_game_object_header_timestamped
		{
			object_index_t	_object_index;
			integer_small	baseline_index;
			integer_small	message_index;
			boolean			update_baseline;
			PAD8;
			time_t			timestamp;
		};

		struct biped_update_header
		{
			object_index_t	_object_index;
			integer_small	baseline_index;
			integer_small	message_index;
			boolean			update_baseline;
			boolean			force_shield_update;
		};

		struct ctf_update_header
		{
			integer_large	flag_swap_timer;
		};

		struct client_game_update_header
		{
			update_id_t		id;
		};

		struct player_score_update_header
		{
			player_index_t	player;
		};

		struct remote_player_action_update_header
		{
			player_index_t	player;
			update_id_t		update_id;
			remote_player_action_update_baseline_id baseline_id;
		};

		struct remote_player_position_update_header
		{
			player_index_t	player_index;
			update_id_t		update_id;
			remote_player_action_update_baseline_id	baseline_id;
		};

		struct remote_player_total_update_header
		{
			remote_player_action_update_header		action_header;
			remote_player_update_sequence_number	sequence_number;
		};




		struct hud_chat_network_data
		{
			integer_small msg_type; // [Enums::hud_chat_type], see GameUI.hpp
			PAD24;
			integer_small player_number;
			PAD24;
			hud_chat_message_ptr message;
		};
	};
};