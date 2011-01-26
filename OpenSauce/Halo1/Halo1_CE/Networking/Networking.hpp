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

#include "Memory/Data.hpp"
#include "Memory/MemoryInterface.hpp"
#include "Game/GameEngine.hpp"

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_ipv4_address_length = 4,
			k_ipv6_address_length = 16,
		};

		enum bitstream_mode : long_enum
		{
			_bitstream_mode_write,
			_bitstream_mode_read,
		};

		enum game_connection : _enum
		{
			_game_connection_local,
			_game_connection_network_client,
			_game_connection_network_server,

			_game_connection = 4,
		};

		enum transport_type : byte_enum
		{
			_transport_type_udp = 17,
			_transport_type_tcp,

			_transport_type_gamespy = 20, // I just pulled this name out of my ass
		};

		enum transport_rejection_code : long_enum
		{
			_transport_rejection_code_none,
			_transport_rejection_code_unknown,
			_transport_rejection_code_too_many_connection_attempts,
			_transport_rejection_code_invalid_protocol,

			_transport_rejection_code_version_too_old,
			_transport_rejection_code_version_too_new,
			_transport_rejection_code_game_is_full,
			_transport_rejection_code_game_is_closed,
			_transport_rejection_code_blacklisted_machine,
			_transport_rejection_code,
		};

		enum network_game_server_state : _enum
		{
			_network_game_server_state_pregame,
			_network_game_server_state_ingame,
			_network_game_server_state_postgame,

			_network_game_server_state,
		};

		enum network_game_client_state : _enum
		{
			_network_game_client_state_searching,
			_network_game_client_state_joining,
			_network_game_client_state_pregame,
			_network_game_client_state_ingame,
			_network_game_client_state_postgame,

			_network_game_client_state,
		};

		enum network_game_generic_state : _enum // made this up for Yelo's code
		{
			_network_game_generic_state_pregame,
			_network_game_generic_state_ingame,
			_network_game_generic_state_postgame,

			_network_game_generic_state_unknown,
			_network_game_generic_state,
		};

		// Old halo packets
		enum message_type
		{
			_message_client_broadcast_game_search,
			_message_client_ping,
			_message_server_game_advertise,
			_message_server_pong,
			_message_server_new_client_challenge, // new
			_message_server_machine_accepted,
			_message_server_machine_rejected,
			_message_server_game_is_ending_holdup, // new
			_message_server_game_settings_update,
			_message_server_pregame_countdown,
			_message_server_begin_game,
			_message_server_graceful_game_exit_pregame,
			_message_server_pregame_keep_alive,
			_message_server_postgame_keep_alive,
			_message_client_join_game_request,
			_message_client_add_player_request_pregame,
			_message_client_remove_player_request_pregame,
			_message_client_settings_request,
			_message_client_player_settings_request,
			_message_client_game_start_request,
			_message_client_graceful_game_exit_pregame,
			_message_client_map_is_precached_pregame,
			_message_server_game_update,
			_message_server_add_player_ingame,
			_message_server_remove_player_ingame,
			_message_server_game_over,
			_message_client_loaded,
			_message_client_game_update,
			_message_client_add_player_request_ingame,
			_message_client_remove_player_request_ingame,
			_message_client_graceful_game_exit_ingame, // new
			_message_client_host_crashed_cry_for_help,
			_message_client_join_new_host,
			_message_server_reconnect, // new
			_message_server_graceful_game_exit,
			_message_client_remove_player_request_postgame,
			_message_client_switch_to_pregame,
			_message_client_graceful_game_exit_postgame,
			_message_type,
		};

		enum gamespy_field
		{
			_gamespy_field_hostname,
			_gamespy_field_gamename,
			_gamespy_field_gamever,
			_gamespy_field_hostport,
			_gamespy_field_mapname,
			_gamespy_field_gametype,
			_gamespy_field_gamevariant,
			_gamespy_field_numplayers,
			_gamespy_field_numteams,
			_gamespy_field_maxplayers,
			_gamespy_field_gamemode,
			_gamespy_field_teamplay,
			_gamespy_field_fraglimit,
			_gamespy_field_team_fraglimit,
			_gamespy_field_timeelapsed,
			_gamespy_field_timelimit,
			_gamespy_field_roundtime,
			_gamespy_field_roundelapsed,
			_gamespy_field_password,
			_gamespy_field_groupid,
			_gamespy_field_player,
			_gamespy_field_score,
			_gamespy_field_skill,
			_gamespy_field_ping,
			_gamespy_field_team,
			_gamespy_field_deaths,
			_gamespy_field_pid,
			_gamespy_field_team_t,
			_gamespy_field_score_t,
#pragma region Unused fields
			_gamespy_field_unused1,
			_gamespy_field_unused2,
			_gamespy_field_unused3,
			_gamespy_field_unused4,
			_gamespy_field_unused5,
			_gamespy_field_unused6,
			_gamespy_field_unused7,
			_gamespy_field_unused8,
			_gamespy_field_unused9,
			_gamespy_field_unused10,
			_gamespy_field_unused11,
			_gamespy_field_unused12,
			_gamespy_field_unused13,
			_gamespy_field_unused14,
			_gamespy_field_unused15,
			_gamespy_field_unused16,
			_gamespy_field_unused17,
			_gamespy_field_unused18,
			_gamespy_field_unused19,
			_gamespy_field_unused20,
			_gamespy_field_unused21,
#pragma endregion
			_gamespy_field_dedicated,
			_gamespy_field_player_flags,
			_gamespy_field_game_flags,
			_gamespy_field_game_classic,
			_gamespy_field,
		};

		enum gamespy_connection_state : long_enum
		{
			GTI2AwaitingServerChallenge,
			GTI2AwaitingAcceptance,
			GTI2AwaitingClientChallenge,
			GTI2AwaitingClientResponse,
			GTI2AwaitingAcceptReject,
			GTI2Connected,
			GTI2Closing,
			GTI2Closed,
		};
	};

	namespace Networking
	{
		struct s_gamespy_connection
		{
			in_addr address;
			int16 port; PAD16;

			UNKNOWN_TYPE(int32); // pointer
			Enums::gamespy_connection_state state;

			byte pad[0x150 - 0x10]; // TODO
		}; BOOST_STATIC_ASSERT( sizeof(s_gamespy_connection) == 0x150 );
		struct s_gamespy_socket // NOTE: this is actually part of the gamespy API, just using OS naming standards
		{
			SOCKET socket;
			in_addr address;
			int16 port; PAD16;

			void* connections;
			void* closedConnections;
			UNKNOWN_TYPE(int32); // 0x14
			UNKNOWN_TYPE(int32);
			void* connectAttemptCallback;
			void* socketErrorCallback;
			UNKNOWN_TYPE(int32); // 0x24 pointer to a proc
			UNKNOWN_TYPE(int32); // 0x28 pointer to a proc
			void* unrecongizedMessageCallback;
			UNKNOWN_TYPE(int32); // 0x30, s_transport_endpoint*
			int32 incomingBufferSize;
			int32 outgoingBufferSize;
			UNKNOWN_TYPE(int32); // 0x3C
			UNKNOWN_TYPE(int32); // 0x40, s_transport_endpoint*
			UNKNOWN_TYPE(int32); // 0x44
		}; BOOST_STATIC_ASSERT( sizeof(s_gamespy_socket) == 0x48 );

		struct s_transport_address
		{
			union
			{
				struct _ipv4 {
					byte class_a;
					byte class_b;
					byte class_c;
					byte class_d;
				}ipv4;

				struct _ipv6 {
					int16 class_a;
					int16 class_b;
					int16 class_c;
					int16 class_d;

					int16 class_e;
					int16 class_f;
					int16 class_g;
					int16 class_h;
				}ipv6;

				byte ip[Enums::k_ipv6_address_length];
			};
			int16 address_length;
			int16 port;

			int32 unknown;
		}; BOOST_STATIC_ASSERT( sizeof(s_transport_address) == 0x18 );

		struct s_transport_endpoint
		{
			s_gamespy_connection* gs_connection;
			bool is_listening_endpoint;	// 0x4
			UNKNOWN_TYPE(byte);			// 0x5, bool
			PAD16;

			SOCKET socket;				// 0x8, unused in PC, use gs_connection
			byte_flags flags;			// 0xC
			Enums::transport_type type;	// 0xD
			int16 error_code;			// 0xE
			Memory::s_circular_queue* received_data_queue;
			int32 gamespy_log_column;
			Enums::transport_rejection_code rejection_code;

		}; BOOST_STATIC_ASSERT( sizeof(s_transport_endpoint) == 0x1C );

		struct s_transport_endpoint_set
		{
			fd_set select_set;

			s_transport_endpoint** ep_array;
			int32 max_endpoints;
			int32 count;
			int32 array_cursor;
		}; BOOST_STATIC_ASSERT( sizeof(s_transport_endpoint_set) == 0x114 );

		struct s_bitstream
		{
			Enums::bitstream_mode mode;
			void* buffer;		// 0x4
			void* first_bit;	// 0x8 first bit where we can write to
			void* byte_number;	// 0xC byte index in the buffer
			void* bit_in_byte;	// 0x10 bitNum in torque
			void* last_bit;		// 0x14 last bit where we can write to
			int32 size;			// 0x18 size of buffer
			PAD32;				// 0x1C bool error?
		};


		struct s_network_connection : TStructImpl(0xAE4)
		{
			TStructGetPtrImpl(s_transport_endpoint*, ReliableEndpoint, 0x0);
			// 0x4, bool?
			TStructGetPtrImpl(Memory::s_circular_queue*, ReliableIncomingQueue, 0xC);
			// 0x10, message stream?

			// 0xA78, prioritization_buffer.numMessages
			// 0xA7C, prioritization_buffer.messages, array of sizeof(32) structure
				// 0x8, headerMaxSizeInBytes
				// 0xC, dataMaxSizeInBytes
				// 0x14, s_bitstream*

			// 0xA80, int32
			// 0xA88, connection_class
			// 0xA8C, byte_flags flags
				//_connection_create_server_bit = BIT(0)
			// 0xA98, bool
			TStructGetPtrImpl(s_transport_endpoint_set, EndpointSet, 0xA9C );
			TStructGetPtrImpl(s_network_connection*, ClientList, 0xAA0 ); // [16]
			// 0xAE0, pad here or value?
			TStructGetPtrImpl(bool, HasLocalConnection, 0xAE1 );
		};

		struct s_network_machine : TStructImpl( PLATFORM_VALUE(0x60, 0xEC) )
		{
			TStructGetPtrImpl(s_network_connection*, Connection, 0x0);
			TStructGetPtrImpl(int32, LastReceivedUpdateSequenceNumber, 0x4);
			TStructGetPtrImpl(int32, StallStartTime, 0x8);
			TStructGetPtrImpl(int16, MachineIndex, 0xC);
			// BIT(0) - 
			// *BIT(1) - is_joined_to_game
			// *BIT(2) - game_loading_complete
			// *BIT(3) - is_precached
			TStructGetPtrImpl(word_flags, Flags, 0xE);
			TStructGetPtrImpl(bool, HasPlayers, 0x50);

			TStructGetPtrImpl(char, GsChallengeStr, 0x52); // char[7+1]
			TStructGetPtrImpl(int32, GsMachineKey, 0x5C); // same value as gs_machine_data->unknown1

#if PLATFORM_IS_DEDI
			TStructGetPtrImpl(wchar_t, PlayerName, 0x60); // [12]
			TStructGetPtrImpl(int32, PlayerNumber, 0x78);
			TStructGetPtrImpl(int32, TeamIndex, 0x7C);
			TStructGetPtrImpl(char, IpAddress, 0x80);
			TStructGetPtrImpl(char, CdKey, 0xA0);
#endif
		};

		struct s_network_map
		{
			int32 version;
			char name[128];
		}; BOOST_STATIC_ASSERT( sizeof(s_network_map) == 0x84 );

		struct s_network_player
		{
			wchar_t name[12];			// 0x0
			int16 primary_color_index;	// 0x18
			int16 icon_index;			// 0x1A
			byte machine_index;			// 0x1C
			byte controller_index;		// 0x1D
			// These values will be the same on the client as they are on the server, so 
			// we can use them safely for player operations in multiplayer code
			byte team_index;			// 0x1E
			byte player_list_index;		// 0x1F
		}; BOOST_STATIC_ASSERT( sizeof(s_network_player) == 0x20 );

		struct s_network_game
		{
			wchar_t name[64];						// 0x0
			s_network_map map;						// 0x80
			GameEngine::s_game_variant game_variant;// 0x104
			byte _unused_game_engine;				// 0x1DC
			byte maximum_players;					// 0x1DD
			int16 difficulty_level;					// 0x1DE
			int16 player_count;						// 0x1E0
			s_network_player players[16];			// 0x1E2
			PAD16;									// 0x3E2
			int32 network_game_random_seed;			// 0x3E4
			int32 number_of_games_played;			// 0x3E8
			int32 local_data;						// 0x3EC
		}; BOOST_STATIC_ASSERT( sizeof(s_network_game) == 0x3F0 );


		struct s_network_game_server : TStructImpl( PLATFORM_VALUE(0xA50, 0xA50+0x8C0) )
		{
			TStructGetPtrImpl(s_network_connection*, Connection, 0x0);
			TStructGetImpl(Enums::network_game_server_state, State, 0x4);
			// game_is_open = BIT(0)
			// server_is_dedicated = BIT(2) // doesn't show sv_status info when not set
			TStructGetPtrImpl(uint16, Flags, 0x6);
			TStructGetPtrImpl(s_network_game, Game, 0x8);
			TStructGetPtrImpl(s_network_machine, ClientMachines, 0x3F8); // [16]

			// sizeof(s_network_machine) is different under dedicated builds 
			// which causes us to have to offset all proceeding members by 0x8C0
			TStructGetPtrImpl(int32, NextUpdateNumber, PLATFORM_VALUE(0x9F8, 0x9F8+0x8C0) );
			TStructGetPtrImpl(int32, TimeOfLastKeepAlive, PLATFORM_VALUE(0x9FC, 0x9FC+0x8C0) );
			TStructGetPtrImpl(int32, TimeOfFirstClientLoadingCompletion, PLATFORM_VALUE(0xA04, 0xA04+0x8C0) );
			// 0xA4E bool
			// 0xA4F bool
			TStructGetPtrImpl(wchar_t, Password, PLATFORM_VALUE(0xA3C, 0xA3C+0x8C0) ); // [8]

			// bool pause_countdown; // 0xA15
		};

		struct s_network_game_client : TStructImpl(0xF90)
		{
			TStructGetPtrImpl(int16, MachineIndex, 0x0);

			TStructGetPtrImpl(s_transport_address, SvIpAddress, 0xAB4);
			// 0x10 of unknown bytes between here...
			TStructGetPtrImpl(s_network_connection*, Connection, 0xADC);
			// 0x34 of unknown bytes between here...
			TStructGetPtrImpl(s_network_game, Game, 0xB14);
			// 0xF04 int32
			// 0xF08 int32
			TStructGetPtrImpl(uint32, NextExpectedUpdate, 0xF0C);
			TStructGetPtrImpl(uint32, TimeOfLastUpdate, 0xF10);
			// 0xF18 int16 (set to NONE, so its signed)
			TStructGetImpl(Enums::network_game_client_state, State, 0xF1A);
			// 0xF1C int16
			// 0xF1E word_flags
			// 0xF20 bool
			// 0xF21 bool
			// PAD16?
			// 0xF24 0x30 byte data structure
			// 0xF54 0x34 byte data structure

			struct s_player_update_history
			{
				uint32 next_update_history_id; // maximum = 64
				PAD32; // void* to 0x418 structure (0x4 in that structure references the 'next', if once exists, 0x414 is another GlobalAlloc'd pointer)
				PAD32;
				uint32 number_of_playbacks;
				uint32 total_ticks;
				uint32 total_updates;
				PAD32; // 0x18
				PAD32; // 0x1C
				PAD32; // 0x20
				real avg_prediction_error;
				real avg_ticks_played_back;
			};
			TStructGetPtrImpl(s_player_update_history*, UpdateHistory, 0xF88);
		};


		struct s_player_action
		{
			uint32 update_id;
			real_euler_angles2d desired_facing;
			real_vector2d throttle;
			real primary_trigger;
			int16 weapon_index, grenade_index;
			int16 zoom_index;
			PAD16;
		}; BOOST_STATIC_ASSERT( sizeof(s_player_action) == 0x20 );

		struct s_action_update : TStructImpl(40)
		{
		};
		typedef Memory::DataArray<s_action_update, 16> t_update_client_queues_data;

		struct s_update_client_globals
		{
			bool initialized;
			PAD24;
			uint32 current_update_id;
			PAD32;
			struct {
				s_player_action actions[1];
			}saved_action_collection;
			uint32 ticks_to_apply_action_to;
			PAD32;
			int32 current_local_player;

			t_update_client_queues_data* queue_data;
			byte queue_data_buffer[0x308][128];
		}; BOOST_STATIC_ASSERT( sizeof(s_update_client_globals) == 0x1843C );
		s_update_client_globals* UpdateClientGlobals();

		struct update_server_queues_datum : TStructImpl(100)
		{
			//s_action_update current_action

			TStructGetPtrImpl(Memory::s_simple_circular_queue, ActionQueue, 0x28);
		};
		typedef Memory::DataArray<update_server_queues_datum, 16> t_update_server_queues_data;

		struct s_update_server_globals
		{
			bool initialized;
			PAD24;
			uint32 current_update_id;
			t_update_server_queues_data* queue_data;
			byte queue_data_buffer[0x308][32];
		}; BOOST_STATIC_ASSERT( sizeof(s_update_server_globals) == 0x610C );
		s_update_server_globals* UpdateServerGlobals();


		struct network_update_globals
		{
			struct _local_player {
				// 15
				int32 update_rate;
				// 15
				int32 vehicle_update_rate;
			}local_player;

			struct _remote_player {
				// 180
				int32 action_baseline_update_rate;
				// 30
				int32 position_update_rate;
				// 90
				int32 position_baseline_update_rate;
				// 15
				int32 vehicle_update_rate;
				// 90, not used
				int32 vehicle_baseline_update_rate;
			}remote_player;

			bool use_super_remote_players_action_update; // true
			bool use_new_vehicle_update_scheme; //true
		};
		// Network settings dealing with the rate of updating certain data
		network_update_globals* UpdateSettings();


#ifdef API_DEBUG
		extern cstring message_packet_to_string_table[];
#endif


		void Initialize();
		void Dispose();

		// Gets the current connection, returning a [game_connection]
		Enums::game_connection GameConnection();
		// Is this machine a server?
		bool IsServer();
		// Is this machine a client?
		bool IsClient();


		// Gets the network game server pointer
		s_network_game_server* NetworkGameServer();

		// Gets the network game client pointer
		s_network_game_client* NetworkGameClient();


		API_INLINE Enums::network_game_generic_state GetNetworkGameState()
		{
			if(IsClient())		return CAST(Enums::network_game_generic_state, 
				NetworkGameClient()->GetState()-Enums::_network_game_client_state_pregame);
			else if(IsServer()) return CAST(Enums::network_game_generic_state, 
				NetworkGameServer()->GetState()-Enums::_network_game_server_state_pregame);

			return Enums::_network_game_generic_state_unknown;
		}
	};
};