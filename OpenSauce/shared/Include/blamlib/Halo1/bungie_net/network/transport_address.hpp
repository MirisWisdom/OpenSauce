/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_ipv4_address_length = 4,
			k_ipv6_address_length = 16,
		};

		enum transport_type : byte_enum
		{
			_transport_type_udp = 17,
			_transport_type_tcp,

			_transport_type_gamespy = 20, // I just pulled this name out of my ass
		};

		enum transport_rejection_code : long_enum
		{
			_transport_rejection_code_none,
			_transport_rejection_code_unknown,
			_transport_rejection_code_too_many_connection_attempts,
			_transport_rejection_code_invalid_protocol,

			_transport_rejection_code_version_too_old,
			_transport_rejection_code_version_too_new,
			_transport_rejection_code_game_is_full,
			_transport_rejection_code_game_is_closed,
			_transport_rejection_code_blacklisted_machine,
			_transport_rejection_code,
		};
	};

	namespace Networking
	{
		struct s_transport_address
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

				byte ip[Enums::k_ipv6_address_length];
			};
			int16 address_length;
			int16 port;

			int32 unknown;
		}; BOOST_STATIC_ASSERT( sizeof(s_transport_address) == 0x18 );
	};
};