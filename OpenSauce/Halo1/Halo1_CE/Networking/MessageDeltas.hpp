/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/networking/message_delta_parameters_protocol.hpp>

#include "Networking/Networking.hpp"
#include "Networking/MessageDeltaDefinitions.hpp"

namespace Yelo
{
	namespace MessageDeltas
	{
		s_message_delta_parameters* Parameters();

		field_type_definition* GlobalFieldTypeList();
		Enums::message_delta_encoding_class* EncodingClass();

		typedef message_delta_parameter exposed_parameters_t[Enums::k_mdp_max_exposed_parameters];
		exposed_parameters_t* ExposedProtocolParameters();

		s_parameters_protocol_globals* ParametersProtocolGlobals();

		// Buffer that holds all received data from the clients or the server depending on the game connection
		byte* PacketBufferReceived();
		// Buffer that holds all data to be sent to a client or to the server depending on the game connection
		byte* PacketBufferSent();

		const message_delta_definition* const* OriginalPackets();
#ifndef YELO_NO_NETWORK
		const message_delta_definition* const* NewPackets();
	#define GET_MDP_DEFINITION(name) MessageDeltas::NewPackets()[ BOOST_PP_CAT(Yelo::Enums::_message_delta_,name) ]
#endif


		void Initialize();
		void Dispose();

		// If not yet enabled, changes networking settings for a configuration which can possibly improve game performance.
		// Returns true if steroids were applied or have already been applied.
		bool EnableNetworkingSteroids();

#ifndef YELO_NO_NETWORK
		inline bool ClientSendMessageToServer(
			size_t data_size_in_bits,
			bool unbuffered = false, bool flush_queue = false, int32 buffer_priority = Enums::k_message_highest_priority)
		{
			return Networking::ClientSendMessageToServer(
				PacketBufferSent(), data_size_in_bits,
				Enums::_network_messsage_type_message_delta, 
				unbuffered, flush_queue, buffer_priority);
		}

		inline bool SvSendMessageToMachine(int32 machine_index, 
			size_t data_size_in_bits, 
			bool unbuffered = false, bool flush_queue = false, bool write_to_local_connection = false, 
			int32 buffer_priority = Enums::k_message_highest_priority)
		{
			return Networking::SvSendMessageToMachine(machine_index,
				PacketBufferSent(), data_size_in_bits,
				Enums::_network_messsage_type_message_delta, 
				unbuffered, flush_queue, write_to_local_connection, 
				buffer_priority);
		}
		inline bool SvSendMessageToAll(
			size_t data_size_in_bits, 
			BOOL unbuffered = false, BOOL flush_queue = false, BOOL write_to_local_connection = false, 
			int32 buffer_priority = Enums::k_message_highest_priority)
		{
			return Networking::SvSendMessageToAll(
				PacketBufferSent(), data_size_in_bits,
				Enums::_network_messsage_type_message_delta, 
				unbuffered, flush_queue, write_to_local_connection, 
				buffer_priority);
		}
		inline bool SvSendMessageToAllIngame(
			size_t data_size_in_bits, 
			BOOL unbuffered = false, BOOL flush_queue = false, BOOL write_to_local_connection = false, 
			int32 buffer_priority = Enums::k_message_highest_priority)
		{
			return Networking::SvSendMessageToAllIngame(
				PacketBufferSent(), data_size_in_bits,
				Enums::_network_messsage_type_message_delta, 
				unbuffered, flush_queue, write_to_local_connection, 
				buffer_priority);
		}

		// [message_delta_mode]
		// [message_delta]
		// pointer array (0,1-n) to the to headers of [datas]
		// pointer array (0?,1-n) to the data structures to encode
		// pointer array (0,1-n) for the base line updates for each of the datas
		// # of iterations
		// ?
		int32 MdpiEncode(long_enum mode, long_enum definition_type, 
			const void* buffer, size_t buffer_size_in_bits, 
			const void** headers, const void** datas, const void** baselines = nullptr, 
			int32 iterations = 1, int32 /*bool*/ unk = false);

		int32 EncodeStateless(long_enum definition_type, 
			const void* source_header, const void* source_data, 
			void* buffer, size_t buffer_size_in_bits);
		int32 EncodeStateless(long_enum definition_type, 
			const void* source_header, const void* source_data);

		// Decodes the current data in the packet buffer, and puts the data into [header] if there is a header, and [destination_data] for the main packet body
		bool DecodeStatelessIterated(message_dependant_header* header, void* destination_data);

		bool DecodeIncrementalIterated(message_dependant_header* header, void* destination_data, 
			void* baseline_data, bool has_no_iteration_body = false);

		// This is called when in a decode function for a mdp type, the call to network_decode_message fails, so it removes the data sent to this machine
		void DiscardIterationBody(message_dependant_header* header);
#endif
	};
};