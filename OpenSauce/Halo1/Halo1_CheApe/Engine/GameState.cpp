/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Engine/GameState.hpp"

#include <blamlib/Halo1/game/game_globals.hpp>
#include <blamlib/Halo1/game/game_globals_structures.hpp>

#include <YeloLib/cseries/random.hpp>
#include <YeloLib/Halo1/shell/shell_windows_command_line.hpp>

#include "Engine/Objects.hpp"
#include "Engine/Units.hpp"
#include "Engine/AI.hpp"

namespace Yelo
{
	namespace GameState
	{
#if PLATFORM_TYPE == PLATFORM_SAPIEN
		FUNC_PTR(UI_WIDGETS_SAFE_TO_LOAD,						PTR_NULL, PTR_NULL, 0x5BC480);
		FUNC_PTR(GAME_INITIALIZE_HOOK,							PTR_NULL, PTR_NULL, 0x6171F0);
		FUNC_PTR(GAME_INITIALIZE_FOR_NEW_MAP_HOOK,				PTR_NULL, PTR_NULL, 0x618337);
		FUNC_PTR(GAME_DISPOSE_FROM_OLD_MAP_HOOK,				PTR_NULL, PTR_NULL, 0x617419);

		void PLATFORM_API InitializeForNewGameState()
		{
			AI::InitializeForNewGameState();
		}

		void PLATFORM_API InitializeForNewMap()
		{
			Random::InitializeSeed(GameGlobals()->options.game_random_seed);

			Objects::Units::InitializeForNewMap();
			AI::InitializeForNewMap();
		}

		void PLATFORM_API DisposeFromOldMap()
		{
			Objects::Units::DisposeFromOldMap();
			AI::DisposeFromOldMap();
		}
		
		API_FUNC_NAKED static void PLATFORM_API InitializeForNewGameStateHook()
		{
			__asm {
				call		GameState::InitializeForNewGameState
				retn
			}
		}

		API_FUNC_NAKED void InitializeForNewMapHook()
		{
			static uintptr_t CALL_ADDRESS = GET_FUNC_PTR(UI_WIDGETS_SAFE_TO_LOAD);

			_asm
			{
				call	CALL_ADDRESS
				pop		ecx
				call	InitializeForNewMap
				retn
			}
		}

		API_FUNC_NAKED void DisposeFromOldMapHook()
		{
			_asm
			{
				mov     byte ptr [eax+1], 0
				call	DisposeFromOldMap
				retn
			}
		}
#endif

		static bool g_yelo_game_state_enabled;
		bool YeloGameStateEnabled()
		{
			return g_yelo_game_state_enabled;
		}

		void Initialize()
		{
#if PLATFORM_TYPE == PLATFORM_SAPIEN
			g_yelo_game_state_enabled = !CMDLINE_GET_PARAM(no_os_gamestate).ParameterSet();

			Memory::WriteRelativeJmp(&InitializeForNewGameStateHook, GET_FUNC_VPTR(GAME_INITIALIZE_HOOK), true);
			Memory::WriteRelativeJmp(&InitializeForNewMapHook, GET_FUNC_VPTR(GAME_INITIALIZE_FOR_NEW_MAP_HOOK), true);
			Memory::WriteRelativeJmp(&DisposeFromOldMapHook, GET_FUNC_VPTR(GAME_DISPOSE_FROM_OLD_MAP_HOOK), true);
#endif

			AI::Initialize();
		}

		void Dispose()
		{
			AI::Dispose();
		}
	};
};