/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/resource_id.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI 
	{
#define K_SCREEN_MAIN_MENU			RESOURCE_ID("#SCN_main_menu",			0x249DE908)
#define K_SCREEN_MAIN_MENU_BOTTOM	RESOURCE_ID("#SCN_main_menu_bottom",	0x740E4AC2)
#define K_SCREEN_MAP_DOWNLOAD		RESOURCE_ID("#SCN_map_download",		0x4B24C75C)
#define K_SCREEN_IN_GAME			RESOURCE_ID("#SCN_in_game",				0x8E012088)
	};};
};
#endif