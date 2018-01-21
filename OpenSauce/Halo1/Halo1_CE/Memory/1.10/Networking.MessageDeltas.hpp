/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Memory/1.10/Pointers/HaloCE_110_Runtime.Networking.inl"
#include "Memory/1.10/Pointers/HaloCE_110_Runtime_Manual.Networking.inl"
#else
#include "Memory/1.10/Pointers/HaloCE_110_Dedi.Networking.inl"
#include "Memory/1.10/Pointers/HaloCE_110_Dedi_Manual.Networking.inl"
#endif

#include <blamlib/networking/message_delta_definitions.hpp>
#include <blamlib/networking/message_delta_parameters_protocol.hpp>
#include <blamlib/networking/network_server_manager_structures.hpp>
#include <Memory/MemoryInterface.hpp>
#include <Networking/MessageDefinitions.hpp>
#include <yelolib/memory/memory_interface_base.hpp>

namespace Yelo
{
	namespace MessageDeltas
	{
		ENGINE_PTR(s_message_delta_parameters, message_delta_parameters, DUO_PTR(K_MESSAGE_DELTA_PARAMETERS));
		ENGINE_PTR(field_type_definition, message_delta_global_field_type_list, DUO_PTR(K_MESSAGE_DELTA_GLOBAL_FIELD_TYPE_LIST));
		ENGINE_PTR(Enums::message_delta_encoding_class, g_message_delta_encoding_class, DUO_PTR(K_G_MESSAGE_DELTA_ENCODING_CLASS));
		ENGINE_PTR(exposed_parameters_t, g_exposed_params, DUO_PTR(K_G_EXPOSED_PARAMS));
		ENGINE_PTR(s_parameters_protocol_globals, mdp_parameters_protocol_globals, DUO_PTR(K_MDP_PARAMETERS_PROTOCOL_GLOBALS));

		ENGINE_PTR(byte, mdp_packet_buffer_received_data, DUO_PTR(K_MDP_PACKET_BUFFER_RECEIVED_DATA));
		ENGINE_PTR(byte, mdp_packet_buffer_sent_data, DUO_PTR(K_MDP_PACKET_BUFFER_SENT_DATA));

		ENGINE_DPTR(message_delta_definition, message_delta_packets, DUO_PTR(K_MESSAGE_DELTA_PACKETS));

		#ifndef YELO_NO_NETWORK
		#pragma region MessageDeltaPointerReferences
		static void** MessageDeltaPointerReferences[] = {
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_0),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_1),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_2),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_3),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_4),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_5),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_6),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_7),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_8),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_9),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_10),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_11),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_12),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_13),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_14),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_15),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_16),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_17),
		};
		#pragma endregion

		#pragma region MessageDeltaPointerEndChecks
		static void** MessageDeltaPointerEndChecks[] = {
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_END_CHECK_0),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_END_CHECK_1),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_END_CHECK_2),
		};

		static byte* MessageDeltaTypeCountChecks8bit[] = {
			CAST_PTR(byte*, K_MESSAGE_DELTA_TYPE_COUNT_CHECK_8BIT_0),
		};
		#pragma endregion

		FUNC_PTR(NETWORK_GAME_CLIENT_GAME_SETTINGS_UPDATED, DUO_PTR(K_NETWORK_GAME_CLIENT_GAME_SETTINGS_UPDATED));
		FUNC_PTR(NETWORK_GAME_CLIENT_GAME_SETTINGS_UPDATED_CALL1, DUO_PTR(K_NETWORK_GAME_CLIENT_GAME_SETTINGS_UPDATED_CALL1));
		FUNC_PTR(NETWORK_GAME_CLIENT_GAME_SETTINGS_UPDATED_CALL2, DUO_PTR(K_NETWORK_GAME_CLIENT_GAME_SETTINGS_UPDATED_CALL2));

		FUNC_PTR(NETWORK_GAME_CLIENT_HANDLE_MESSAGE_DELTA_MESSAGE_BODY_CALL, DUO_PTR(K_NETWORK_GAME_CLIENT_HANDLE_MESSAGE_DELTA_MESSAGE_BODY_CALL));
		DATA_PTR(DONT_SEND_OBJECT_NEW_MSG, DUO_PTR(K_DONT_SEND_OBJECT_NEW_MSG));
		FUNC_PTR(NETWORK_GAME_CLIENT_HANDLE_MESSAGE_DELTA_MESSAGE_BODY, DUO_PTR(K_NETWORK_GAME_CLIENT_HANDLE_MESSAGE_DELTA_MESSAGE_BODY));


		ENGINE_PTR(Networking::s_network_game_server, global_network_game_server_data, DUO_PTR(K_GLOBAL_NETWORK_GAME_SERVER_DATA));
		ENGINE_PTR(Networking::s_network_game_client, global_network_game_client_data, DUO_PTR(K_GLOBAL_NETWORK_GAME_CLIENT_DATA));

		FUNC_PTR(MDPI_ENCODE, DUO_PTR(K_MDPI_ENCODE));
		FUNC_PTR(MDP_DECODE_STATELESS_ITERATED, DUO_PTR(K_MDP_DECODE_STATELESS_ITERATED));
		FUNC_PTR(MDP_DECODE_INCREMENTAL_ITERATED, DUO_PTR(K_MDP_DECODE_INCREMENTAL_ITERATED));
		FUNC_PTR(MDP_DISCARD_ITERATION_BODY, DUO_PTR(K_MDP_DISCARD_ITERATION_BODY));
		#endif
	}
}
