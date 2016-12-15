/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/game_configuration.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct s_unit_camera;
	};

	namespace Players
	{
		struct s_player_control
		{
			datum_index unit_index;									// 0x0
			long_flags control_flags;								// 0x4
			PAD16; // unknown field									// 0x8
			PAD16; // unknown field									// 0xA
			real_euler_angles2d	 desired_angles;					// 0xC
			real_vector2d throttle;									// 0x14
			real primary_trigger;									// 0x1C
			int16 weapon_index;										// 0x20
			int16 grenade_index;									// 0x22
			int16 zoom_level;										// 0x24
			sbyte weapon_swap_ticks; // unknown field				// 0x26
			PAD8; // unknown field									// 0x27
			datum_index target_object_index;						// 0x28
			real autoaim_level;										// 0x2C
			PAD32; // unknown field									// 0x30
			PAD32;													// 0x34
			PAD32; // unknown field									// 0x38
			PAD32; // unknown field									// 0x3C
		}; BOOST_STATIC_ASSERT( sizeof(s_player_control) == 0x40 );

		struct s_player_control_globals_data
		{
			long_flags action_flags[2]; // see "action_test" script functions
			PAD32; // TODO: document the fields in the first 12 bytes of this struct
			long_flags flags; // FLAG(0) = camera control

			s_player_control local_players[Enums::k_maximum_number_of_local_players];
		}; BOOST_STATIC_ASSERT( sizeof(s_player_control_globals_data) == 0x50 );

		struct s_unit_camera_info
		{
			datum_index unit_index;
			int16 seat_index;
			PAD16;
			const TagGroups::s_unit_camera* unit_camera_definition;
			real_point3d position;
		}; BOOST_STATIC_ASSERT( sizeof(s_unit_camera_info) == 0x18 );

		s_player_control_globals_data*	PlayerControlGlobals();
	};

	namespace blam
	{
		void PLATFORM_API player_control_get_unit_camera_info(const int16 player_index, Players::s_unit_camera_info& camera_info);
	};
};