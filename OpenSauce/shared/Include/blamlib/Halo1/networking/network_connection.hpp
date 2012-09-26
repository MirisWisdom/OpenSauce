/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/bungie_net/network/transport_address.hpp>
#include <blamlib/Halo1/bungie_net/network/transport_endpoint_winsock.hpp>
#include <blamlib/Halo1/bungie_net/network/transport_endpoint_set_winsock.hpp>
#include <blamlib/Halo1/memory/bitstream.hpp>
#include <blamlib/Halo1/memory/circular_queue.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_network_machine_count = 16,
		};

		enum network_connection_class : long_enum
		{
			_network_connection_class_0,
			_network_connection_class_1,
			_network_connection_class_2,
			_network_connection_class_3,
			_network_connection_class_4,
			_network_connection_class_5,
			_network_connection_class_6,
			_network_connection_class_7,
			_network_connection_class_8,

			_network_connection_class,
		};
	};

	namespace Flags
	{
		enum network_connection_flags
		{
			_connection_create_server_bit,
			_connection_create_clientside_client_bit,
			_connection_create_serverside_client_bit,
		};
	};

	namespace Networking
	{
		struct s_network_server_connection;

		struct s_message_stream
		{
			enum { k_protocol_bits = 0x2880 };

			Memory::s_bitstream bitstream;
			bool empty;
			byte buffer[k_protocol_bits / (sizeof(byte)*8)]; // 0x1D
			PAD24;
			PAD32;
		}; BOOST_STATIC_ASSERT( sizeof(s_message_stream) == 0x534 );

		struct s_connection_message // made up name, nothing to verify name with
		{
			bool has_data; PAD24; // engine sets this to false after the message stream has been flushed
			int32 priority;
			int32 headerMaxSizeInBytes;
			int32 dataMaxSizeInBytes;
			int32 header_size_in_bits;
			int32 data_size_in_bits;
			byte* header_buffer;
			byte* data_buffer;
		}; BOOST_STATIC_ASSERT( sizeof(s_connection_message) == 0x20 );
		struct s_connection_prioritization_buffer
		{
			int32 numMessages;				// 0xA78
			s_connection_message* messages;	// 0xA7C
			UNKNOWN_TYPE(uint32);			// 0xA80
			UNKNOWN_TYPE(uint32);			// 0xA84, initialize by GetTickCount
		}; BOOST_STATIC_ASSERT( sizeof(s_connection_prioritization_buffer) == 0x10 );


		struct s_network_connection
		{
			typedef void (PLATFORM_API* proc_connection_reject)(s_transport_endpoint* ep, long_enum rejection_code);

			s_transport_endpoint* reliable_endpoint;
			uint32 keep_alive_time;
			proc_connection_reject connection_rejection_proc;
			Memory::s_circular_queue* incoming_queue;
			s_message_stream reliable_outgoing_sled;					// 0x10
			s_message_stream unreliable_outgoing_sled;					// 0x544
			s_connection_prioritization_buffer prioritization_buffer;	// 0xA78
			long_enum connection_class;
			long_flags flags; // Flags::network_connection_flags
			UNUSED_TYPE(int32);
			s_network_server_connection* server_connection;
			bool is_local_connection; PAD24;
		}; BOOST_STATIC_ASSERT( sizeof(s_network_connection) == 0xA9C );
		
		struct s_network_server_connection
		{
			s_network_connection connection;
			s_transport_endpoint_set* endpoint_set;
			s_network_connection* client_list[Enums::k_maximum_network_machine_count];
			UNKNOWN_TYPE(bool);
			bool has_local_connection; PAD16;
		}; BOOST_STATIC_ASSERT( sizeof(s_network_server_connection) == 0xAE4 );
	};
};