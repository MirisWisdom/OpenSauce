/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include "Networking/MessageDeltas.hpp"

namespace Yelo
{
	namespace Networking
	{
		struct s_network_game_client;
	};

	namespace Enums
	{
		enum message_deltas_new
		{
#ifndef YELO_NO_NETWORK
	#if FALSE
			_message_delta_yelo_version = k_message_deltas_count,	// TODO hook replicate_current_state_to_new_player
			_message_delta_yelo_game_state_update,					// TODO hook replicate_current_state_to_new_player

			_message_delta_script_global_update,

			//////////////////////////////////////////////////////////////////////////
			// everything below this comment is experimental (and so none of these should ever be used in a non-internal build)

			_message_delta_switch_bsp,								// TODO hook scenario_switch_structure_bsp
			
			_message_delta_device_group_update,
			_message_delta_device_new,
			_message_delta_device_update,

		#ifdef API_DEBUG
			// This should only ever be defined in debug builds and used for testing purposes only
			// NO new delta types should come after this. This should always be last.
			_message_delta_test,
		#endif
	#endif

	#if FALSE
			k_message_deltas_new_count,
	#else
			k_message_deltas_new_count = k_message_deltas_count,
	#endif
#else
			k_message_deltas_new_count = k_message_deltas_count,
#endif

			// How many packets Yelo has added
			k_message_deltas_yelo_count = k_message_deltas_new_count - k_message_deltas_count,
		};

		enum message_deltas_new_flags
		{
			// Packet is decodable by the client
			_message_deltas_new_client_bit,
			// Packet is decodable by the server
			_message_deltas_new_server_bit,

			// Packet can be decoded by both client and server connections
			_message_deltas_new_omni_connection_flag = 
				FLAG(_message_deltas_new_client_bit) |
				FLAG(_message_deltas_new_server_bit),
		};
	};

	namespace MessageDeltas
	{
		typedef void (API_FUNC* proc_message_delta_from_network)(Networking::s_network_game_client* client, message_dependant_header* header);

		struct packet_decoder
		{
			long_flags Flags;			// [message_deltas_new_flags]
			proc_message_delta_from_network Proc;
		};

#ifndef YELO_NO_NETWORK
		// Yelo's new message delta definitions
		extern message_delta_definition* kYeloMessageDeltas[];
		// Yelo's new message delta decoders
		extern const packet_decoder kYeloMessageDeltaDecoders[];
#endif

#if !defined(YELO_NO_NETWORK) && FALSE
		void* TestToNetwork();
#endif
	};
};