/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// Controls.cpp
#if __EL_INCLUDE_FILE_ID == __EL_INTERFACE_CONTROLS
	FUNC_PTR(INPUT_UPDATE_HOOK,				0x493A06, FUNC_PTR_NULL);

	ENGINE_PTR(ControlSettings, Settings,			0x6AD73A, PTR_NULL);
	ENGINE_PTR(ControlStates, ControlState,			0x64C488, PTR_NULL);
	ENGINE_PTR(PositionState, MousePositionState,	0x6B3F40, PTR_NULL);
	ENGINE_PTR(byte, InputStateFlags,				0x6AD4AA, PTR_NULL);

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

		ENGINE_PTR(s_hud_chat_globals, hud_chat_globals,			0x64E6C0, 0x5C04F0);
		ENGINE_PTR(int32, hud_chat_line_count,						0x6B43EC, PTR_NULL);

		FUNC_PTR(HUD_UPDATE,										0x4ACB70, FUNC_PTR_NULL);
		FUNC_PTR(HUD_UPDATE_CALL,									0x45C3A2, FUNC_PTR_NULL);
		FUNC_PTR(FIRST_PERSON_WEAPONS_UPDATE,						0x495340, FUNC_PTR_NULL);
		FUNC_PTR(FIRST_PERSON_WEAPONS_UPDATE_CALL,					0x45C37E, FUNC_PTR_NULL);

		FUNC_PTR(FIRST_PERSON_WEAPON_SET_STATE_HOOK,				0x495FD9, FUNC_PTR_NULL);
	};

	namespace Hud
	{
		#pragma region ShowCrosshair
		FUNC_PTR(RENDER_CROSSHAIRS_DISABLE_MOD,							0x4B62B1, FUNC_PTR_NULL);
		#pragma endregion

		#pragma region Scale
		FUNC_PTR(RENDER_WIDGET_RECURSIVE,								0x49D9F0, PTR_NULL);
		static void** K_RENDER_WIDGET_RECURSIVE_CALLS[] =
		{
			CAST_PTR(void**, 0x49B5CA), CAST_PTR(void**, 0x49B6A6), CAST_PTR(void**, 0x49DCEE),
			CAST_PTR(void**, 0x49E729), CAST_PTR(void**, 0x49EC21), CAST_PTR(void**, 0x49EC5F)
		};

		static void* K_RASTERIZER_RENDER_UI_SET_SCREENPROJ_CALLS[] =
		{ CAST_PTR(void*, 0x520855), CAST_PTR(void*, 0x535027), CAST_PTR(void*, 0x52F7F9) };
		#pragma endregion

		#pragma region render nav points
		FUNC_PTR(HUD_RENDER_NAV_POINTS,								0x4B3160, PTR_NULL);
		FUNC_PTR(HUD_RENDER_NAV_POINTS_CALL,						0x4ACCDD, PTR_NULL);

		ENGINE_PTR(uint32, HUD_RENDER_NAV_POINT_ANCHOR_HALF_X,		0x4B2CE0+1, PTR_NULL);
		ENGINE_PTR(uint32, HUD_RENDER_NAV_POINT_ANCHOR_HALF_Y,		0x4B2CE9+2, PTR_NULL);

		ENGINE_PTR(real, HUD_ANCHOR_SCREEN_X,						0x612300, PTR_NULL);
		ENGINE_PTR(real, HUD_ANCHOR_SCREEN_Y,						0x6122FC, PTR_NULL);
		ENGINE_PTR(real, HUD_ANCHOR_SCREEN_HALF_X,					0x612974, PTR_NULL);
		ENGINE_PTR(real, HUD_ANCHOR_SCREEN_HALF_Y,					0x612978, PTR_NULL);
		#pragma endregion

		#pragma region render team indicator
		FUNC_PTR(HUD_RENDER_PLAYERS,								0x4ADA30, PTR_NULL);
		FUNC_PTR(HUD_RENDER_PLAYERS_CALL,							0x4ACC5E, PTR_NULL);
		#pragma endregion

		#pragma region render scoreboard
		FUNC_PTR(HUD_RENDER_SCOREBOARD_INGAME,						0x45FE50, PTR_NULL);
		FUNC_PTR(HUD_RENDER_SCOREBOARD_INGAME_CALL,					0x497A52, PTR_NULL);
		FUNC_PTR(HUD_RENDER_SCOREBOARD_POSTGAME,					0x45E290, PTR_NULL);
		FUNC_PTR(HUD_RENDER_SCOREBOARD_POSTGAME_CALL,				0x497A49, PTR_NULL);
		#pragma endregion

		#pragma region hud
		ENGINE_PTR(uint32, HUD_POINT_ANCHOR_WIDTH,					0x4AECEF+1, PTR_NULL);
		ENGINE_PTR(uint32, HUD_POINT_ANCHOR_HEIGHT,					0x4AED21+1, PTR_NULL);
		ENGINE_PTR(uint32, HUD_POINT_ANCHOR_HALF_WIDTH,				0x4AED41+1, PTR_NULL);
		ENGINE_PTR(uint32, HUD_POINT_ANCHOR_HALF_HEIGHT,			0x4AED65+1, PTR_NULL);

		ENGINE_PTR(uint32, HUD_POINT_DAMAGE_ANCHOR_WIDTH,			0x4B4B64+1, PTR_NULL);
		ENGINE_PTR(uint32, HUD_POINT_DAMAGE_ANCHOR_HEIGHT,			0x4B4B3D+1, PTR_NULL);
		ENGINE_PTR(real, HUD_POINT_DAMAGE_ANCHOR_HALF_HEIGHT,		0x4B4B7B+4, PTR_NULL);

		static void** K_HUD_POINT_DAMAGE_ANCHOR_HALF_WIDTH_PTRS[] =
		{
			CAST_PTR(void**, 0x4B4B58+2), CAST_PTR(void**, 0x4B4B18+2)
		};
		#pragma endregion

		#pragma region multiplayer icons
		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_CTF_X,				0x4B4844, PTR_NULL);
		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_CTF_Y,				0x4B484B, PTR_NULL);
		
		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_KING_X,				0x4B4876, PTR_NULL);

		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_ODDBALL_X,			0x4B48A1, PTR_NULL);
		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_ODDBALL_Y,			0x4B48A8, PTR_NULL);

		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_RACE_X,				0x4B48D0, PTR_NULL);
		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_RACE_Y,				0x4B48D7, PTR_NULL);

		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_SLAYER_X,			0x4B48FF, PTR_NULL);

		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_BACKGROUND_X,		0x4B49AA, PTR_NULL);
		ENGINE_PTR(uint16, HUD_MULTIPLAYER_ICON_BACKGROUND_Y,		0x4B49B1, PTR_NULL);
		#pragma endregion

		ENGINE_DPTR(void, hud_globals,								0x6B43DC, PTR_NULL);
	};
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Keystone.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_INTERFACE_KEYSTONE
	FUNC_PTR(KEYSTONE_CHATLOG_ADD_ITEM_SEND_MESSAGE, 0x4AEAA0, FUNC_PTR_NULL);

	ENGINE_PTR(wchar_t, ChatLogName,		0x637D64, PTR_NULL);
	ENGINE_PTR(void**, MainWindow,			0x6BD0AC, PTR_NULL);

	FUNC_PTR(KS_TRANSLATEACCELERATOR_CALL,	0x5453C2, FUNC_PTR_NULL);
	FUNC_PTR(KS_TRANSLATEACCELERATOR,		0x6BD0B8, FUNC_PTR_NULL);
	FUNC_PTR(KS_GETWINDOW,					0x6BD0C0, FUNC_PTR_NULL);
	FUNC_PTR(KW_RELEASE,					0x6BD0D0, FUNC_PTR_NULL);
	FUNC_PTR(KW_GETCONTROLBYID,				0x6BD0D4, FUNC_PTR_NULL);
	FUNC_PTR(KC_SENDMESSAGE,				0x6BD0F0, FUNC_PTR_NULL);

//////////////////////////////////////////////////////////////////////////
// UIWidget.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_INTERFACE_UI_WIDGET
	FUNC_PTR(UI_WIDGET_LOAD_BY_NAME_OR_TAG,		0x49ABA0, FUNC_PTR_NULL);

#else
	#error Undefined engine layout include for: __EL_INCLUDE_INTERFACE
#endif