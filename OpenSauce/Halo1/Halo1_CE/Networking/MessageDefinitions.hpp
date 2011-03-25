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
#include "Networking/MessageDeltas.hpp"

namespace Yelo
{
	namespace Enums
	{
		enum message_deltas_new
		{
#ifndef YELO_NO_NETWORK
			_message_delta_test = k_message_deltas_count,
			_message_delta_update_script_global,
			_message_delta_player_biped_update,

			k_message_deltas_new_count,
#else
			k_message_deltas_new_count = k_message_deltas_count,
#endif

			// How many packets Yelo as added
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
		typedef void  (API_FUNC* mdp_from_network_proc)(Networking::s_network_game_client* client, message_dependant_header* header);

		struct mdp_packet_decoder
		{
			long_flags Flags;			// [message_deltas_new_flags]
			mdp_from_network_proc Proc;
		};

#ifndef YELO_NO_NETWORK
		extern message_delta_definition* new_mdp_packets[];
		extern const mdp_packet_decoder new_mdp_packet_decoders[];
#endif

#ifndef YELO_NO_NETWORK
		void* TestToNetwork();
#endif
	};
};