/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/player_action.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_action_updates_in_super_update = 16,

			k_number_of_remote_player_action_update_baseline_ids = 2,
		};
	};

	namespace Networking
	{
		struct s_remote_player_action_update_header
		{
			datum_index player_index;
			sbyte update_id;
			sbyte baseline_id; // k_number_of_remote_player_action_update_baseline_ids
		}; BOOST_STATIC_ASSERT( sizeof(s_remote_player_action_update_header) == 0x6 );

		struct s_remote_player_action_update_network_data
		{
			int32 ticks_to_apply_update_to;
			// desired_facing angles and zoom_index not synced
			Players::s_player_action action;
			real_vector3d facing_vector;
		}; BOOST_STATIC_ASSERT( sizeof(s_remote_player_action_update_network_data) == 0x30 );

		struct s_remote_player_position_update_network_data
		{
			real_point3d position;
		}; BOOST_STATIC_ASSERT( sizeof(s_remote_player_position_update_network_data) == 0xC );

		struct s_remote_player_vehicle_update_network_data
		{
			datum_index vehicle_index;
			real_point3d position;
			real_vector3d translational_velocity, angular_velocity;
			real_vector3d forward, up;
		}; BOOST_STATIC_ASSERT( sizeof(s_remote_player_vehicle_update_network_data) == 0x40 );
	};
};