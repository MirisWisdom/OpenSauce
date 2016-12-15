/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/Rasterizer.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_RASTERIZER
#include "Memory/_EngineLayout.inl"

	namespace GameState
	{
		s_rasterizer_cinematic_globals* _RasterizerCinematicGlobals()	DPTR_IMP_GET(_RasterizerCinematicGlobals);
		s_rasterizer_unk_globals* _RasterizerUnkGlobals()				DPTR_IMP_GET(_RasterizerUnkGlobals);
		s_rasterizer_bloom_globals* _RasterizerBloomGlobals()			DPTR_IMP_GET(_RasterizerBloomGlobals);
		s_weather_globals* _WeatherGlobals()							DPTR_IMP_GET(_WeatherGlobals);
	};
};