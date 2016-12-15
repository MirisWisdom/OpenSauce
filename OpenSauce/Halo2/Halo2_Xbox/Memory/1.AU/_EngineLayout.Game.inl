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
	DATA_PTR(SCRIPTED_CAMERA_UPDATE,	NULL);
	DATA_PTR(FIRST_PERSON_CAMERA_UPDATE,NULL);
	DATA_PTR(FOLLOWING_CAMERA_UPDATE,	NULL);
	DATA_PTR(EDITOR_CAMERA_UPDATE,		NULL);
	DATA_PTR(DEAD_CAMERA_UPDATE,		NULL);

	FUNC_PTR(PLAYER_CONTROL_UPDATE_HOOK,NULL);
};

namespace GameState
{
	ENGINE_DPTR(Camera::s_director_globals, _DirectorGlobals,		NULL);
	ENGINE_DPTR(s_observer, _Observers,								NULL);
	ENGINE_DPTR(s_camera_globals, _CameraGlobals,					NULL);
	ENGINE_DPTR(s_scripted_camera_globals, _ScriptedCameraGlobals,	NULL);
	ENGINE_DPTR(s_observer_globals, _ObserverGlobals,				NULL);
	ENGINE_DPTR(s_cinematic_globals, _CinematicGlobals,				NULL);
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
	ENGINE_DPTR(s_game_engine_globals, _GameEngine, NULL);
};


//////////////////////////////////////////////////////////////////////////
// GameTeams.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_GAME_TEAMS
namespace GameState
{
	ENGINE_DPTR(s_game_allegiance_globals, _GameAllegianceGlobals, NULL);
};


//////////////////////////////////////////////////////////////////////////
// Globals.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_GLOBALS
namespace GameState
{
	ENGINE_DPTR(s_header, _Header,									NULL);
	ENGINE_DPTR(s_random_math_globals, _RandomGlobals,				NULL);
	ENGINE_DPTR(s_game_globals, _GameGlobals,						NULL);
	ENGINE_DPTR(s_game_time_globals, _GameTimeGlobals,				NULL);
	ENGINE_DPTR(t_campaign_level_data, _CampaignLevelHandles,		NULL);
	ENGINE_DPTR(t_multiplayer_level_data, _MultiplayerLevelHandles,	NULL);

	ENGINE_DPTR(GameGlobals::group, _Globals,	NULL);
	ENGINE_DPTR(Scenario::group, _Scenario,		NULL);
};


//////////////////////////////////////////////////////////////////////////
// Main.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_MAIN

FUNC_PTR(MAIN_HOOK, NULL);

FUNC_PTR(MAIN_LOOP, NULL);


//////////////////////////////////////////////////////////////////////////
// Players.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_PLAYERS
namespace Players
{
	FUNC_PTR(PLAYERS_UPDATE_BEFORE_GAME,		NULL);
	FUNC_PTR(PLAYERS_UPDATE_BEFORE_GAME_HOOK,	NULL);
	FUNC_PTR(PLAYERS_UPDATE_AFTER_GAME,			NULL);
	FUNC_PTR(PLAYERS_UPDATE_AFTER_GAME_HOOK,	NULL);
};

namespace GameState
{
	ENGINE_DPTR(t_players_data, _Players,							NULL);
	ENGINE_DPTR(s_player_globals, _PlayerGlobals,					NULL);
	ENGINE_DPTR(s_player_control_globals, _PlayerControlGlobals,	NULL);
	ENGINE_DPTR(s_player_training_globals, _PlayerTrainingGlobals,	NULL);
	ENGINE_DPTR(s_player_effect_globals, _PlayerEffectGlobals,		NULL);
};


//////////////////////////////////////////////////////////////////////////
// Rasterizer.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_RASTERIZER
namespace GameState
{
	ENGINE_DPTR(s_rasterizer_cinematic_globals, _RasterizerCinematicGlobals,	NULL);
	ENGINE_DPTR(s_rasterizer_unk_globals, _RasterizerUnkGlobals,				NULL);
	ENGINE_DPTR(s_rasterizer_bloom_globals, _RasterizerBloomGlobals,			NULL);
	ENGINE_DPTR(s_weather_globals, _WeatherGlobals,								NULL);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_AI
#endif