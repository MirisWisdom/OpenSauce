/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace GameState
	{
		struct s_yelo_header_data
		{
			PAD8; // since the unused 32 bytes in s_header_data is more than likely a tag_string, we don't want to touch what is actually the first char (we want it to stay zero)
			struct {
				bool initialized : 1;
				bool game_state_upgrades_on : 1;
				bool update_unit_grenade_types_count : 1;
			}flags;									// 0x1
			struct {
				byte major;							// 0x2
				byte minor;							// 0x3
				uint16 build;						// 0x4
			}version;
			byte unit_grenade_types_count;			// 0x6
			PAD8;
		}; BOOST_STATIC_ASSERT( sizeof(s_yelo_header_data) <= 0x20 );
	};
};