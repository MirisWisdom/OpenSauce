/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Networking
	{
		enum
		{
			k_ipv4_address_length = 4,
			k_ipv6_address_length = 16,
		};

		namespace e_transport_type
		{
			typedef enum : byte_enum
			{
				udp = 17,
				tcp,

				// Name is guesswork
				gamespy = 20,

				k_count
			} type_t;
		}

		namespace e_transport_rejection_code
		{
			typedef enum : long_enum
			{
				none,
				unknown,
				too_many_connection_attempts,
				invalid_protocol,

				version_too_old,
				version_too_new,
				game_is_full,
				game_is_closed,
				blacklisted_machine,

				k_count
			} type_t;
		}

		struct s_transport_address
		{
			union
			{
				struct _ipv4
				{
					byte class_a;
					byte class_b;
					byte class_c;
					byte class_d;
				} ipv4;

				struct _ipv6
				{
					int16 class_a;
					int16 class_b;
					int16 class_c;
					int16 class_d;

					int16 class_e;
					int16 class_f;
					int16 class_g;
					int16 class_h;
				} ipv6;

				byte ip[k_ipv6_address_length];
			};

			int16 address_length;
			int16 port;

			int32 unknown;
		};

		BOOST_STATIC_ASSERT( sizeof(s_transport_address) == 0x18 );
	}
}
