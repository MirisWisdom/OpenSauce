/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Pointers/1.10/HaloCE_110_Runtime.Interface.inl"
#include "Pointers/1.10/HaloCE_110_Runtime_Manual.Interface.inl"
#endif

#include <blam/interface/first_person_weapons.hpp>
#include <blam/interface/hud.hpp>
#include <blam/interface/hud_chat.hpp>
#include <blam/interface/hud_messaging.hpp>
#include <blam/interface/hud_nav_points.hpp>
#include <blam/interface/hud_unit.hpp>
#include <blam/interface/hud_weapon.hpp>
#include <blam/interface/motion_sensor.hpp>
#include <blam/text/draw_string.hpp>
#include <Memory/MemoryInterface.hpp>
#include <yelo/memory/memory_interface_base.hpp>

namespace Yelo
{
	namespace GameUI
	{
		ENGINE_DPTR(s_hud_scripted_globals, hud_scripted_globals, K_HUD_SCRIPTED_GLOBALS, PTR_NULL);
		ENGINE_DPTR(s_hud_messaging, hud_messaging, K_HUD_MESSAGING, PTR_NULL);
		ENGINE_DPTR(s_hud_unit_interface, hud_unit_interface, K_HUD_UNIT_INTERFACE, PTR_NULL);
		ENGINE_DPTR(s_hud_weapon_interface, hud_weapon_interface, K_HUD_WEAPON_INTERFACE, PTR_NULL);
		ENGINE_DPTR(s_hud_nav_points, hud_nav_points, K_HUD_NAV_POINTS, PTR_NULL);
		ENGINE_DPTR(s_motion_sensor, motion_sensor, K_MOTION_SENSOR, PTR_NULL);
		ENGINE_DPTR(s_first_person_weapons, first_person_weapons, K_FIRST_PERSON_WEAPONS, PTR_NULL);

		ENGINE_PTR(s_text_bounds_globals_data, text_bounds_globals, K_TEXT_BOUNDS_GLOBALS, PTR_NULL);
		ENGINE_PTR(s_font_drawing_globals_data, font_drawing_globals,K_FONT_DRAWING_GLOBALS,PTR_NULL);

		ENGINE_PTR(s_hud_chat_globals, hud_chat_globals, K_HUD_CHAT_GLOBALS, PTR_NULL);
		ENGINE_PTR(int32, hud_chat_line_count, K_HUD_CHAT_LINE_COUNT, PTR_NULL);

		FUNC_PTR(HUD_UPDATE, K_HUD_UPDATE, FUNC_PTR_NULL);
		FUNC_PTR(HUD_UPDATE_CALL, K_HUD_UPDATE_CALL, FUNC_PTR_NULL);
		FUNC_PTR(FIRST_PERSON_WEAPONS_UPDATE, K_FIRST_PERSON_WEAPONS_UPDATE, FUNC_PTR_NULL);
		FUNC_PTR(FIRST_PERSON_WEAPONS_UPDATE_CALL, K_FIRST_PERSON_WEAPONS_UPDATE_CALL, FUNC_PTR_NULL);

		FUNC_PTR(FIRST_PERSON_WEAPON_SET_STATE_HOOK, K_FIRST_PERSON_WEAPON_SET_STATE_HOOK, FUNC_PTR_NULL);
	}
}
