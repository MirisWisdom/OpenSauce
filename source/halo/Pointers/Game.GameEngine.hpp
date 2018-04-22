/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Pointers/1.10/HaloCE_110_Runtime.Game.inl"
#include "Pointers/1.10/HaloCE_110_Runtime_Manual.Game.inl"
#else
#include "Pointers/1.10/HaloCE_110_Dedi.Game.inl"
#include "Pointers/1.10/HaloCE_110_Dedi_Manual.Game.inl"
#endif

#include <blam/game/game_engine.hpp>
#include <Memory/MemoryInterface.hpp>
#include <yelo/memory/memory_interface_base.hpp>

namespace Yelo
{
	namespace GameEngine
	{
		ENGINE_PTR(s_ctf_globals, ctf_globals, DUO_PTR(K_CTF_GLOBALS));
		ENGINE_PTR(s_slayer_globals, slayer_globals, DUO_PTR(K_SLAYER_GLOBALS));
		ENGINE_PTR(s_oddball_globals, oddball_globals, DUO_PTR(K_ODDBALL_GLOBALS));
		ENGINE_PTR(s_king_globals, king_globals, DUO_PTR(K_KING_GLOBALS));
		ENGINE_PTR(s_race_globals, race_globals, DUO_PTR(K_RACE_GLOBALS));

		ENGINE_PTR(s_game_variant, global_variant, DUO_PTR(K_GLOBAL_VARIANT));

		ENGINE_DPTR(game_engine_definition, game_engines, DUO_PTR(K_GAME_ENGINES));
		ENGINE_DPTR(game_engine_definition, game_engine, DUO_PTR(K_GAME_ENGINE));

		#pragma region New Engines functions
		//////////////////////////////////////////////////////////////////////////
		// FixHudRenderUnitInterface()
		FUNC_PTR(HUD_RENDER_UNIT_INTERFACE_JMP_TABLE, K_HUD_RENDER_UNIT_INTERFACE_JMP_TABLE, FUNC_PTR_NULL);
		ENGINE_PTR(void**, hud_render_unit_interface_jmp_ptr, K_HUD_RENDER_UNIT_INTERFACE_JMP_PTR, PTR_NULL);

		//////////////////////////////////////////////////////////////////////////
		// FixFunc4F7440()
		FUNC_PTR(Func4F7440JmpTable, DUO_PTR(K_FUNC4F7440JMPTABLE));
		ENGINE_PTR(void**, Func4F7440JmpPtr, DUO_PTR(K_FUNC4F7440JMPPTR));

		//////////////////////////////////////////////////////////////////////////
		// FixFunc4F7580()
		FUNC_PTR(Func4F7580JmpTable, DUO_PTR(K_FUNC4F7580JMPTABLE));
		ENGINE_PTR(void**, Func4F7580JmpPtr, DUO_PTR(K_FUNC4F7580JMPPTR));

		//////////////////////////////////////////////////////////////////////////
		// Initialize()
		ENGINE_PTR(game_engine_definition**, game_engine_init_mod, DUO_PTR(K_GAME_ENGINE_INIT_MOD));
		#pragma endregion

		FUNC_PTR(GAME_ENGINE_INITIALIZE_FOR_NEW_MAP_HOOK, DUO_PTR(K_GAME_ENGINE_INITIALIZE_FOR_NEW_MAP_HOOK));
		FUNC_PTR(GAME_ENGINE_UPDATE_HOOK, DUO_PTR(K_GAME_ENGINE_UPDATE_HOOK));
	}
}
