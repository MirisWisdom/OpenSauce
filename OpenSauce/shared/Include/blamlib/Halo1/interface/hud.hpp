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
		struct s_hud_scripted_globals
		{
			bool show_hud;
			bool show_hud_help_text;
			PAD16;
		};
		s_hud_scripted_globals*		HudScriptedGlobals();
	};

	namespace blam
	{
		wcstring PLATFORM_API hud_get_item_string(int16 reference_index);
	};
};