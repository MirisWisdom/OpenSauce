/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// Director.cpp
#if __EL_INCLUDE_FILE_ID == __EL_GAME_DIRECTOR
namespace Camera
{
	DATA_PTR(SCRIPTED_CAMERA_UPDATE,	0x0FC330);
	DATA_PTR(FIRST_PERSON_CAMERA_UPDATE,0x1D2C70);
	DATA_PTR(FOLLOWING_CAMERA_UPDATE,	0x1D3B10);
	DATA_PTR(EDITOR_CAMERA_UPDATE,		0x1D4060);
	DATA_PTR(DEAD_CAMERA_UPDATE,		0x1D4F40);

	FUNC_PTR(PLAYER_CONTROL_UPDATE_HOOK,0x187121, NULL, 0x103E35);
};

namespace GameState
{
	ENGINE_DPTR(Camera::s_director_globals, _DirectorGlobals,		0x65DC58);
	ENGINE_DPTR(s_observer, _Observers,								0x6BA340);
	ENGINE_DPTR(s_camera_globals, _CameraGlobals,					0x65DC54);
	ENGINE_DPTR(s_scripted_camera_globals, _ScriptedCameraGlobals,	NULL);
	ENGINE_DPTR(s_observer_globals, _ObserverGlobals,				0x6D9E68);
	ENGINE_DPTR(s_cinematic_globals, _CinematicGlobals,				0x6D9E20);
};

namespace Camera
{
	FUNC_PTR(PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_HORIZ_START,	NULL);
	FUNC_PTR(PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_HORIZ_END,	NULL);
	FUNC_PTR(PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_VERT_START,	NULL);
	FUNC_PTR(PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_VERT_END,		NULL);
	FUNC_PTR(PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_VERT_END2,	NULL);

	FUNC_PTR(OBSERVER_UPDATE_POSITIONS_START,	NULL);
	FUNC_PTR(OBSERVER_UPDATE_POSITIONS_END,		NULL);
	FUNC_PTR(OBSERVER_UPDATE_POSITIONS_END2,	NULL);

	FUNC_PTR(OBSERVER_UPDATE_VELOCITIES_NULLING,	NULL);
	FUNC_PTR(OBSERVER_UPDATE_POSITIONS_NULLING1,	NULL);
	FUNC_PTR(OBSERVER_UPDATE_POSITIONS_NULLING2,	NULL);
	FUNC_PTR(DIRECTOR_SET_CAMERA_NULLING,			NULL);

	// for c_yelo_camera::UpdateObserverPositions()
	DATA_PTR(OBSERVER_PARAMETER_REAL_COUNTS,		NULL);
};


//////////////////////////////////////////////////////////////////////////
// EngineFunctions.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_ENGINE_FUNCTIONS
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.Game.EngineFunctions.inl)


//////////////////////////////////////////////////////////////////////////
// GameEngine.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_GAME_ENGINE
namespace GameState
{
	ENGINE_DPTR(s_game_engine_globals, _GameEngine,		0x65E134);
};


//////////////////////////////////////////////////////////////////////////
// GameTeams.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_GAME_TEAMS
namespace GameState
{
	ENGINE_DPTR(s_game_allegiance_globals, _GameAllegianceGlobals,	0x6B0D3C);
};


//////////////////////////////////////////////////////////////////////////
// Globals.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_GLOBALS
namespace GameState
{
	ENGINE_DPTR(s_header, _Header,									0x6F81E0);
	ENGINE_DPTR(s_random_math_globals, _RandomGlobals,				0x65C1DC);
	ENGINE_DPTR(s_game_globals, _GameGlobals,						0x65DC50);
	ENGINE_DPTR(s_game_time_globals, _GameTimeGlobals,				0x6D9E30);
	ENGINE_DPTR(t_campaign_level_data, _CampaignLevelHandles,		0x6C5B6C);
	ENGINE_DPTR(t_multiplayer_level_data, _MultiplayerLevelHandles,	0x6C5B68);

	ENGINE_DPTR(GameGlobals::group, _Globals,	0x65C158);
	ENGINE_DPTR(Scenario::group, _Scenario,		0x65C16C);
};


//////////////////////////////////////////////////////////////////////////
// Main.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_MAIN

FUNC_PTR(MAIN_HOOK,	0xBCCC0);

FUNC_PTR(MAIN_LOOP,	0xBC9D0);


//////////////////////////////////////////////////////////////////////////
// Players.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_PLAYERS
namespace Players
{
	FUNC_PTR(PLAYERS_UPDATE_BEFORE_GAME,		0xDD0A0);
	FUNC_PTR(PLAYERS_UPDATE_BEFORE_GAME_HOOK,	0xE31C0);
	FUNC_PTR(PLAYERS_UPDATE_AFTER_GAME,			0xDD3B0);
	FUNC_PTR(PLAYERS_UPDATE_AFTER_GAME_HOOK,	0xE3201);
};

namespace GameState
{
	ENGINE_DPTR(t_players_data, _Players,							0x65DC4C);
	ENGINE_DPTR(s_player_globals, _PlayerGlobals,					0x65DC48);
	ENGINE_DPTR(s_player_control_globals, _PlayerControlGlobals,	0x65E364);
	ENGINE_DPTR(s_player_training_globals, _PlayerTrainingGlobals,	NULL);
	ENGINE_DPTR(s_player_effect_globals, _PlayerEffectGlobals,		0x6D9E28);
};


//////////////////////////////////////////////////////////////////////////
// Rasterizer.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_RASTERIZER
namespace GameState
{
	ENGINE_DPTR(s_rasterizer_cinematic_globals, _RasterizerCinematicGlobals,	0x6D93DC);
	ENGINE_DPTR(s_rasterizer_unk_globals, _RasterizerUnkGlobals,				NULL);
	ENGINE_DPTR(s_rasterizer_bloom_globals, _RasterizerBloomGlobals,			NULL);
	ENGINE_DPTR(s_weather_globals, _WeatherGlobals,								NULL);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_AI
#endif