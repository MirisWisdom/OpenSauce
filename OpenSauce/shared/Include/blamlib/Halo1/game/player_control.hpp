/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/players.hpp>

namespace Yelo
{
	namespace Players
	{
		struct s_player_control : TStructImpl(0x40)
		{
			TStructGetPtrImpl(datum_index, UnitIndex, 0x0);

			// 0x8, word
			// 0xA, word
			TStructGetPtrImpl(real_euler_angles2d, DesiredAngles, 0xC); // facing
			TStructGetPtrImpl(real_vector2d, Throttle, 0x14);
			TStructGetPtrImpl(real, PrimaryTrigger, 0x1C);
			TStructGetPtrImpl(int16, CurrentWeaponIndex, 0x20);
			TStructGetPtrImpl(int16, CurrentGrenadeIndex, 0x22);
			TStructGetPtrImpl(int16, ZoomLevel, 0x24);
			TStructGetPtrImpl(sbyte, WeaponSwapTicks, 0x26);
			// 0x27, sbyte, unused ticks counter
			TStructGetPtrImpl(datum_index, TargetUnitIndex, 0x28);
			TStructGetPtrImpl(real, AutoaimLevel, 0x2C);
			// 0x30, real

			// 0x38, real
			// 0x3C, real
		};
		struct s_player_control_globals_data
		{
			long_flags action_flags[2]; // see "action_test" script functions
			PAD32; // TODO: document the fields in the first 12 bytes of this struct
			long_flags flags; // FLAG(0) = camera control

			s_player_control local_players[Enums::k_maximum_number_of_local_players];
		}; BOOST_STATIC_ASSERT( sizeof(s_player_control_globals_data) == 0x50 );
	};
};