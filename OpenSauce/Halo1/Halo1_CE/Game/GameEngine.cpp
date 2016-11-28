/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/GameEngine.hpp"

#include <blamlib/Halo1/game/game_engine_ctf.hpp>
#include <blamlib/Halo1/game/game_engine_king.hpp>
#include <blamlib/Halo1/game/game_engine_oddball.hpp>
#include <blamlib/Halo1/game/game_engine_race.hpp>
#include <blamlib/Halo1/game/game_engine_slayer.hpp>
#include <blamlib/Halo1/game/game_globals_structures.hpp>

#include "Memory/MemoryInterface.hpp"
#include "Game/GameState.hpp"

namespace Yelo
{
	namespace GameEngine
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_GAME_ENGINE
#include "Memory/_EngineLayout.inl"

		s_ctf_globals* CtfGlobals()							PTR_IMP_GET2(ctf_globals);
		s_slayer_globals* SlayerGlobals()					PTR_IMP_GET2(slayer_globals);
		s_oddball_globals* OddballGlobals()					PTR_IMP_GET2(oddball_globals);
		s_king_globals* KingGlobals()						PTR_IMP_GET2(king_globals);
		s_race_globals* RaceGlobals()						PTR_IMP_GET2(race_globals);

		s_game_variant* GlobalVariant()						PTR_IMP_GET2(global_variant);

		const game_engine_definition* const* Definitions()	DPTR_IMP_GET2(game_engines);
		const game_engine_definition* Current()				DPTR_IMP_GET(game_engine);

#pragma region New Engines functions
		static game_engine_definition* new_definitions[Enums::_game_engine_custom + 1] = {
// 			nullptr,
// 			nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // old game engines
// 			nullptr, nullptr, nullptr, nullptr, nullptr, // space for new game engines
// 			nullptr, // terminator
		};
		static int32 new_definitions_count = Enums::k_number_of_game_engines;
#define GAME_ENGINE_REAL_COUNT (Enums::k_number_of_game_engines - 2)

		const game_engine_definition* const* NewDefinitions() { return new_definitions; }

		int32 NewDefinitionsCount() { return new_definitions_count; }

		void RegisterNewGameEngine(game_engine_definition* engine)
		{
			if(new_definitions_count < Enums::_game_engine_custom)
				new_definitions[new_definitions_count++] = engine;
		}

		static void FixHudRenderUnitInterface()
		{
#if !PLATFORM_IS_DEDI

			static void* jmp_table[Enums::_game_engine_custom-1];

			// TODO: need to modify pre-jump table code before this works the way we want it to

			// copy the game's jump table into our jmp table
			for (int32 x = 0; x < Enums::k_number_of_game_engines - GAME_ENGINE_REAL_COUNT; x++)
				jmp_table[x] = *(void**)(GET_FUNC_PTR(HUD_RENDER_UNIT_INTERFACE_JMP_TABLE) + (sizeof(void*) * x));

			// just use the slayer variant of the code, it doesn't matter to us.
			for (int32 x = Enums::k_number_of_game_engines-GAME_ENGINE_REAL_COUNT; x < NUMBEROF(jmp_table); x++)
				jmp_table[x] = *(void**)(GET_FUNC_PTR(HUD_RENDER_UNIT_INTERFACE_JMP_TABLE) + (sizeof(void*) * (Enums::_game_engine_slayer - 1)));

			// set the game's jump table address to our's
			GET_PTR(hud_render_unit_interface_jmp_ptr) = jmp_table;
#endif
		}

		static void FixFunc4F7440() // function for the engine vehicle remapper
		{
			static void* jmp_table[Enums::_game_engine_custom-1];

			// TODO: reference jump table code for the changes to make this work the way we want it to

			// copy the game's jump table into our jmp table
			for (int32 x = 0; x < Enums::k_number_of_game_engines - GAME_ENGINE_REAL_COUNT; x++)
				jmp_table[x] = *(void**)(GET_FUNC_PTR(Func4F7440JmpTable) + (sizeof(void*) * x));

			// just use the slayer variant of the code, it doesn't matter to us.
			for (int32 x = Enums::k_number_of_game_engines-GAME_ENGINE_REAL_COUNT; x < NUMBEROF(jmp_table); x++)
				jmp_table[x] = *(void**)(GET_FUNC_PTR(Func4F7440JmpTable) + (sizeof(void*) * (Enums::_game_engine_slayer - 1)));

			// set the game's jump table address to our's
			GET_PTR(Func4F7440JmpPtr) = jmp_table;
		}

		static void FixFunc4F7580() // function for the engine vehicle remapper
		{
			static void* jmp_table[Enums::_game_engine_custom-1];

			// TODO: reference jump table code for the changes to make this work the way we want it to

			// copy the game's jump table into our jmp table
			for (int32 x = 0; x < Enums::k_number_of_game_engines - GAME_ENGINE_REAL_COUNT; x++)
				jmp_table[x] = *(void**)(GET_FUNC_PTR(Func4F7580JmpTable) + (sizeof(void*) * x));

			// just use the slayer variant of the code, it doesn't matter to us.
			for (int32 x = Enums::k_number_of_game_engines-GAME_ENGINE_REAL_COUNT; x < NUMBEROF(jmp_table); x++)
				jmp_table[x] = *(void**)(GET_FUNC_PTR(Func4F7580JmpTable) + (sizeof(void*) * (Enums::_game_engine_slayer - 1)));

			// set the game's jump table address to our's
			GET_PTR(Func4F7580JmpPtr) = jmp_table;
		}
#pragma endregion

		API_FUNC_NAKED static void GameEngineUpdate_Hook()
		{
			API_FUNC_NAKED_START_()
				call	GameEngine::Update
			API_FUNC_NAKED_END_()
		}

		void Initialize()
		{
			Memory::CreateHookRelativeCall(&GameEngine::InitializeForNewMap, GET_FUNC_VPTR(GAME_ENGINE_INITIALIZE_FOR_NEW_MAP_HOOK), Enums::_x86_opcode_ret);
			Memory::WriteRelativeJmp(&GameEngineUpdate_Hook, GET_FUNC_VPTR(GAME_ENGINE_UPDATE_HOOK), true);

#pragma region New Engines implementation
			// copy the game engine pointers into our list
			for(int32 x = 0; x < new_definitions_count; x++)
				new_definitions[x] = GET_DPTR2(game_engines)[x];

			// replace the game's game_engines list with ours
			GET_PTR(game_engine_init_mod) = new_definitions;

			FixHudRenderUnitInterface();
			FixFunc4F7440();
			FixFunc4F7580();
#pragma endregion
		}

		void Dispose()
		{
		}

		void PLATFORM_API InitializeForNewMap()
		{
			if( GameEngine::Current() )
			{
				GameState::WriteEvent(GameState::GameGlobals()->options.map_name);
 				GameState::WriteEvent("", false);
			}
		}

		void PLATFORM_API Update()
		{
			const real k_update_normal = 1.6666668e-2f; // 0.016666668
			const real k_update_double_speed = 3.3333335e-2f; // 0.033333335

			real delta_time = !GameState::GameGlobals()->players_are_double_speed
				? k_update_normal
				: k_update_double_speed;

			GameState::Update(delta_time);

			// NOTE: Place any update code in the GameState's Update function, or the 
			// Update function of the corresponding component (ie, Effect, Players, etc) 
			// Don't mess with the GameEngine Update function unless it's related to the game_engine
		}
	};
};