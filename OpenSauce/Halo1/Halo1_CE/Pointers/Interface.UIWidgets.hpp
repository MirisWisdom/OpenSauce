/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Pointers/1.10/HaloCE_110_Runtime.Interface.inl"
#include "Pointers/1.10/HaloCE_110_Runtime_Manual.Interface.inl"
#endif

#include <YeloLib/memory/memory_interface_base.hpp>
#include <Memory/MemoryInterface.hpp>

namespace Yelo
{
	namespace UIWidgets
	{
		FUNC_PTR(UI_WIDGET_LOAD_BY_NAME_OR_TAG, K_UI_WIDGET_LOAD_BY_NAME_OR_TAG, FUNC_PTR_NULL);
	}
}
