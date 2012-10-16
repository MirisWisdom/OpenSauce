/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/networking/player_update.hpp>

namespace Yelo
{
	namespace Networking
	{
		struct s_player_server_update
		{
			UNKNOWN_TYPE(uint32); // 0xE8
			UNKNOWN_TYPE(int32);  // 0xEC
			UNKNOWN_TYPE(int32);  // 0xF0
			int32 last_completed_update_id;
			real_point3d position;

			// 0x104 datum_index or int32
			// 0x108 boolean, PAD24
			// 0x10C uint32
			// 0x110 uint32
			// 0x114 uint32
			// 0x118 uint32
			// 0x11C uint32
			// 0x120 int32
			// 0x124 int32
			// 0x128 int32
			// 0x12C int32 action_baseline_id, NUMBER_OF_REMOTE_PLAYER_ACTION_UPDATE_BASELINE_IDS = 2
			// 0x130, s_remote_player_action_update_network_data
			// 0x160 uint32
			// 0x164 datum_index or int32
			// 0x168 datum_index or int32
			// 0x16C datum_index or int32
			// 0x170 real_point3d predict_position
			// 0x17C datum_index or int32
			// 0x180 datum_index or int32
			// 0x184 datum_index or int32
			// 0x188, 0x40 byte structure
		}; //BOOST_STATIC_ASSERT( sizeof(s_server_update_data) == 0xE0 );
	};
};