/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/Main.hpp"

#include "Memory/Runtime.hpp"
#include "Game/Director.hpp"
#include "Game/EngineFunctions.hpp"

namespace XboxLib
{
	void DisplayLastError()
	{
		Yelo::Game::DisplayLastError();
	}
}

static Yelo::Memory::Opcode::s_call_ret UNWIND_MAIN;

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_MAIN
#include "Memory/_EngineLayout.inl"

	void Initialize()
	{
		YELO_MEM_WRITE(OverwriteJmp, &UNWIND_MAIN, GET_FUNC_VPTR(MAIN_HOOK), &Yelo::Update);
	}

	void Dispose()
	{
		YELO_MEM_WRITE(OverwriteJmp, &UNWIND_MAIN, GET_FUNC_VPTR(MAIN_HOOK), CAST_PTR(void*, UNWIND_MAIN.Address));
	}

	void Update()
	{
		#pragma region Beta time limit code
		// TODO: use the game's hud counter to display time left
		// don't forget to disallow game freezing and such
#if defined(PLATFORM_BETA)
		enum
		{
			k_beta_time_limit = 15,
			k_beta_time_multiplier = 1800,
		};
		static uint32 GameTime = 0;

		GameTime++;

		if(GameTime > (k_beta_time_limit * k_beta_time_multiplier))
		{
			__asm {
				push	2
				call	XBOX_EXPORT_TABLE.HalReturnToFirmware
			};
		}
#endif
		#pragma endregion

		#pragma region Goto the main loop
		static proc_update_no_args MainLoop = CAST_PTR(proc_update_no_args, GET_FUNC_PTR(MAIN_LOOP));
		MainLoop();
		#pragma endregion
	}

	namespace Game
	{
		void PrintScreen(cstring str)
		{
			wchar_t wide_str[64];
			int32 length = Std_strlen(str);

			if(length > NUMBEROF(wide_str))
				length = NUMBEROF(wide_str);
			string_to_wstring_lazy(wide_str, length, str);

			GameState::_CinematicGlobals()->InProgress = false;
			Engine::Interface::HudMessagePrint(wide_str);
		}

		void DisplayLastError()
		{
			DWORD error = ApiGetLastError();

			char buffer[64];
			Std_sprintf(buffer, "Error number %X has occurred!", error);

			PrintScreen(" ");
			PrintScreen("Refer to XDK documentation for more information.");
			PrintScreen(buffer);
		}		
	};
};