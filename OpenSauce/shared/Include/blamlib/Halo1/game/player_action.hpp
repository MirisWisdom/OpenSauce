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
		enum player_action_result
		{
			_player_action_result_none,
			_player_action_result_pickup1,
			_player_action_result_pickup2,
			_player_action_result_exit_seat,
			_player_action_result_4, // game engine related

			_player_action_result_swap_equipment,
			_player_action_result_swap_weapon,
			_player_action_result_pickup_weapon,

			_player_action_result_enter_seat,
			_player_action_result_force_ai_to_exit_seat,
			_player_action_result_touch_device,
			_player_action_result_flip_vehicle,

			_player_action_result,
		};
	};

	namespace Players
	{
		struct s_player_action_result
		{
			datum_index action_object_index;
			int16 action_result;			// enum object_type
			int16 action_seat_index;
		}; BOOST_STATIC_ASSERT( sizeof(s_player_action_result) == 0x8 );

		struct s_player_action
		{
			int32 update_id;
			real_euler_angles2d desired_facing;
			real_vector2d throttle;
			real primary_trigger;
			int16 weapon_index, grenade_index;
			int16 zoom_index;
			PAD16;
		}; BOOST_STATIC_ASSERT( sizeof(s_player_action) == 0x20 );

		struct s_player_set_action_result_network_data
		{
			datum_index player_index;
			long_enum action_result_type;
			s_player_action_result action;
			datum_index weapon_to_drop_as_result_of_swap;
		}; BOOST_STATIC_ASSERT( sizeof(s_player_set_action_result_network_data) == 0x14 );
	};
};