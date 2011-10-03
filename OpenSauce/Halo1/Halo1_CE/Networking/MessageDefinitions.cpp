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
		//////////////////////////////////////////////////////////////////////////
		// Template'd handler for automatically handling DiscardIterationBody when a message fails to be decoded from the network
		typedef bool (* proc_message_delta_from_network_impl)(Networking::s_network_game_client* client, message_dependant_header* header);
		
		template<const proc_message_delta_from_network_impl k_impl>
		static void MessageDeltaFromNetworkResultHandler(Networking::s_network_game_client* client, message_dependant_header* header)
		{
			if(!k_impl(client, header))
				MessageDeltas::DiscardIterationBody(header);
		}
		//////////////////////////////////////////////////////////////////////////

		/* --- CUSTOM FIELDS --- */


		/* --- CUSTOM PACKETS --- */
		static bool NullFromNetwork(Networking::s_network_game_client* client, message_dependant_header* header)
		{
			YELO_DEBUG("Received a packet we haven't made a function to decode yet!", true);
			
			return false;
		}

		struct MDP_STRUCT_NAME(test)
		{
			MDP_STRUCT_DEFINE_TRAITS(1, 1, _message_delta_test);

			real value;

			static bool FromNetwork(Networking::s_network_game_client* client, message_dependant_header* header)
			{
				bool result = false;

				MDP_STRUCT_NAME(test) test;
				decoding_information_data* decode_info = header->decoding_information;

				if( result = MessageDeltas::DecodeStatelessIterated(header, &test) )
					GameState::Physics()->gravity = test.value;

				return result;
			}
		};

		MDP_DEFINITION_START(test, MDP_GET_FIELD_SET_DEFINITION(empty))
			MDP_DEFINITION_FIELD(test, value, real),
		MDP_DEFINITION_END();

		void* TestToNetwork()
		{
			if( !Networking::IsServer() ) return NULL;

			MDP_STRUCT_NAME(test) test;
			test.value = GameState::Physics()->gravity;

			void* data = &test;

			int32 bits_encoded = MessageDeltas::EncodeStateless(
				Enums::_message_delta_mode_stateless,
				Enums::_message_delta_test,
				NULL, &data);
			
			if( bits_encoded <= 0 || !MessageDeltas::SvSendMessageToAll(bits_encoded) )
			{
				YELO_DEBUG("TestToNetwork failed", true);
				YELO_DEBUG_FORMAT("bits encoded: %d", bits_encoded);
			}

			return NULL;
		}


		struct MDP_STRUCT_NAME(update_script_global)
		{
			MDP_STRUCT_DEFINE_TRAITS(1, 4, _message_delta_update_script_global);

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

		MDP_DEFINITION_START(update_script_global, MDP_GET_FIELD_SET_DEFINITION(empty))
			MDP_DEFINITION_FIELD(update_script_global, is_external, boolean),
			MDP_DEFINITION_FIELD(update_script_global, type, integer_medium),
			MDP_DEFINITION_FIELD(update_script_global, index, integer_medium),
			MDP_DEFINITION_FIELD(update_script_global, value, integer_large),
		MDP_DEFINITION_END();



		struct MDP_STRUCT_NAME(player_biped_update)
		{
			MDP_STRUCT_DEFINE_TRAITS(1, 2, _message_delta_player_biped_update);

			player_index player;
			definition_index biped;
		};

		MDP_DEFINITION_START(player_biped_update, MDP_GET_FIELD_SET_DEFINITION(empty))
			MDP_DEFINITION_FIELD(player_biped_update, player, player_index),
			MDP_DEFINITION_FIELD(player_biped_update, biped, definition_index),
		MDP_DEFINITION_END();

		/* --- CUSTOM PACKET DEF POINTERS --- */

		message_delta_definition* kYeloMessageDeltas[] = {
			&GET_NEW_MDP_DEFINITION(test),
			&GET_NEW_MDP_DEFINITION(update_script_global),
			&GET_NEW_MDP_DEFINITION(player_biped_update),
		};

		const packet_decoder kYeloMessageDeltaDecoders[] = {

			{FLAG(Enums::_message_deltas_new_client_bit), 
				MessageDeltaFromNetworkResultHandler<MDP_STRUCT_NAME(test)::FromNetwork>},
			{FLAG(Enums::_message_deltas_new_client_bit), 
				MessageDeltaFromNetworkResultHandler<NullFromNetwork>},
			{FLAG(Enums::_message_deltas_new_client_bit), 
				MessageDeltaFromNetworkResultHandler<NullFromNetwork>},

		};
		BOOST_STATIC_ASSERT( 
			NUMBEROF(kYeloMessageDeltas) == Enums::k_message_deltas_yelo_count &&
			NUMBEROF(kYeloMessageDeltaDecoders) == Enums::k_message_deltas_yelo_count 
		);
#endif
	};
};