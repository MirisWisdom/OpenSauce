/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/GameTeams.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_GAME_TEAMS
#include "Memory/_EngineLayout.inl"

	namespace GameState
	{
		s_game_allegiance_globals* _GameAllegianceGlobals()	DPTR_IMP_GET(_GameAllegianceGlobals);
	};
};