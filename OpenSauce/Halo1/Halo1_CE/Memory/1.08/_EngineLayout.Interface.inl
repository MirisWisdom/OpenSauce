/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// Controls.cpp
#if __EL_INCLUDE_FILE_ID == __EL_INTERFACE_CONTROLS
	FUNC_PTR(INPUT_UPDATE, 0x493A06, FUNC_PTR_NULL);

	ENGINE_PTR(ControlSettings, Settings,	0x6AD73A, PTR_NULL);
	ENGINE_PTR(ControlStates, ControlState,	0x64C488, PTR_NULL);
	ENGINE_PTR(byte, InputStateFlags,		0x6AD4AA, PTR_NULL);

#pragma region GameUI
//////////////////////////////////////////////////////////////////////////
// GameUI.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_INTERFACE_GAME_UI
	namespace GameUI
	{
		ENGINE_DPTR(s_hud_scripted_globals, hud_scripted_globals,	0x6B43E0, 0x61CC60);
		ENGINE_DPTR(s_hud_messaging, hud_messaging,					0x64E8A8, 0x5C06B8);
		ENGINE_DPTR(s_hud_unit_interface, hud_unit_interface,		0x6B43F4, 0x61CC70);
		ENGINE_DPTR(s_hud_weapon_interface, hud_weapon_interface,	0x6B43F8, 0x61CC74);
		ENGINE_DPTR(s_hud_nav_points, hud_nav_points,				0x64E8AC, 0x5C06BC);
		ENGINE_DPTR(s_motion_sensor, motion_sensor,					0x6B4400, 0x61CC7C);
		ENGINE_DPTR(s_first_person_weapons, first_person_weapons,	0x64DC00, 0x5BFA30);

		ENGINE_PTR(s_text_bounds_globals_data, text_bounds_globals,	0x67F60C, PTR_NULL);
		ENGINE_PTR(s_font_drawing_globals_data, font_drawing_globals,0x67F620,PTR_NULL);

		FUNC_PTR(HUD_UPDATE_HOOK,									0x4ACBE3, FUNC_PTR_NULL);
		FUNC_PTR(FIRST_PERSON_WEAPONS_UPDATE_HOOK,					0x4953B8, FUNC_PTR_NULL);

		FUNC_PTR(FIRST_PERSON_WEAPON_SET_STATE_HOOK,				0x495FD9, FUNC_PTR_NULL);
	};

	namespace Hud
	{
		#pragma region ShowCrosshair
		FUNC_PTR(RENDER_CROSSHAIRS_DISABLE_MOD,					0x4B62B1, FUNC_PTR_NULL);
		#pragma endregion

		#pragma region ShowHud
		FUNC_PTR(INTERFACE_DRAW_SCREEN,							0x4976B0, FUNC_PTR_NULL);
		FUNC_PTR(RENDER_WINDOWS_CALL_HOOK_INTERFACE_DRAW_SCREEN,0x5101C1, FUNC_PTR_NULL);
		#pragma endregion

		#pragma region Scale
		// ??, _rasterizer_psuedo_dynamic_screen_quad_draw, rasterizer_psuedo_dynamic_screen_quad_draw, _rasterizer_hud_motion_sensor_blip_end
		static void* K_SCALE_ALLOWED_REFS[] = {
			CAST_PTR(void*, 0x518B4E), CAST_PTR(void*, 0x520ACC), CAST_PTR(void*, 0x520E0C), CAST_PTR(void*, 0x52F8E2),
		};
		#pragma endregion

		#pragma region render nav point
		// GAME_ENGINE_RENDER_NAV_POINTS_CALL_RENDER_NAV_POINT
			FUNC_PTR(RENDER_NAV_POINT,											0x4B2BB0, FUNC_PTR_NULL);
			FUNC_PTR(GAME_ENGINE_RENDER_NAV_POINTS_CALL_HOOK_RENDER_NAV_POINT,	0x463C61, FUNC_PTR_NULL);
		
		// HUD_RENDER_NAV_POINTS_CALL_RENDER_NAV_POINT
			FUNC_PTR(HUD_RENDER_NAV_POINTS_CALL_HOOK_RENDER_NAV_POINT,			0x4B32AD, FUNC_PTR_NULL);
		#pragma endregion

		#pragma region render team indicator
		// HUD_DRAW_PLAYERS_CALL_HUD_DRAW_FRIENDLY_INDICATOR
			FUNC_PTR(HUD_DRAW_FRIENDLY_INDICATOR,								0x4AD650, FUNC_PTR_NULL);
			FUNC_PTR(HUD_DRAW_PLAYERS_CALL_HOOK_HUD_DRAW_FRIENDLY_INDICATOR,	0x4ADAD4, FUNC_PTR_NULL);
		#pragma endregion

		ENGINE_DPTR(void, hud_globals, 0x6B43DC, PTR_NULL);
	};
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Keystone.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_INTERFACE_KEYSTONE
	FUNC_PTR(KEYSTONE_CHATLOG_ADD_ITEM_SEND_MESSAGE, 0x4AEAA0, FUNC_PTR_NULL);

	ENGINE_PTR(wchar_t, ChatLogName,	0x637D64, PTR_NULL);
	ENGINE_PTR(void, MainWindow,		0x6BD0AC, PTR_NULL);

	FUNC_PTR(KS_GETWINDOW,		0x6BD0C0, FUNC_PTR_NULL);
	FUNC_PTR(KW_RELEASE,		0x6BD0D0, FUNC_PTR_NULL);
	FUNC_PTR(KW_GETCONTROLBYID,	0x6BD0D4, FUNC_PTR_NULL);
	FUNC_PTR(KC_SENDMESSAGE,	0x6BD0F0, FUNC_PTR_NULL);

#else
	#error Undefined engine layout include for: __EL_INCLUDE_INTERFACE
#endif