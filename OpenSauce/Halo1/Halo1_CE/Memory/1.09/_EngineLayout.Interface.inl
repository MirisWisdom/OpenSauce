/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// Controls.cpp
#if __EL_INCLUDE_FILE_ID == __EL_INTERFACE_CONTROLS
	FUNC_PTR(INPUT_UPDATE_HOOK,				0x493A16, FUNC_PTR_NULL);

	ENGINE_PTR(ControlSettings, Settings,			0x6AD822, PTR_NULL);
	ENGINE_PTR(ControlStates, ControlState,			0x64C570, PTR_NULL);
	ENGINE_PTR(PositionState, MousePositionState,	0x6B4028, PTR_NULL);
	ENGINE_PTR(byte, InputStateFlags,				0x6AD592, PTR_NULL);

#pragma region GameUI
//////////////////////////////////////////////////////////////////////////
// GameUI.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_INTERFACE_GAME_UI
	namespace GameUI
	{
		ENGINE_DPTR(s_hud_scripted_globals, hud_scripted_globals,	0x6B44C8, PTR_NULL);
		ENGINE_DPTR(s_hud_messaging, hud_messaging,					0x64E990, PTR_NULL);
		ENGINE_DPTR(s_hud_unit_interface, hud_unit_interface,		0x6B44DC, PTR_NULL);
		ENGINE_DPTR(s_hud_weapon_interface, hud_weapon_interface,	0x6B44E0, PTR_NULL);
		ENGINE_DPTR(s_hud_nav_points, hud_nav_points,				0x64E994, PTR_NULL);
		ENGINE_DPTR(s_motion_sensor, motion_sensor,					0x6B44E8, PTR_NULL);
		ENGINE_DPTR(s_first_person_weapons, first_person_weapons,	0x64DCE8, PTR_NULL);

		ENGINE_PTR(s_text_bounds_globals_data, text_bounds_globals,	0x67F6F4, PTR_NULL);
		ENGINE_PTR(s_font_drawing_globals_data, font_drawing_globals,0x67F708,PTR_NULL);

		ENGINE_PTR(s_hud_chat_globals, hud_chat_globals,			0x64E7A8, PTR_NULL);
		ENGINE_PTR(int32, hud_chat_line_count,						0x6B44D4, PTR_NULL);

		FUNC_PTR(HUD_UPDATE,										0x4ACBA0, FUNC_PTR_NULL);
		FUNC_PTR(HUD_UPDATE_CALL,									0x45C3B2, FUNC_PTR_NULL);
		FUNC_PTR(FIRST_PERSON_WEAPONS_UPDATE,						0x495350, FUNC_PTR_NULL);
		FUNC_PTR(FIRST_PERSON_WEAPONS_UPDATE_CALL,					0x45C38E, FUNC_PTR_NULL);

		FUNC_PTR(FIRST_PERSON_WEAPON_SET_STATE_HOOK,				0x495FE9, FUNC_PTR_NULL);
	};

	namespace Hud
	{
		#pragma region ShowCrosshair
		ENGINE_PTR(uint16, RENDER_CROSSHAIRS_DISABLE_MOD,			0x4B62E1, PTR_NULL);
		#pragma endregion

		#pragma region Scale
		FUNC_PTR(RENDER_WIDGET_RECURSIVE,							0x49DA20, PTR_NULL);
		static void** K_RENDER_WIDGET_RECURSIVE_CALLS[] =
		{
			CAST_PTR(void**, 0x49B5FA), CAST_PTR(void**, 0x49B6D6), CAST_PTR(void**, 0x49DD1E),
			CAST_PTR(void**, 0x49E759), CAST_PTR(void**, 0x49EC51), CAST_PTR(void**, 0x49EC8F)
		};

		static void* K_RASTERIZER_RENDER_UI_SET_SCREENPROJ_CALLS[] =
		{ CAST_PTR(void*, 0x520885), CAST_PTR(void*, 0x535057), CAST_PTR(void*, 0x52F829) };
		#pragma endregion

		#pragma region render nav points
		FUNC_PTR(HUD_RENDER_NAV_POINTS,								0x4B3190, PTR_NULL);
		FUNC_PTR(HUD_RENDER_NAV_POINTS_CALL,						0x4ACD0D, PTR_NULL);

		ENGINE_PTR(uint32, HUD_RENDER_NAV_POINT_ANCHOR_HALF_X,		0x4B2D10+1, PTR_NULL);
		ENGINE_PTR(uint32, HUD_RENDER_NAV_POINT_ANCHOR_HALF_Y,		0x4B2D19+2, PTR_NULL);

		ENGINE_PTR(real, HUD_ANCHOR_SCREEN_X,						0x6122E0, PTR_NULL);
		ENGINE_PTR(real, HUD_ANCHOR_SCREEN_Y,						0x6122DC, PTR_NULL);
		ENGINE_PTR(real, HUD_ANCHOR_SCREEN_HALF_X,					0x612954, PTR_NULL);
		ENGINE_PTR(real, HUD_ANCHOR_SCREEN_HALF_Y,					0x612958, PTR_NULL);
		#pragma endregion

		#pragma region render team indicator
		FUNC_PTR(HUD_RENDER_PLAYERS,								0x4ADA60, PTR_NULL);
		FUNC_PTR(HUD_RENDER_PLAYERS_CALL,							0x4ACC8E, PTR_NULL);
		#pragma endregion

		#pragma region render scoreboard
		FUNC_PTR(HUD_RENDER_SCOREBOARD_INGAME,						0x45FE60, PTR_NULL);
		FUNC_PTR(HUD_RENDER_SCOREBOARD_INGAME_CALL,					0x497A62, PTR_NULL);
		FUNC_PTR(HUD_RENDER_SCOREBOARD_POSTGAME,					0x45E2A0, PTR_NULL);
		FUNC_PTR(HUD_RENDER_SCOREBOARD_POSTGAME_CALL,				0x497A59, PTR_NULL);
		#pragma endregion

		#pragma region hud
		ENGINE_PTR(uint32, HUD_POINT_ANCHOR_WIDTH,					0x4AED1F+1, PTR_NULL);
		ENGINE_PTR(uint32, HUD_POINT_ANCHOR_HEIGHT,					0x4AED51+1, PTR_NULL);
		ENGINE_PTR(uint32, HUD_POINT_ANCHOR_HALF_WIDTH,				0x4AED71+1, PTR_NULL);
		ENGINE_PTR(uint32, HUD_POINT_ANCHOR_HALF_HEIGHT,			0x4AED95+1, PTR_NULL);

		ENGINE_PTR(uint32, HUD_POINT_DAMAGE_ANCHOR_WIDTH,			0x4B4B94+1, PTR_NULL);
		ENGINE_PTR(uint32, HUD_POINT_DAMAGE_ANCHOR_HEIGHT,			0x4B4B6D+1, PTR_NULL);
		ENGINE_PTR(real, HUD_POINT_DAMAGE_ANCHOR_HALF_HEIGHT,		0x4B4BAB+4, PTR_NULL);

		static void** K_HUD_POINT_DAMAGE_ANCHOR_HALF_WIDTH_PTRS[] =
		{
			CAST_PTR(void**, 0x4B4B88+2), CAST_PTR(void**, 0x4B4B48+2)
		};
		#pragma endregion

		#pragma region multiplayer icons
		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_CTF_X,				0x4B4874, PTR_NULL);
		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_CTF_Y,				0x4B487B, PTR_NULL);
		
		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_KING_X,				0x4B48A6, PTR_NULL);

		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_ODDBALL_X,			0x4B48D1, PTR_NULL);
		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_ODDBALL_Y,			0x4B48D8, PTR_NULL);

		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_RACE_X,				0x4B4900, PTR_NULL);
		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_RACE_Y,				0x4B4907, PTR_NULL);

		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_SLAYER_X,			0x4B492F, PTR_NULL);

		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_BACKGROUND_X,		0x4B49DA, PTR_NULL);
		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_BACKGROUND_Y,		0x4B49E1, PTR_NULL);
		#pragma endregion

		ENGINE_DPTR(void, hud_globals,								0x6B44C4, PTR_NULL);
	};
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Keystone.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_INTERFACE_KEYSTONE
	FUNC_PTR(KEYSTONE_CHATLOG_ADD_ITEM_SEND_MESSAGE, 0x4AEAD0, FUNC_PTR_NULL);

	ENGINE_PTR(wchar_t, ChatLogName,		0x637D64, PTR_NULL);
	ENGINE_PTR(void**, MainWindow,			0x6BD194, PTR_NULL);

	FUNC_PTR(KS_TRANSLATEACCELERATOR_CALL,	0x5453F2, FUNC_PTR_NULL);
	FUNC_PTR(KS_TRANSLATEACCELERATOR,		0x6BD1A0, FUNC_PTR_NULL);
	FUNC_PTR(KS_GETWINDOW,					0x6BD1A8, FUNC_PTR_NULL);
	FUNC_PTR(KW_RELEASE,					0x6BD1B8, FUNC_PTR_NULL);
	FUNC_PTR(KW_GETCONTROLBYID,				0x6BD1BC, FUNC_PTR_NULL);
	FUNC_PTR(KC_SENDMESSAGE,				0x6BD1D8, FUNC_PTR_NULL);

//////////////////////////////////////////////////////////////////////////
// UIWidget.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_INTERFACE_UI_WIDGET
	FUNC_PTR(UI_WIDGET_LOAD_BY_NAME_OR_TAG,		0x49ABD0, FUNC_PTR_NULL);

#else
	#error Undefined engine layout include for: __EL_INCLUDE_INTERFACE
#endif