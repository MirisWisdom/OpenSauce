/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

namespace Yelo
{
	namespace GameState
	{
		struct s_user_interface_controller : TStructImpl(136)
		{
			// floats 0x60, 0x64
			// floats 0x68, 0x6C, 0x70, 0x74, 0x78, 0x7C
			// 2 floats @ 0x80,0x84, appear to be the left and right vibration motor values
		};
		struct s_user_interface_controller_globals : TStructImpl(556)
		{
			TStructGetPtrImpl(s_user_interface_controller, Users, 0x0); // [4]
			// 3 floats @ 0x220, 0x224, 0x228
		};
		s_user_interface_controller_globals* _UserInterfaceControllerGlobals();
	};
};