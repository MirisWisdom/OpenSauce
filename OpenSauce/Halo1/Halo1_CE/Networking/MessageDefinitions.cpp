/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
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
		typedef bool (API_FUNC* proc_message_delta_from_network_impl)(Networking::s_network_game_client* client, message_dependant_header* header);
		
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

#if FALSE
		//////////////////////////////////////////////////////////////////////////
		// _message_delta_script_global_update
		struct MDP_STRUCT_NAME(script_global_update)
		{
			MDP_STRUCT_DEFINE_TRAITS(1, 4, _message_delta_script_global_update);

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

		MDP_DEFINITION_START(script_global_update, MDP_GET_FIELD_SET_DEFINITION(empty))
			MDP_DEFINITION_FIELD(script_global_update, is_external, boolean),
			MDP_DEFINITION_FIELD(script_global_update, type, integer_medium),
			MDP_DEFINITION_FIELD(script_global_update, index, integer_medium),
			MDP_DEFINITION_FIELD(script_global_update, value, integer_large),
		MDP_DEFINITION_END();


	#ifdef API_DEBUG
		//////////////////////////////////////////////////////////////////////////
		// _message_delta_test
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
			if( !Networking::IsServer() ) return nullptr;

			MDP_STRUCT_NAME(test) test;
			test.value = GameState::Physics()->gravity;

			int32 bits_encoded = MessageDeltas::EncodeStateless(
				Enums::_message_delta_test,
				nullptr, &test);
			
			if( bits_encoded <= 0 || !MessageDeltas::SvSendMessageToAll(bits_encoded) )
			{
				YELO_DEBUG("TestToNetwork failed", true);
				YELO_DEBUG_FORMAT("bits encoded: %d", bits_encoded);
			}

			return nullptr;
		}
	#endif

#endif


		/* --- CUSTOM PACKET DEF POINTERS --- */
		// TODO: Game will crash when a delta definition is left null (ie, unimplemented).

		message_delta_definition* kYeloMessageDeltas[Enums::k_message_deltas_yelo_count+1] = {
#if FALSE
			nullptr, // _message_delta_yelo_version
			nullptr, // _message_delta_yelo_game_state_update

			&GET_NEW_MDP_DEFINITION(script_global_update),

			nullptr, // _message_delta_switch_bsp

			nullptr, // _message_delta_device_group_update
			nullptr, // _message_delta_device_new
			nullptr, // _message_delta_device_update

	#ifdef API_DEBUG
			&GET_NEW_MDP_DEFINITION(test),
	#endif
#endif
			nullptr // EOL
		};

		const packet_decoder kYeloMessageDeltaDecoders[Enums::k_message_deltas_yelo_count+1] = {
#if FALSE
			{FLAG(Enums::_message_deltas_new_client_bit), // _message_delta_yelo_version
				MessageDeltaFromNetworkResultHandler<NullFromNetwork>},
			{FLAG(Enums::_message_deltas_new_client_bit), // _message_delta_yelo_game_state_update
				MessageDeltaFromNetworkResultHandler<NullFromNetwork>},

			{FLAG(Enums::_message_deltas_new_client_bit), // _message_delta_script_global_update
				MessageDeltaFromNetworkResultHandler<NullFromNetwork>},

			{FLAG(Enums::_message_deltas_new_client_bit), // _message_delta_switch_bsp
				MessageDeltaFromNetworkResultHandler<NullFromNetwork>},

			{FLAG(Enums::_message_deltas_new_client_bit), // _message_delta_device_group_update
				MessageDeltaFromNetworkResultHandler<NullFromNetwork>},
			{FLAG(Enums::_message_deltas_new_client_bit), // _message_delta_device_new
				MessageDeltaFromNetworkResultHandler<NullFromNetwork>},
			{FLAG(Enums::_message_deltas_new_client_bit), // _message_delta_device_update
				MessageDeltaFromNetworkResultHandler<NullFromNetwork>},

	#ifdef API_DEBUG
			{FLAG(Enums::_message_deltas_new_client_bit), 
				MessageDeltaFromNetworkResultHandler<MDP_STRUCT_NAME(test)::FromNetwork>},
	#endif
#endif
			{} // EOL
		};
		BOOST_STATIC_ASSERT( 
			NUMBEROF(kYeloMessageDeltas)-1 == Enums::k_message_deltas_yelo_count &&
			NUMBEROF(kYeloMessageDeltaDecoders)-1 == Enums::k_message_deltas_yelo_count 
		);
#endif
	};
};