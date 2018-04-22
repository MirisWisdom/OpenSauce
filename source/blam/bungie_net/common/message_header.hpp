/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blam/cseries/cseries_base.hpp>
#include <blam/cseries/enum_templates.h>
#include <yelo/cseries/cseries_yelo_base.hpp>

namespace Yelo
{
	namespace Networking
	{
		enum
		{
			k_maximum_message_size = 4095,
		};

		enum
		{
			k_number_of_message_types,
		};

		namespace e_message_type
		{
			typedef enum : _enum
			{
				none,
				lowlevel_error,
				data,
				// unused
				packet,

				k_count
			} type_t;
		}

		// no code actually uses these flags
		namespace e_message_flags
		{
			typedef enum : _enum
			{
				encrypted_bit,
				unknown1_bit,

				k_count
			} type_t;

			typedef flags_type<type_t> flags_t;
		}

		struct s_message_header
		{
			uint16 flags : bitfield_size<e_message_flags::k_count>::value;
			uint16 type : bitfield_enum_size<e_message_type::k_count>::value;
			uint16 size : bitfield_size<k_maximum_message_size>::value;
		};

		ASSERT_SIZE(s_message_header, 0x2);

		struct s_message_lowlevel_error
		{
			char error_string[128];
			byte error;
		};

		ASSERT_SIZE(s_message_lowlevel_error, 0x81);
	}
}
