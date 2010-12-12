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
#include "Common/Precompile.hpp"
#include "Networking/MessageDefinitions.hpp"

#include "Game/EngineFunctions.hpp"
#include "Networking/MDP.hpp"
#include "Game/GameState.hpp"

namespace Yelo
{
	namespace MessageDeltas
	{
#ifndef YELO_NO_NETWORK
		/* --- CUSTOM FIELDS --- */


		/* --- CUSTOM PACKETS --- */
		void null_from_network(Networking::s_network_game_client* client, message_dependant_header* header)
		{
			YELO_DEBUG("Received a packet we haven't made a function to decode yet!", true);
			MessageDeltas::DiscardIterationBody(header);
		}

		struct mdp_test
		{
			real value;
		};

		MDP_DEFINITION_START(test, test, 1, MDP_GET_FIELD_SET_DEFINITION(empty), 1)
			MDP_DEFINITION_FIELD(test, value, real),
		MDP_DEFINITION_END();

		void test_from_network(Networking::s_network_game_client* client, message_dependant_header* header)
		{
			mdp_test test;
			decoding_information_data* decode_info = header->decoding_information;

			YELO_DEBUG("test_from_network", true);
			if( MessageDeltas::DecodeStatelessIterated(header, &test) )
			{
				YELO_DEBUG("success f", true);
				*GameState::Gravity() = test.value;
			}
			else
			{
				MessageDeltas::DiscardIterationBody(header);
				YELO_DEBUG("failure f", true);
			}
		}

		void* TestToNetwork()
		{
			if( !Networking::IsServer() ) return NULL;
			YELO_DEBUG_FORMAT("is init'd? %d\ttotal size: %d", GET_MDP_DEFINITION(test)->initialized, GET_MDP_DEFINITION(test)->total_size);
			YELO_DEBUG("TestToNetwork", true);

			mdp_test test;
			test.value = *GameState::Gravity();

			void* data[2];
			data[0] = &test;
			data[1] = NULL; // eol

			int32 bits_encoded = MessageDeltas::EncodeStateless(
				Enums::_message_delta_mode_stateless,
				Enums::_message_delta_test,
				NULL,
				data);
			YELO_DEBUG_FORMAT("bits encoded: %d", bits_encoded);
			if( bits_encoded <= 0 && !MessageDeltas::SvSendMessageToAll(bits_encoded/*, false*/) )
			{	YELO_DEBUG("failed t", true); }
			else
			{	YELO_DEBUG("success t", true); }

			return NULL;
		}

		/*!
		* \brief
		* Update Script Global Packet
		*
		* used to update a internal or external global
		*/
		struct mdp_update_script_global
		{
			boolean is_external; // is an engine global
			integer_small type;
			integer_medium index;
			union {
				byte data[4];

				short word;
				long dword;
				real single;
				datum_index datum;
			}value;
		};

		MDP_DEFINITION_START(update_script_global, update_script_global, 1, MDP_GET_FIELD_SET_DEFINITION(empty), 3)
			MDP_DEFINITION_FIELD(update_script_global, is_external, boolean),
			MDP_DEFINITION_FIELD(update_script_global, type, integer_medium),
			MDP_DEFINITION_FIELD(update_script_global, index, integer_medium),
			MDP_DEFINITION_FIELD(update_script_global, value, integer_large),
		MDP_DEFINITION_END();


		/*!
		* \brief
		* 
		*
		* 
		*/
		struct mdp_player_biped_update
		{
			player_index player;
			definition_index biped;
		};

		MDP_DEFINITION_START(player_biped_update, player_biped_update, 1, MDP_GET_FIELD_SET_DEFINITION(empty), 2)
			MDP_DEFINITION_FIELD(player_biped_update, player, player_index),
			MDP_DEFINITION_FIELD(player_biped_update, biped, definition_index),
		MDP_DEFINITION_END();

		/* --- CUSTOM PACKET DEF POINTERS --- */

		message_delta_definition* new_mdp_packets[] = {
			&GET_NEW_MDP_DEFINITION(test),
			&GET_NEW_MDP_DEFINITION(update_script_global),
			&GET_NEW_MDP_DEFINITION(player_biped_update),
		};

		const mdp_packet_decoder new_mdp_packet_decoders[] = {

			{BIT32(Enums::_message_deltas_new_client_bit), 
				test_from_network},
			{BIT32(Enums::_message_deltas_new_client_bit), 
				null_from_network},
			{BIT32(Enums::_message_deltas_new_client_bit), 
				null_from_network},

		};
		
		BOOST_STATIC_ASSERT( 
				NUMBEROF(new_mdp_packets) == Enums::k_message_deltas_yelo_count &&
				NUMBEROF(new_mdp_packet_decoders) == Enums::k_message_deltas_yelo_count 
			);
#endif
	};
};