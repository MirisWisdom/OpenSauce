/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/GameEngine.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_GAME_ENGINE
#include "Memory/_EngineLayout.inl"

	namespace GameState
	{
		s_game_engine_globals* _GameEngine()	DPTR_IMP_GET(_GameEngine);
	};
};