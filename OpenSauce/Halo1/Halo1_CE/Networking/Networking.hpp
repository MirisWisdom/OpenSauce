/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include "Memory/Data.hpp"
#include "Memory/MemoryInterface.hpp"
#include "Game/GameEngine.hpp"
#include "Networking/GameSpyApi.hpp"

#include <blamlib/Halo1/bungie_net/network/transport_address.hpp>
#include <blamlib/Halo1/bungie_net/network/transport_endpoint_winsock.hpp>
#include <blamlib/Halo1/bungie_net/network/transport_endpoint_set_winsock.hpp>
#include <blamlib/Halo1/game/players.hpp>
#include <blamlib/Halo1/main/main.hpp>
#include <blamlib/Halo1/memory/bitstream.hpp>
#include <blamlib/Halo1/networking/network_connection.hpp>
#include <blamlib/Halo1/networking/network_client_manager.hpp>
#include <blamlib/Halo1/networking/network_game_globals.hpp>
#include <blamlib/Halo1/networking/network_game_manager.hpp>
#include <blamlib/Halo1/networking/network_messages.hpp>
#include <blamlib/Halo1/networking/network_server_manager.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_message_highest_priority = 0,
			k_message_lowest_priority = 9,
		};

		enum network_game_generic_state : _enum // made this up for Yelo's code
		{
			_network_game_generic_state_pregame,
			_network_game_generic_state_ingame,
			_network_game_generic_state_postgame,

			_network_game_generic_state_unknown,
			_network_game_generic_state,
		};
	};

	namespace Networking
	{
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
				Players::s_player_action actions[Enums::k_maximum_number_of_local_players];
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


		// Network settings dealing with the rate of updating certain data
		network_update_globals* UpdateSettings();


#ifdef API_DEBUG
		extern cstring message_packet_to_string_table[];
#endif


		void Initialize();
		void Dispose();

		// Gets the current connection, returning a [game_connection]
		Enums::game_connection GameConnection();
		// Is the game *only* running the simulation locally? (ie, campaign or splitscreen)
		bool IsLocal();
		// Is this machine a server?
		bool IsServer();
		// Is this machine a client?
		bool IsClient();


		// Gets the network game server pointer
		s_network_game_server* NetworkGameServer();

		// Gets the network game client pointer
		s_network_game_client* NetworkGameClient();


		// Writes [data_size_in_bits] of the packet buffer to the connection
		// returns true if it was successful
		bool ConnectionWrite(const s_network_connection& connection, 
			const void* data, size_t data_size_in_bits,
			const void* header, size_t header_size_in_bits,
			bool unbuffered = false, bool flush_queue = false, int32 buffer_priority = Enums::k_message_highest_priority);

		bool ClientSendMessageToServer(
			const void* data, size_t data_size_in_bits, 
			Enums::network_messsage_type message_type = Enums::_network_messsage_type_message_delta, 
			bool unbuffered = false, bool flush_queue = false, int32 buffer_priority = Enums::k_message_highest_priority);

		bool ServerSendRejectionMessage(s_network_game_player& rejected_player, Enums::transport_rejection_code code);
		bool ServerHoldupNewClient(s_network_client_machine& client_machine);

		// Sends [data] of the packet buffer to [machine_index]
		bool SvSendMessageToMachine(int32 machine_index, 
			const void* data, size_t data_size_in_bits, 
			Enums::network_messsage_type message_type = Enums::_network_messsage_type_message_delta, 
			BOOL unbuffered = false, BOOL flush_queue = false, BOOL write_to_local_connection = false, 
			int32 buffer_priority = Enums::k_message_highest_priority);

		// Sends [data] of the packet buffer to all machines
		bool SvSendMessageToAll(
			const void* data, size_t data_size_in_bits, 
			Enums::network_messsage_type message_type = Enums::_network_messsage_type_message_delta, 
			BOOL unbuffered = false, BOOL flush_queue = false, BOOL write_to_local_connection = false, 
			int32 buffer_priority = Enums::k_message_highest_priority,
			BOOL ingame_only = false);

		// Sends [data] of the packet buffer to all machines ingame
		inline bool SvSendMessageToAllIngame(
			const void* data, size_t data_size_in_bits, 
			Enums::network_messsage_type message_type = Enums::_network_messsage_type_message_delta, 
			BOOL unbuffered = false, BOOL flush_queue = false, BOOL write_to_local_connection = false, 
			int32 buffer_priority = Enums::k_message_highest_priority)
		{
			return SvSendMessageToAll(
				data, data_size_in_bits, 
				message_type, 
				unbuffered, flush_queue, write_to_local_connection, 
				buffer_priority, 
				true);
		}


		API_INLINE Enums::network_game_generic_state GetNetworkGameState()
		{
			if(IsClient())		return CAST(Enums::network_game_generic_state, 
				NetworkGameClient()->state-Enums::_network_game_client_state_pregame);
			else if(IsServer()) return CAST(Enums::network_game_generic_state, 
				NetworkGameServer()->state-Enums::_network_game_server_state_pregame);

			return Enums::_network_game_generic_state_unknown;
		}
	};
};