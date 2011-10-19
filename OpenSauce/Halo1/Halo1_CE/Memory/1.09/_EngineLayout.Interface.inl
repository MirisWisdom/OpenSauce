/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//////////////////////////////////////////////////////////////////////////
// Controls.cpp
#if __EL_INCLUDE_FILE_ID == __EL_INTERFACE_CONTROLS
	FUNC_PTR(INPUT_UPDATE, 0x493A16, FUNC_PTR_NULL);

	ENGINE_PTR(ControlSettings, Settings,	0x6AD822, PTR_NULL);
	ENGINE_PTR(ControlStates, ControlState,	0x64C570, PTR_NULL);
	ENGINE_PTR(byte, InputStateFlags,		0x6AD592, PTR_NULL);

#pragma region GameUI
//////////////////////////////////////////////////////////////////////////
// GameUI.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_INTERFACE_GAME_UI
	namespace GameUI
	{
		// TODO: I stopped giving a flying fuck about server addresses here when upgrading to 1.09.
		// You've been warned.
		ENGINE_DPTR(s_hud_scripted_globals, hud_scripted_globals,	0x6B44C8, PTR_NULL);
		ENGINE_DPTR(s_hud_messaging, hud_messaging,					0x64E990, PTR_NULL);
		ENGINE_DPTR(s_hud_unit_interface, hud_unit_interface,		0x6B44DC, PTR_NULL);
		ENGINE_DPTR(s_hud_weapon_interface, hud_weapon_interface,	0x6B44E0, PTR_NULL);
		ENGINE_DPTR(s_hud_nav_points, hud_nav_points,				0x64E994, PTR_NULL);
		ENGINE_DPTR(s_motion_sensor, motion_sensor,					0x6B44E8, PTR_NULL);
		ENGINE_DPTR(s_first_person_weapons, first_person_weapons,	0x64DCE8, PTR_NULL);

		ENGINE_PTR(s_text_bounds_globals_data, text_bounds_globals,	0x67F6F4, PTR_NULL);
		ENGINE_PTR(s_font_drawing_globals_data, font_drawing_globals,0x67F708,PTR_NULL);

		FUNC_PTR(HUD_UPDATE_HOOK,									0x4ACC13, FUNC_PTR_NULL);
		FUNC_PTR(FIRST_PERSON_WEAPONS_UPDATE_HOOK,					0x4953C8, FUNC_PTR_NULL);

		FUNC_PTR(FIRST_PERSON_WEAPON_SET_STATE_HOOK,				0x495FE9, FUNC_PTR_NULL);
	};

	namespace Hud
	{
		#pragma region ShowCrosshair
		FUNC_PTR(RENDER_CROSSHAIRS_DISABLE_MOD,					0x4B62E1, FUNC_PTR_NULL);
		#pragma endregion

		#pragma region ShowHud
		FUNC_PTR(INTERFACE_DRAW_SCREEN,							0x4976C0, FUNC_PTR_NULL);
		FUNC_PTR(RENDER_WINDOWS_CALL_HOOK_INTERFACE_DRAW_SCREEN,0x5101F1, FUNC_PTR_NULL);
		#pragma endregion

		#pragma region Scale
		// ??, _rasterizer_psuedo_dynamic_screen_quad_draw, rasterizer_psuedo_dynamic_screen_quad_draw, _rasterizer_hud_motion_sensor_blip_end
		static void* K_SCALE_ALLOWED_REFS[] = {
			CAST_PTR(void*, 0x518B7E), CAST_PTR(void*, 0x520AFC), CAST_PTR(void*, 0x520E3C), CAST_PTR(void*, 0x52F912),
		};
		#pragma endregion

		#pragma region render nav point
		// GAME_ENGINE_RENDER_NAV_POINTS_CALL_RENDER_NAV_POINT
			FUNC_PTR(RENDER_NAV_POINT,											0x4B2BE0, FUNC_PTR_NULL);
			FUNC_PTR(GAME_ENGINE_RENDER_NAV_POINTS_CALL_HOOK_RENDER_NAV_POINT,	0x463C71, FUNC_PTR_NULL);
		
		// HUD_RENDER_NAV_POINTS_CALL_RENDER_NAV_POINT
			FUNC_PTR(HUD_RENDER_NAV_POINTS_CALL_HOOK_RENDER_NAV_POINT,			0x4B32DD, FUNC_PTR_NULL);
		#pragma endregion

		#pragma region render team indicator
		// HUD_DRAW_PLAYERS_CALL_HUD_DRAW_FRIENDLY_INDICATOR
			FUNC_PTR(HUD_DRAW_FRIENDLY_INDICATOR,								0x4AD680, FUNC_PTR_NULL);
			FUNC_PTR(HUD_DRAW_PLAYERS_CALL_HOOK_HUD_DRAW_FRIENDLY_INDICATOR,	0x4ADB04, FUNC_PTR_NULL);
		#pragma endregion

		ENGINE_DPTR(void, hud_globals, 0x6B44C4, PTR_NULL);
	};
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Keystone.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_INTERFACE_KEYSTONE
	FUNC_PTR(KEYSTONE_CHATLOG_ADD_ITEM_SEND_MESSAGE, 0x4AEAD0, FUNC_PTR_NULL);

	ENGINE_PTR(wchar_t, ChatLogName,	0x637D64, PTR_NULL);
	ENGINE_PTR(void, MainWindow,		0x6BD194, PTR_NULL);

	FUNC_PTR(KS_GETWINDOW,		0x6BD1A8, FUNC_PTR_NULL);
	FUNC_PTR(KW_RELEASE,		0x6BD1B8, FUNC_PTR_NULL);
	FUNC_PTR(KW_GETCONTROLBYID,	0x6BD1BC, FUNC_PTR_NULL);
	FUNC_PTR(KC_SENDMESSAGE,	0x6BD1D8, FUNC_PTR_NULL);

#else
	#error Undefined engine layout include for: __EL_INCLUDE_INTERFACE
#endif