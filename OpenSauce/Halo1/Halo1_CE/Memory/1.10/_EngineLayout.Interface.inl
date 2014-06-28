/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// Controls.cpp
#if __EL_INCLUDE_FILE_ID == __EL_INTERFACE_CONTROLS
	FUNC_PTR(INPUT_UPDATE_HOOK,				0x493856, FUNC_PTR_NULL);

	ENGINE_PTR(ControlSettings, Settings,	0x6AD802, PTR_NULL);
	ENGINE_PTR(ControlStates, ControlState,	0x64C550, PTR_NULL);
	ENGINE_PTR(byte, InputStateFlags,		0x6AD572, PTR_NULL);

#pragma region GameUI
//////////////////////////////////////////////////////////////////////////
// GameUI.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_INTERFACE_GAME_UI
	namespace GameUI
	{
		ENGINE_DPTR(s_hud_scripted_globals, hud_scripted_globals,	0x6B44A8, PTR_NULL);
		ENGINE_DPTR(s_hud_messaging, hud_messaging,					0x64E970, PTR_NULL);
		ENGINE_DPTR(s_hud_unit_interface, hud_unit_interface,		0x6B44BC, PTR_NULL);
		ENGINE_DPTR(s_hud_weapon_interface, hud_weapon_interface,	0x6B44C0, PTR_NULL);
		ENGINE_DPTR(s_hud_nav_points, hud_nav_points,				0x64E974, PTR_NULL);
		ENGINE_DPTR(s_motion_sensor, motion_sensor,					0x6B44C8, PTR_NULL);
		ENGINE_DPTR(s_first_person_weapons, first_person_weapons,	0x64DCC8, PTR_NULL);

		ENGINE_PTR(s_text_bounds_globals_data, text_bounds_globals,	0x67F6D4, PTR_NULL);
		ENGINE_PTR(s_font_drawing_globals_data, font_drawing_globals,0x67F6E8,PTR_NULL);

		ENGINE_PTR(s_hud_chat_globals, hud_chat_globals,			0x64E788, PTR_NULL);
		ENGINE_PTR(int32, hud_chat_line_count,						0x6B44B4, PTR_NULL);

		FUNC_PTR(HUD_UPDATE,										0x4AC9A0, FUNC_PTR_NULL);
		FUNC_PTR(HUD_UPDATE_CALL,									0x45C1A2, FUNC_PTR_NULL);
		FUNC_PTR(FIRST_PERSON_WEAPONS_UPDATE,						0x495190, FUNC_PTR_NULL);
		FUNC_PTR(FIRST_PERSON_WEAPONS_UPDATE_CALL,					0x45C17E, FUNC_PTR_NULL);

		FUNC_PTR(FIRST_PERSON_WEAPON_SET_STATE_HOOK,				0x495E19, FUNC_PTR_NULL);
	};

	namespace Hud
	{
		#pragma region ShowCrosshair
		ENGINE_PTR(uint16, RENDER_CROSSHAIRS_DISABLE_MOD,				0x4B60F1, PTR_NULL);
		#pragma endregion

		#pragma region Scale
		FUNC_PTR(RENDER_WIDGET_RECURSIVE,							0x49D850, PTR_NULL);
		static void** K_RENDER_WIDGET_RECURSIVE_CALLS[] =
		{
			CAST_PTR(void**, 0x49B42A), CAST_PTR(void**, 0x49B506), CAST_PTR(void**, 0x49DB4E),
			CAST_PTR(void**, 0x49E589), CAST_PTR(void**, 0x49EA81), CAST_PTR(void**, 0x49EABF)
		};

		static void* K_RASTERIZER_RENDER_UI_SET_SCREENPROJ_CALLS[] =
		{ CAST_PTR(void*, 0x520485), CAST_PTR(void*, 0x534C57), CAST_PTR(void*, 0x52F429) };
		#pragma endregion

		#pragma region render nav points
		FUNC_PTR(HUD_RENDER_NAV_POINTS,								0x4B2F80, PTR_NULL);
		FUNC_PTR(HUD_RENDER_NAV_POINTS_CALL,						0x4ACAF3, PTR_NULL);

		ENGINE_PTR(uint32, HUD_RENDER_NAV_POINT_ANCHOR_HALF_X,		0x4B2B05+1, PTR_NULL);
		ENGINE_PTR(uint32, HUD_RENDER_NAV_POINT_ANCHOR_HALF_Y,		0x4B2B0E+2, PTR_NULL);

		ENGINE_PTR(real, HUD_ANCHOR_SCREEN_X,						0x6122E0-0x78, PTR_NULL);
		ENGINE_PTR(real, HUD_ANCHOR_SCREEN_Y,						0x6122DC-0x78, PTR_NULL);
		ENGINE_PTR(real, HUD_ANCHOR_SCREEN_HALF_X,					0x612954-0x70, PTR_NULL);
		ENGINE_PTR(real, HUD_ANCHOR_SCREEN_HALF_Y,					0x612958-0x70, PTR_NULL);
		#pragma endregion

		#pragma region render team indicator
		FUNC_PTR(HUD_RENDER_PLAYERS,								0x4AD830, PTR_NULL);
		FUNC_PTR(HUD_RENDER_PLAYERS_CALL,							0x4ACA74, PTR_NULL);
		#pragma endregion

		#pragma region render scoreboard
		FUNC_PTR(HUD_RENDER_SCOREBOARD_INGAME,						0x45FC50, PTR_NULL);
		FUNC_PTR(HUD_RENDER_SCOREBOARD_INGAME_CALL,					0x497892, PTR_NULL);
		FUNC_PTR(HUD_RENDER_SCOREBOARD_POSTGAME,					0x45E090, PTR_NULL);
		FUNC_PTR(HUD_RENDER_SCOREBOARD_POSTGAME_CALL,				0x497889, PTR_NULL);
		#pragma endregion

		#pragma region hud
		ENGINE_PTR(uint32, HUD_POINT_ANCHOR_WIDTH,					0x4AEAEF+1, PTR_NULL);
		ENGINE_PTR(uint32, HUD_POINT_ANCHOR_HEIGHT,					0x4AEB21+1, PTR_NULL);
		ENGINE_PTR(uint32, HUD_POINT_ANCHOR_HALF_WIDTH,				0x4AEB41+1, PTR_NULL);
		ENGINE_PTR(uint32, HUD_POINT_ANCHOR_HALF_HEIGHT,			0x4AEB65+1, PTR_NULL);

		ENGINE_PTR(uint32, HUD_POINT_DAMAGE_ANCHOR_WIDTH,			0x4B49A8+1, PTR_NULL);
		ENGINE_PTR(uint32, HUD_POINT_DAMAGE_ANCHOR_HEIGHT,			0x4B4981+1, PTR_NULL);
		ENGINE_PTR(real, HUD_POINT_DAMAGE_ANCHOR_HALF_HEIGHT,		0x4B49BF+4, PTR_NULL);

		static void** K_HUD_POINT_DAMAGE_ANCHOR_HALF_WIDTH_PTRS[] =
		{
			CAST_PTR(void**, 0x4B499C+2), CAST_PTR(void**, 0x4B495C+2)
		};
		#pragma endregion

		#pragma region multiplayer icons
		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_CTF_X,				0x4B467F+5, PTR_NULL);
		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_CTF_Y,				0x4B4686+5, PTR_NULL);
		
		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_KING_X,				0x4B46B1+5, PTR_NULL);

		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_ODDBALL_X,			0x4B46DC+5, PTR_NULL);
		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_ODDBALL_Y,			0x4B46E3+5, PTR_NULL);

		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_RACE_X,				0x4B470B+5, PTR_NULL);
		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_RACE_Y,				0x4B4712+5, PTR_NULL);

		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_SLAYER_X,			0x4B473A+5, PTR_NULL);

		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_BACKGROUND_X,		0x4B47E5+5, PTR_NULL);
		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_BACKGROUND_Y,		0x4B47EC+5, PTR_NULL);
		#pragma endregion

		ENGINE_DPTR(void, hud_globals,								0x6B44C4-0x20, PTR_NULL);
	};
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Keystone.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_INTERFACE_KEYSTONE
	FUNC_PTR(KEYSTONE_CHATLOG_ADD_ITEM_SEND_MESSAGE, 0x0x4AE8A0, FUNC_PTR_NULL);

	ENGINE_PTR(wchar_t, ChatLogName,		0x637D54, PTR_NULL);
	ENGINE_PTR(void**, MainWindow,			0x6BD174, PTR_NULL);

	FUNC_PTR(KS_GETWINDOW,					0x6BD1A8-0x20, FUNC_PTR_NULL);
	FUNC_PTR(KW_RELEASE,					0x6BD1B8-0x20, FUNC_PTR_NULL);
	FUNC_PTR(KW_GETCONTROLBYID,				0x6BD1BC-0x20, FUNC_PTR_NULL);
	FUNC_PTR(KC_SENDMESSAGE,				0x6BD1D8-0x20, FUNC_PTR_NULL);

//////////////////////////////////////////////////////////////////////////
// UIWidget.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_INTERFACE_UI_WIDGET
	FUNC_PTR(UI_WIDGET_LOAD_BY_NAME_OR_TAG,		0x49AA00, FUNC_PTR_NULL);

#else
	#error Undefined engine layout include for: __EL_INCLUDE_INTERFACE
#endif