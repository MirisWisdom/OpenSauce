/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once
#if !PLATFORM_IS_DEDI

#include "Interface/ControlTypes.hpp"
#define YELO_HAS_VEHICLE_VIEWS FALSE // vehicle views is currently unimplemented

namespace Yelo
{
	namespace Enums
	{
		enum settings_menu : int32 {
			_settings_menu_off = NONE,
			_settings_menu_on = 0,

			_settings_menu_fov = 1,
			_settings_menu_weapons,
			_settings_menu_hud,
			_settings_menu_vehicles,
			_settings_menu,

			_settings_menu__first_catagory = _settings_menu_fov,
			_settings_menu__last_catagory = _settings_menu-1,
		};

		enum settings_menu_key {
			k_settings_menu_key = Enums::_KeyF7,

			k_settings_menu_base_key =		Enums::_Key1-1, // base key used in figuring out the category selection
			k_settings_menu_fov_key,		// +1
			k_settings_menu_weapons_key,	// +2
			k_settings_menu_hud_key,		// +3
#if YELO_HAS_VEHICLE_VIEWS == TRUE
			k_settings_menu_vehicles_key,	// +4
#endif
			k_settings_menu_last_key,

			k_settings_menu_key_first_catagory = k_settings_menu_base_key+1,
			k_settings_menu_key_last_catagory = k_settings_menu_last_key-1,
		};
	};
};
#endif