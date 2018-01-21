/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Memory/1.10/Pointers/HaloCE_110_Runtime.Game.inl"
#include "Memory/1.10/Pointers/HaloCE_110_Runtime_Manual.Game.inl"
#else
#include "Memory/1.10/Pointers/HaloCE_110_Dedi.Game.inl"
#include "Memory/1.10/Pointers/HaloCE_110_Dedi_Manual.Game.inl"
#endif

#include <Game/Camera.hpp>
#include <blamlib/camera/director_structures.hpp>
#include <Memory/MemoryInterface.hpp>
#include <yelolib/memory/memory_interface_base.hpp>

namespace Yelo
{
	namespace Camera
	{
		ENGINE_PTR(s_observer, observers, K_OBSERVERS, PTR_NULL);
		ENGINE_PTR(s_camera_script_globals_data, camera_script_globals, K_CAMERA_SCRIPT_GLOBALS, PTR_NULL);
		ENGINE_DPTR(s_director_scripting_data, director_scripting, K_DIRECTOR_SCRIPTING, PTR_NULL);
		ENGINE_DPTR(s_cinematic_globals_data, cinematic_globals, K_CINEMATIC_GLOBALS, PTR_NULL);
		ENGINE_PTR(s_director_data, global_director, K_GLOBAL_DIRECTOR, PTR_NULL);

		FUNC_PTR(DIRECTOR_CHOOSE_GAME_PERSPECTIVE_HOOK, K_DIRECTOR_CHOOSE_GAME_PERSPECTIVE_HOOK, PTR_NULL);
	};
}
