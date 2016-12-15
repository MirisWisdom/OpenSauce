/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/memory/function_interface.hpp>

namespace Yelo
{
	namespace Memory
	{
#if !PLATFORM_IS_DEDI
		bool& FunctionProcessRenderHudIsDisabled();
		bool& FunctionProcessUpdateUIWidgetsDisabled();
		bool& FunctionProcessRenderCursorDisabled();
#endif
	};
};