/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/main/main.hpp>
#include <blamlib/Halo1/networking/network_messages.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum transport_rejection_code : long_enum;

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
		struct s_network_game_player;
		struct s_network_client_machine;

#ifdef API_DEBUG
		extern cstring message_packet_to_string_table[];
#endif


		void Initialize();
		void Dispose();

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


		Enums::network_game_generic_state GetNetworkGameState();
	};
};