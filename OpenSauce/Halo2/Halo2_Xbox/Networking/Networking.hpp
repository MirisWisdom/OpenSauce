/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			_ipv4,
			_ipv6,

			k_ipv4_address_length = 4,
			k_ipv6_address_length = 16,
		};

		enum transport_type : _enum
		{
			_transport_type_udp = 2,
			_transport_type_vdp,
			_transport_type_tcp,
			_transport_type_unix,
			_transport_type,
		};

		enum socket_option
		{
			// allow local address reuse
			_socket_option_reuse_address,
			// linger on close if data present
			_socket_option_linger,
			// permit sending of broadcast msgs
			_socket_option_broadcast,
			// send buffer size
			_socket_option_include_write_size,
			// receive buffer size
			_socket_option_include_read_size,
			// 0x4001
			_socket_option_unknown,
			_socket_option,
		};

		enum bitstream_state : long_enum
		{
			_bitstream_state_none,
			_bitstream_state_writing,
			_bitstream_state_write_finished,
			_bitstream_state_reading,
			_bitstream_state_read_and_write,
			_bitstream_state_5,				// haven't spent time to figure this one out
		};
	};

	namespace Networking
	{
		class c_bitstream
		{
			byte* data;
			uint32 data_size_bytes;
			uint32 data_size_alignment;
			Enums::bitstream_state state;
			int32 current_bit_position;
			bool is_debug_stream;
			PAD24;
			int32 position_stack_depth;
			uint32 stack[4];
			UNKNOWN_TYPE(int32);
			UNKNOWN_TYPE(int32);
		}; BOOST_STATIC_ASSERT( sizeof(c_bitstream) == 0x34 );

		struct transport_address
		{
			union
			{
				struct _ipv4 {
					byte class_a;
					byte class_b;
					byte class_c;
					byte class_d;
				}ipv4;

				struct _ipv6 {
					int16 class_a;
					int16 class_b;
					int16 class_c;
					int16 class_d;

					int16 class_e;
					int16 class_f;
					int16 class_g;
					int16 class_h;
				}ipv6;

				uint32 ip_ulong;
				byte ip[16];
			};
			int16 port;
			int16 address_length;

			int32 unknown;

			// Converts this address description into sockaddr format
			// Assumes this is a ipv4 address. Ipv6 code is available
			// but wasn't added.
			sockaddr_in* to_sockaddr_in(sockaddr_in* sd);
		};

		struct transport_endpoint
		{
			SOCKET socket;
			word_flags flags;
			Enums::transport_type type;
		};

		transport_address* transport_address_create(transport_address* address, Enums::transport_type type, int16 port);
		transport_address* transport_address_create(transport_address* address, Enums::transport_type type, cstring ip, int16 port);


		transport_endpoint* transport_endpoint_create(short transport_type);

		// returns: true if the option is set
		bool transport_endpoint_get_option(transport_endpoint* endpoint, long_enum socket_option);

		// returns: true if the option was successfully set
		bool transport_endpoint_set_option(transport_endpoint* endpoint, long_enum socket_option);

		bool transport_endpoint_bind(transport_endpoint* endpoint, transport_address* address);

		void transport_endpoint_close(transport_endpoint* endpoint);

		// returns: bytes actually read
		int16 transport_endpoint_read(transport_endpoint* endpoint, void* buffer, int16 buffer_length);

		// returns: bytes actually written
		int16 transport_endpoint_write(transport_endpoint* endpoint, void* buffer, int16 buffer_length);

		// returns: bytes actually read
		int16 transport_endpoint_read_from(transport_endpoint* endpoint, sockaddr_in* source, void* buffer, int16 buffer_length);

		// returns: bytes actually written
		int16 transport_endpoint_write(transport_endpoint* endpoint, transport_address* address, void* buffer, int16 buffer_length);

		// returns: true if successful
		bool transport_endpoint_connect(transport_endpoint* endpoint, transport_address* address);

		// returns: true if connected
		bool transport_endpoint_async_is_connected(transport_endpoint* endpoint, bool* is_connected);
	};
};