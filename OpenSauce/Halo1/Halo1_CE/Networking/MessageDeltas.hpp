/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
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

#include "Networking/Networking.hpp"

namespace Yelo
{
	namespace Enums
	{
#pragma region field_type
		enum field_type : long_enum
		{
			_field_type_integer,
			_field_type_real,
			_field_type_boolean,
			_field_type_ascii_character,
			_field_type_wide_character,
			_field_type_ascii_string,
			_field_type_wide_string,
			_field_type_arbitary_data,
			_field_type_array,
			_field_type_structure,
			_field_type_pointer,
			_field_type_enumeration,
			_field_type_bounded_index,
			_field_type_translated_index,


			/*!
			* \brief
			* real point 2d\3d
			*
			* parameter specifies if its 2d or 3d.
			* bit count = (parameter << 5) + parameter
			*/
			_field_type_point,

			/*!
			* \brief
			* real vector 2d\3d
			*
			* parameter specifies if its 2d or 3d.
			* bit count = (parameter << 5) + parameter
			*/
			_field_type_vector,
			_field_type_angle,
			_field_type_flags,
			_field_type_time,
			_field_type_grenade_counts,
			_field_type_fixed_width,
			_field_type_fixed_width_normal_vector,
			_field_type_locality_reference_position,
			_field_type_digital_throttle,
			_field_type_fixed_width_weapon_index,
			_field_type_fixed_width_grenade_index,
			_field_type_smart_vector,
			_field_type_item_placement_position,

			NUMBER_OF_MESSAGE_DELTA_FIELD_TYPES,
		};
#pragma endregion

#pragma region field_type_bit_size
		/*!
		* \brief
		* Bit sizes of some message delta fields
		* 
		* Each enumeration has the value of a message
		* delta's constant field bit count (unlike
		* the integer which has various bit sizes)
		*/
		enum field_type_bit_size
		{
			_field_type_bit_size_real = 32,
			_field_type_bit_size_boolean = 1,
			_field_type_bit_size_ascii_character = 8,
			_field_type_bit_size_wide_character = 16,
			_field_type_bit_size_angle = 32,
			_field_type_bit_size_time = 32,
			_field_type_bit_size_grenade_counts = 6,
			_field_type_bit_size_digital_throttle = 4,
			_field_type_bit_size_fixed_width_weapon_index = 3,
			_field_type_bit_size_fixed_width_grenade_index = 2,
			_field_type_bit_size_item_placement_position = 20 /*X*/ + 20 /*Y*/ + 30 /*Z*/,
		};
#pragma endregion

#pragma region integer_width_type
		enum integer_width_type : long_enum
		{
			_integer_width_type_8bits,
			_integer_width_type_16bits,
			_integer_width_type_32bits,
			_integer_width_type_1bit,
			_integer_width_type_3bits,
			_integer_width_type_5bits,
			_integer_width_type_6bits,
		};
#pragma endregion

		enum enumeration_width_type : long_enum
		{
			_enumeration_width_type_1byte,
			_enumeration_width_type_2byte,
			_enumeration_width_type_4byte,
		};

		enum message_delta_mode : long_enum
		{
			_message_delta_mode_stateless, // no baselines
			_message_delta_mode_incremental, // requires baselines
		};

		enum message_delta_encoding_class : long_enum
		{
			_message_delta_encoding_class_lan,
			_message_delta_encoding_class_internet,
		};

		enum message_delta
		{
			_message_delta_object_deletion,
			_message_delta_projectile_update,
			_message_delta_equipment_update,
			_message_delta_weapon_update,
			_message_delta_biped_update,
			_message_delta_vehicle_update,
			_message_delta_hud_add_item,
			_message_delta_player_create,
			_message_delta_player_spawn,
			_message_delta_player_exit_vehicle,
			_message_delta_player_set_action_result,
			_message_delta_player_effect_start,
			_message_delta_unit_kill,
			_message_delta_client_game_update,
			_message_delta_player_handle_powerup,
			_message_delta_hud_chat,
			_message_delta_slayer_update,
			_message_delta_ctf_update,
			_message_delta_oddball_update,
			_message_delta_king_update,
			_message_delta_race_update,
			_message_delta_player_score_update,
			_message_delta_game_engine_change_mode,
			_message_delta_game_engine_map_reset,
			_message_delta_multiplayer_hud_message,
			_message_delta_multiplayer_sound,
			_message_delta_team_change,
			_message_delta_unit_drop_current_weapon,
			_message_delta_vehicle_new,
			_message_delta_biped_new,
			_message_delta_projectile_new,
			_message_delta_equipment_new,
			_message_delta_weapon_new,
			_message_delta_game_settings_update,
			_message_delta_parameters_protocol,
			_message_delta_local_player_update,
			_message_delta_local_player_vehicle_update,
			_message_delta_remote_player_action_update,
			_message_delta_super_remote_players_action_update,
			_message_delta_remote_player_position_update,
			_message_delta_remote_player_vehicle_update,
			_message_delta_remote_player_total_update_biped,
			_message_delta_remote_player_total_update_vehicle,
			_message_delta_weapon_start_reload,
			_message_delta_weapon_ammo_pickup_mid_reload,
			_message_delta_weapon_finish_reload,
			_message_delta_weapon_cancel_reload,
			_message_delta_netgame_equipment_new,
			_message_delta_projectile_detonate,
			_message_delta_item_accelerate,
			_message_delta_damage_dealt,
			_message_delta_projectile_attach,
			_message_delta_client_to_server_pong,
			_message_delta_super_ping_update,
			_message_delta_sv_motd,
			_message_delta_rcon_request,
			_message_delta_rcon_response,

			k_message_deltas_count
		};

		enum {
			k_mdp_maximum_string_length = 0x7FF,
			k_mdp_maximum_flags = 32,
		};
	};

	namespace MessageDeltas
	{
		void Initialize();
		void Dispose();

		Enums::message_delta_encoding_class* EncodingClass();
		// Buffer that holds all received data from the clients or the server depending on the game connection
		byte* PacketBufferReceived();
		// Buffer that holds all data to be sent to a client or to the server depending on the game connection
		byte* PacketBufferSent();

#ifndef YELO_NO_NETWORK

#ifdef API_DEBUG
		extern cstring message_delta_to_string_table[];
#endif

		// Writes [data_size_in_bits] of the packet buffer to the server connection
		// returns the size in bits of the header
		int32 SvWrite(int32 data_size_in_bits, 
			int32 /*bool*/ dont_bit_encode = true, int32 /*bool*/ dont_flush = false);

		// [message_delta_mode]
		// [message_delta]
		// pointer array (0,1-n) to the to headers of [datas]
		// pointer array (0?,1-n) to the data structures to encode
		// pointer array (0,1-n) for the base line updates for each of the datas
		// # of iterations
		// ?
		int32 EncodeStateless(long_enum mode, long_enum def_type, void** headers, void** datas, 
			void** baselines = NULL, int32 iterations = 1, int32 /*bool*/ unk = false);

		// Decodes the current data in the packet buffer, and puts the data into [header] if there is a header, and [out_buffer] for the main packet body
		bool DecodeStatelessIterated(void* header, void* out_buffer);

		//bool ConnectionFlush(int32 /*bool*/ unk = 1);

		// Sends [data_size_in_bits] of the packet buffer to the server
		void ClientSendMessageToServer(int32 data_size_in_bits);

		// Sends [data_size_in_bits] of the packet buffer to all machines
		bool SvSendMessageToAll(int32 data_size_in_bits, 
			int32 /*bool*/ dont_bit_encode = true, int32 /*bool*/ dont_flush = false, int32 /*bool*/ send_even_after_fail = false, int32 shit = -1);

		// Sends [data_size_in_bits] of the packet buffer to [machine_index]
		bool SvSendMessageToMachine(int32 machine_index, int32 data_size_in_bits, 
			int32 /*bool*/ dont_bit_encode = true, int32 /*bool*/ dont_flush = false, int32 /*bool*/ send_even_after_fail = false, int32 shit = -1);

		// This is called when in a decode function for a mdp type, the call to network_decode_message fails, so it removes the data sent to this machine
		void DiscardIterationBody(void* header);
#endif

		// TODO: I've had some issues with disassembling s_entry and s_slot's structure definitions..
		// Just assume they are wrong until this comment is gone.
		struct s_index_resolution_table
		{
			struct s_entry
			{
				uint32 index;
				uint32 value;
				struct s_slot* parent;
			};

			struct s_slot
			{
				enum { k_entry_count = 50 };

				s_entry* entries;
				s_slot* prev;
			};

			bool is_initialized; PAD24;
			int32 number_of_slots;
			s_slot* slots;
			int32 slots_in_use;
			s_slot* entry_free_list;
			s_slot* entry_pool_list;
		};

#pragma region field_type_definition
		typedef int32 (*mdp_field_type_maximum_size_calculator)(struct field_type_property_definition* field_properties_definition);
		typedef bool (*mdp_field_type_verify_parameters)(struct field_type_property_definition* field_properties_definition);
		typedef void (*mdp_field_type_unk_proc)(struct field_type_property_definition* field_properties_definition);

		struct field_type_definition
		{
			Enums::field_type type;
			bool requires_parameters;
			PAD24;
			mdp_field_type_maximum_size_calculator proc_maximum_size_calculator;
			mdp_field_type_verify_parameters proc_verify_parameters;
			mdp_field_type_unk_proc proc_unk;
			bool initialized;
			PAD24;
		};
#pragma endregion

#pragma region field_properties_definition
		// returns the amount of bytes written
		// If [source_data] == [baseline_data], [source_data] isn't encoded
		typedef int32 (*mdp_field_encode)(struct field_properties_definition* field_properties, const void* baseline_data, const void* source_data, Networking::bitstream* output_stream);
		// returns the amount of bytes read
		typedef int32 (*mdp_field_decode)(struct field_properties_definition* field_properties, void* destination_data, Networking::bitstream* input_stream);
		
		struct field_properties_definition
		{
			Enums::field_type type;
			char name[76];
			mdp_field_encode proc_encode;
			mdp_field_decode proc_decode;
			union field_type_definition_parameters* parameters;
			int32 maximum_size; ///< in bits
			int32 header_bit_size;
			bool initialized;
			PAD24;
		};
#pragma endregion

		struct field_reference
		{
			field_properties_definition* properties;
			uint32 offset; ///< in bytes
			uint32 baseline_offset; ///< in bytes
			bool initialized;
			PAD24;
		};

		struct field_reference_set
		{
			int32 field_count;
			int32 max_data_size; ///< in bits
#pragma warning( push )
#pragma warning( disable : 4200 ) // nonstandard extension used : zero-sized array in struct/union, Cannot generate copy-ctor or copy-assignment operator when UDT contains a zero-sized array
			field_reference fields[];
#pragma warning( pop )
		};

#pragma region field_type_definition_parameters
		union field_type_definition_parameters // Note: engine code doesn't actually use unions, so be sure you're accessing the right structure!
		{
			struct {
				Enums::integer_width_type width;
			}integer;
			struct {
				int32 maximum_length;
			}ascii_string, wide_string;
			struct {
				int32 max_size;
			}arbitrary_data;
			struct {
				Enums::field_type width;
				PAD32; // unknown
				field_properties_definition* member_properties;
			}array;
			struct {
				int32 field_count;
#pragma warning( push )
#pragma warning( disable : 4200 ) // nonstandard extension used : zero-sized array in struct/union, Cannot generate copy-ctor or copy-assignment operator when UDT contains a zero-sized array
				field_reference members_references[];
#pragma warning( pop )
			}structure;
			struct {
				field_properties_definition* referent_properties;
			}pointer;
			struct {
				Enums::enumeration_width_type width;
			}enumeration;
			struct {
				int32 minimum_value;
				int32 maximum_value;
			}bounded_index;
			union {
				int32 maximum_active_at_once;
				PAD32;				// unknown
				int32 bits_needed;
				s_index_resolution_table table;
				int32 cursor;
				datum_index* allocations;
				int32 peak;
				int32 codings;
				int32 none;
			}translated_index;
			struct {
				int32 component_count;
			}point, vector;
			struct {
				int32 count;
				bool valid_bits[Enums::k_mdp_maximum_flags]; // I guess
			}flags;
			struct {
				int32 number_of_bits;
				int32 range_of_values;
			}fixed_width;
			struct {
				int32 number_of_bits_theta_internet;
				int32 number_of_bits_phi_internet;
				int32 number_of_bits_theta_lan;
				int32 number_of_bits_phi_lan;
			}fixed_width_normal_vector;
			struct {
				real minimum_value;
				real maximum_value;
				int32 number_of_bits_theta_internet;
				int32 number_of_bits_phi_internet; // unused
				int32 number_of_bits_theta_lan;
				int32 number_of_bits_phi_lan; // unused
				int32 vectors_count;
				real_vector3d decode_vectors[32];
				real_vector3d encode_vectors[32];
			}smart_vector;
		};
#pragma endregion

#pragma region message_delta_definition
		struct message_delta_definition
		{
			long_enum definition_type;				// 0x0 [Enums::message_delta]
			int32 message_dependent_header_size;	// 0x4 body_size + body field count
			int32 iteration_size;					// 0x8
			int32 iteration_independent_header_size;// 0xC
			int32 total_size;						// 0x10
			int32 max_iterations;					// 0x14
			bool initialized;		PAD24;			// 0x18
			field_reference_set* header_field_set;	// 0x1C
			field_reference_set body_field_set;		// 0x20
		};
#pragma endregion

		const message_delta_definition* const* OriginalPackets();
#ifndef YELO_NO_NETWORK
		const message_delta_definition* const* NewPackets();
	#define GET_MDP_DEFINITION(name) MessageDeltas::NewPackets()[ Yelo::Enums::_message_delta_##name ]
#endif


		struct decoding_information_data
		{
			Enums::message_delta_mode mode;		// 0x0
			long_enum definition_type;			// 0x4 [Enums::message_delta]
			int32 iteration_count;				// 0x8
			int32 state;						// 0xC
			Networking::bitstream* input_stream;// 0x10
			int32 unknown1;						// 0x14
			int32 current_iteration;			// 0x18
			bool iteration_header_decoded;		// 0x1C
			bool iteration_body_decoded;		// 0x1D
			PAD16;
			UNKNOWN_TYPE(int32);	// 0x20, iteration_independent_overhead_type = 3
			UNKNOWN_TYPE(int32);	// 0x24, iteration_independent_overhead_type = 2
			UNKNOWN_TYPE(int32);	// 0x28, iteration_independent_overhead_type = 1
			UNKNOWN_TYPE(int32);	// 0x2C, iteration_independent_overhead_type = 0

			// This structure may have 8 more bytes of data, but I haven't seen them used in code yet...
		};

		struct message_dependant_header
		{
			decoding_information_data* decoding_information;
			BOOL body_field_exists[16];
			void* custom_header_destination;
		}; BOOST_STATIC_ASSERT( sizeof(message_dependant_header) == 0x48 );


#pragma region packet field types
		typedef unsigned char	integer_small;
		typedef short			integer_medium;
		typedef long			integer_large;
		typedef float			real;
		typedef bool			boolean;
		typedef unsigned char	ascii_character;
		typedef wchar_t			wide_character;
		typedef unsigned char	ascii_string;
		typedef wchar_t			wide_string;
		typedef unsigned char	arbitary_data;
		

		typedef void*			pointer;
		typedef short			enumeration;
		
		typedef long			translated_index;

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
			float i;
			float j;
		};
		struct					vector3d
		{
			float i;
			float j;
			float k;
		};
		typedef float			angle;
		typedef unsigned long	time;
		struct					grenade_counts
		{
			short plasma;
			short frags;
		};

		typedef float			fixed_width_1bit;
		typedef float			fixed_width_3bits;
		typedef float			fixed_width_6bits;

		typedef struct			t_fixed_width_normal_4bit
		{
			float i;
			float j;
			float k;
		}fixed_width_normal_4bit, fixed_width_normal_8bit;
		typedef struct			t_fixed_width_normal_16bit
		{
			float i;
			float j;
			float k;
			float w;
		}fixed_width_normal_16bit;
		struct					locality_reference_position
		{
			float x;
			float y;
			float z;
		};
		struct					digital_throttle
		{
			float unknown1;
			float unknown2;
		};
		typedef short			fixed_width_weapon_index;
		typedef short			fixed_width_grenade_index;
		typedef struct			t_smart_vector
		{
			float i;
			float j;
			float k;
		}smart_vector, angular_velocity, translational_velocity;
		struct					item_placement_position
		{
			float x;
			float y;
			float z;
		};
#pragma endregion


#pragma region packet non generic types
		typedef unsigned char	local_player_update_sequence_number;
		typedef unsigned char	remote_player_update_sequence_number;
		typedef unsigned char	remote_player_action_update_baseline_id;
		typedef long			update_tick_count;
		typedef unsigned char	update_id;
		typedef datum_index		object_index;
		typedef datum_index		player_index;
		typedef datum_index		definition_index;
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

		typedef ascii_character map_name[128];
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
		typedef wide_character	hud_chat_message[255];
		typedef pointer			hud_chat_message_ptr;
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
		typedef ascii_character motd_text[255];
		typedef ascii_character rcon_response[80];
		typedef ascii_character rcon_password[8];
		typedef ascii_character rcon_command[64];
#pragma endregion

		struct message_delta_guaranteed_object_header
		{
			object_index	_object_index;
		};

		struct message_delta_game_object_header
		{
			object_index	_object_index;
			integer_small	baseline_index;
			integer_small	message_index;
			boolean			update_baseline;
			PAD8;
		};

		struct message_delta_game_object_header_timestamped
		{
			object_index	_object_index;
			integer_small	baseline_index;
			integer_small	message_index;
			boolean			update_baseline;
			PAD8;
			time			timestamp;
		};

		struct biped_update_header
		{
			object_index	_object_index;
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
			update_id		id;
		};

		struct player_score_update_header
		{
			player_index	player;
		};

		struct remote_player_action_update_header
		{
			player_index	player;
			update_id		update_id;
			remote_player_action_update_baseline_id baseline_id;
		};

		struct remote_player_position_update_header
		{
			player_index	player_index;
			update_id		update_id;
			remote_player_action_update_baseline_id	baseline_id;
		};

		struct remote_player_total_update_header
		{
			remote_player_action_update_header		action_header;
			remote_player_update_sequence_number	sequence_number;
		};




		struct mdp_hud_chat_network_data
		{
			integer_small msg_type; // [Enums::hud_chat_type], see GameUI.hpp
			PAD24;
			integer_small player_number;
			PAD24;
			hud_chat_message_ptr message;
		};
	};
};