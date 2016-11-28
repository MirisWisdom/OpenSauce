/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/Globals.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_GLOBALS
#include "Memory/_EngineLayout.inl"

	namespace GameState
	{
		s_header* _Header()									DPTR_IMP_GET(_Header);
		s_random_math_globals* _RandomGlobals()				DPTR_IMP_GET(_RandomGlobals);
		s_game_globals* _GameGlobals()						DPTR_IMP_GET(_GameGlobals);
		s_game_time_globals* _GameTimeGlobals()				DPTR_IMP_GET(_GameTimeGlobals);
		t_campaign_level_data* _CampaignLevelHandles()		DPTR_IMP_GET(_CampaignLevelHandles);
		t_multiplayer_level_data* _MultiplayerLevelHandles()DPTR_IMP_GET(_MultiplayerLevelHandles);


		GameGlobals::group* _Globals()						DPTR_IMP_GET(_Globals);
		Scenario::group* _Scenario()						DPTR_IMP_GET(_Scenario);
	};
};