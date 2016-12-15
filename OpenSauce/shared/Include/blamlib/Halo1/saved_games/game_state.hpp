/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/saved_games/saved_game_constants.hpp>

namespace Yelo
{
	namespace GameState
	{
		struct s_header_data;
		struct s_game_state_globals;

		s_game_state_globals* GameStateGlobals();
	};

	namespace blam
	{
		void* PLATFORM_API game_state_malloc(cstring name, cstring type, size_t size);
	};
};