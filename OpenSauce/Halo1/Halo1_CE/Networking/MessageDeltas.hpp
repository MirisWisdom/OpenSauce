/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include "Networking/Networking.hpp"
#include "Networking/MessageDeltaDefinitions.hpp"

namespace Yelo
{
	namespace Enums
	{
	};

	namespace MessageDeltas
	{
		struct s_message_delta_parameters
		{
			struct s_locality_reference_position {
				uint32 bits_per_component_full;		// 25
				uint32 bits_per_component_delta;	// 9
				real delta_cutoff_distance;			// 8.0
				real minimum_move_distance;			// 0.0099999998
				uint32 bits_per_component_lan;		// 31
			}locality;

			struct s_item_placement {
				uint32 bits_x; // 20
				uint32 bits_y; // 20
				uint32 bits_z; // 30
			}item_placement;
		}; BOOST_STATIC_ASSERT( sizeof(s_message_delta_parameters) == 0x20 );
		s_message_delta_parameters* Parameters();

		field_type_definition* GlobalFieldTypeList();
		Enums::message_delta_encoding_class* EncodingClass();

		typedef message_delta_parameter exposed_parameters_t[Enums::k_mdp_max_exposed_parameters];
		exposed_parameters_t* ExposedProtocolParameters();

		struct s_parameters_protocol_globals // made-up structure
		{
			bool changeover_enabled;		// g_protocol_changeover_enabled
			PAD24;
			int32 version;					// protocol version
			int32 exposed_params_count;		// g_num_exposed_params
			bool encoding_parameters_set;	// true when a parameters_set is being encoded and queued to be sent across the network
			PAD24;
		}; BOOST_STATIC_ASSERT( sizeof(s_parameters_protocol_globals) == 0x10 );
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
		// Writes [data_size_in_bits] of the packet buffer to the server connection
		// returns the size in bits of the header
		int32 SvWrite(int32 data_size_in_bits, 
			int32 /*bool*/ dont_bit_encode = true, int32 /*bool*/ dont_flush = false);

		// [message_delta_mode]
		// [message_delta]
		// pointer array (0,1-n) to the to headers of [datas]
		// pointer array (0?,1-n) to the data structures to encode
		// pointer array (0,1-n) for the base line updates for each of the datas
		// # of iterations
		// ?
		int32 EncodeStateless(long_enum mode, long_enum def_type, void** headers, void** datas, 
			void** baselines = NULL, int32 iterations = 1, int32 /*bool*/ unk = false);

		// Decodes the current data in the packet buffer, and puts the data into [header] if there is a header, and [out_buffer] for the main packet body
		bool DecodeStatelessIterated(void* header, void* out_buffer);

		//bool ConnectionFlush(int32 /*bool*/ unk = 1);

		// Sends [data_size_in_bits] of the packet buffer to the server
		bool ClientSendMessageToServer(int32 data_size_in_bits);

		// Sends [data_size_in_bits] of the packet buffer to all machines
		bool SvSendMessageToAll(int32 data_size_in_bits, 
			int32 /*bool*/ dont_bit_encode = true, int32 /*bool*/ dont_flush = false, int32 /*bool*/ send_even_after_fail = false, int32 shit = -1);

		// Sends [data_size_in_bits] of the packet buffer to [machine_index]
		bool SvSendMessageToMachine(int32 machine_index, int32 data_size_in_bits, 
			int32 /*bool*/ dont_bit_encode = true, int32 /*bool*/ dont_flush = false, int32 /*bool*/ send_even_after_fail = false, int32 shit = -1);

		// This is called when in a decode function for a mdp type, the call to network_decode_message fails, so it removes the data sent to this machine
		void DiscardIterationBody(void* header);
#endif
	};
};