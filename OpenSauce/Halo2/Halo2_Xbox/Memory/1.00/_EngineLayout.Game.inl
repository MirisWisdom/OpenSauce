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
	DATA_PTR(SCRIPTED_CAMERA_UPDATE,	0x16C840);
	DATA_PTR(FIRST_PERSON_CAMERA_UPDATE,0x23C110);
	DATA_PTR(FOLLOWING_CAMERA_UPDATE,	0x23CBB0);
	DATA_PTR(EDITOR_CAMERA_UPDATE,		0x23D090);
	DATA_PTR(DEAD_CAMERA_UPDATE,		0x23DE50);

	FUNC_PTR(PLAYER_CONTROL_UPDATE_HOOK,0x187121);
};

namespace GameState
{
	ENGINE_DPTR(Camera::s_director_globals, _DirectorGlobals,		0x4E8C38);
	ENGINE_DPTR(s_observer, _Observers,								0x4E9BD4);
	ENGINE_DPTR(s_camera_globals, _CameraGlobals,					0x4E8C34);
	ENGINE_DPTR(s_scripted_camera_globals, _ScriptedCameraGlobals,	0x510C6C);
	ENGINE_DPTR(s_observer_globals, _ObserverGlobals,				0x510C70);
	ENGINE_DPTR(s_cinematic_globals, _CinematicGlobals,				0x510C50);
};

namespace Camera
{
	FUNC_PTR(PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_HORIZ_START,	0x18757D);
	FUNC_PTR(PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_HORIZ_END,	0x187585);
	FUNC_PTR(PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_VERT_START,	0x1879FB);
	FUNC_PTR(PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_VERT_END,		0x187A2F);
	FUNC_PTR(PLAYER_CONTROL_MODIFY_DESIRED_ANGLES_VERT_END2,	0x187A03);

	FUNC_PTR(OBSERVER_UPDATE_POSITIONS_START,	0x1706EA);
	FUNC_PTR(OBSERVER_UPDATE_POSITIONS_END,		0x1706F8);
	FUNC_PTR(OBSERVER_UPDATE_POSITIONS_END2,	0x1706F1);

	FUNC_PTR(OBSERVER_UPDATE_VELOCITIES_NULLING,	0x1705A9);
	FUNC_PTR(OBSERVER_UPDATE_POSITIONS_NULLING1,	0x1707CB);
	FUNC_PTR(OBSERVER_UPDATE_POSITIONS_NULLING2,	0x170801);
	FUNC_PTR(DIRECTOR_SET_CAMERA_NULLING,			0x155DEA);

	// for c_yelo_camera::UpdateObserverPositions()
	DATA_PTR(OBSERVER_PARAMETER_REAL_COUNTS, 0x468D30);
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
	ENGINE_DPTR(s_game_engine_globals, _GameEngine, 0x4E9AE8);
};


//////////////////////////////////////////////////////////////////////////
// GameTeams.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_GAME_TEAMS
namespace GameState
{
	ENGINE_DPTR(s_game_allegiance_globals, _GameAllegianceGlobals, 0x4F55EC);
};


//////////////////////////////////////////////////////////////////////////
// Globals.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_GLOBALS
namespace GameState
{
	ENGINE_DPTR(s_header, _Header,									0x4E6094);
	ENGINE_DPTR(s_random_math_globals, _RandomGlobals,				0x4E7408);
	ENGINE_DPTR(s_game_globals, _GameGlobals,						0x4E6948);
	ENGINE_DPTR(s_game_time_globals, _GameTimeGlobals,				0x510C54);
	ENGINE_DPTR(t_campaign_level_data, _CampaignLevelHandles,		0x4EE4E8);
	ENGINE_DPTR(t_multiplayer_level_data, _MultiplayerLevelHandles,	0x4EE4E4);

	ENGINE_DPTR(GameGlobals::group, _Globals,	0x4E034C);
	ENGINE_DPTR(Scenario::group, _Scenario,		0x4E0350);
};


//////////////////////////////////////////////////////////////////////////
// Main.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_MAIN

FUNC_PTR(MAIN_HOOK, 0x12B6B0);

FUNC_PTR(MAIN_LOOP, 0x12B450);


//////////////////////////////////////////////////////////////////////////
// Players.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_PLAYERS
namespace Players
{
	FUNC_PTR(PLAYERS_UPDATE_BEFORE_GAME,		0x14D3A0);
	FUNC_PTR(PLAYERS_UPDATE_BEFORE_GAME_HOOK,	0x13801F);
	FUNC_PTR(PLAYERS_UPDATE_AFTER_GAME,			0x14D8B0);
	FUNC_PTR(PLAYERS_UPDATE_AFTER_GAME_HOOK,	0x138078);
};

namespace GameState
{
	ENGINE_DPTR(t_players_data, _Players,							0x4E8C24);
	ENGINE_DPTR(s_player_globals, _PlayerGlobals,					0x4E8C20);
	ENGINE_DPTR(s_player_control_globals, _PlayerControlGlobals,	0x4ED284);
	ENGINE_DPTR(s_player_training_globals, _PlayerTrainingGlobals,	0x51E9C0);
	ENGINE_DPTR(s_player_effect_globals, _PlayerEffectGlobals,		0x510C5C);
};


//////////////////////////////////////////////////////////////////////////
// Rasterizer.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_RASTERIZER
namespace GameState
{
	ENGINE_DPTR(s_rasterizer_cinematic_globals, _RasterizerCinematicGlobals,	0x5093E0);
	ENGINE_DPTR(s_rasterizer_unk_globals, _RasterizerUnkGlobals,				0x50934C);
	ENGINE_DPTR(s_rasterizer_bloom_globals, _RasterizerBloomGlobals,			0x5093E4);
	ENGINE_DPTR(s_weather_globals, _WeatherGlobals,								0x50942C);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_AI
#endif