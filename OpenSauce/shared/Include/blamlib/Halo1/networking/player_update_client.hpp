/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/action_queue.hpp>
#include <blamlib/Halo1/game/position_queue.hpp>
#include <blamlib/Halo1/game/vehicle_update_queue.hpp>
#include <blamlib/Halo1/networking/player_update.hpp>

namespace Yelo
{
	namespace Networking
	{
		struct s_player_client_update
		{
			union {
				struct {
					int32 last_acked_sequence_number;		// 0xE8
					int32 last_acked_update_id;				// 0xEC
					real_point3d update_data;				// 0xF0 position
				}local_player;
				struct {
					int32 last_acked_update_id;				// 0xE8
					int32 last_acked_baseline_id;			// 0xEC
					s_remote_player_action_update_network_data update_data;	// 0xF0
				}remote_player;
			};
			Game::s_action_queue action_queue;				// 0x120

			struct {
				int32 last_acked_sequence_number;			// 0x15C
				int32 last_acked_update_id;					// 0x160
				s_remote_player_position_update_network_data update_data;	// 0x164
			}remote_player_position;
			Game::s_position_queue position_queue;			// 0x170
			
			struct {
				int32 last_acked_sequence_number;			// 0x188
				int32 last_acked_update_id;					// 0x18C
				s_remote_player_vehicle_update_network_data update_data;	// 0x190
			}remote_player_vehicle;
			Game::s_vehicle_update_queue vehicle_update_queue;	// 0x1D0
			UNKNOWN_TYPE(int32);							// 0x1E8 related to remove player vehicle updating
			
			struct {
				int32 errors;
				real amount;
			}biped_prediction,								// 0x1EC
			 vehicle_prediction;							// 0x1F4
		}; BOOST_STATIC_ASSERT( sizeof(s_player_client_update) == 0x114 );

		struct s_action_update : TStructImpl(40)
		{
		};
		typedef Memory::DataArray<s_action_update, Enums::k_multiplayer_maximum_players>
			update_client_queues_data_t;

		struct s_update_client_globals
		{
			bool initialized;
			PAD24;
			uint32 current_update_id;
			PAD32;
			struct {
				Players::s_player_action actions[Enums::k_maximum_number_of_local_players];
			}saved_action_collection;
			uint32 ticks_to_apply_action_to;
			PAD32;
			int32 current_local_player;

			update_client_queues_data_t* queue_data;
			byte queue_data_buffer[0x308][128];
		}; BOOST_STATIC_ASSERT( sizeof(s_update_client_globals) == 0x1843C );
		s_update_client_globals* UpdateClientGlobals();
	};
};