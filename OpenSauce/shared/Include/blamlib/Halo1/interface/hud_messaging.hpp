/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace GameUI
	{
		struct s_hud_messaging : TStructImpl(1160)
		{
		};
		s_hud_messaging*			HudMessaging();
	};

	namespace blam
	{
		void PLATFORM_API hud_print_message(int16 local_player_index, wcstring message);
	};
};