/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Pointers/1.10/HaloCE_110_Runtime.OpenSauce.inl"
#include "Pointers/1.10/HaloCE_110_Runtime_Manual.OpenSauce.inl"
#else
#include "Pointers/1.10/HaloCE_110_Dedi.OpenSauce.inl"
#endif

#include <YeloLib/memory/memory_interface_base.hpp>
#include <Memory/MemoryInterface.hpp>

namespace Yelo
{
	namespace Debug
	{
		FUNC_PTR(WINMAIN_EXCEPTION_FILTER, DUO_PTR(K_WINMAIN_EXCEPTION_FILTER));
		FUNC_PTR(WINMAIN_EXCEPTION_FILTER_CALL, DUO_PTR(K_WINMAIN_EXCEPTION_FILTER_CALL));

		FUNC_PTR(RASTERIZER_DX9_SAVE_GAMMA, K_RASTERIZER_DX9_SAVE_GAMMA, FUNC_PTR_NULL);
		FUNC_PTR(RASTERIZER_WINDOWS_PRESENT_FRAME, K_RASTERIZER_WINDOWS_PRESENT_FRAME, FUNC_PTR_NULL);
		FUNC_PTR(SOUND_STOP_ALL, DUO_PTR(K_SOUND_STOP_ALL));

		ENGINE_PTR(void**, VIRTUALPROTECT_LOCK, DUO_PTR(K_VIRTUALPROTECT_LOCK))
		ENGINE_PTR(DWORD, VIRTUALPROTECT_OLD_PROTECT, DUO_PTR(K_VIRTUALPROTECT_OLD_PROTECT));
	}
}
