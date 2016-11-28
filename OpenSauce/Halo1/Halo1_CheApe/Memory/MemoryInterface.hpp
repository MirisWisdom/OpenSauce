/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

// See this header for more "Engine pointer markup system" documentation
#include <YeloLib/memory/memory_interface_base.hpp>
#include <YeloLib/memory/naked_func_writer.hpp>

//////////////////////////////////////////////////////////////////////////
// Value markup system
//
// [g_value]	: value for use in Guerilla builds
// [t_value]	: value for use in Tool builds
// [s_value]	: value for use in Sapien builds
//
//////////////////////////////////////////////////////////////////////////

#if PLATFORM_TYPE == PLATFORM_GUERILLA
	#define PLATFORM_VALUE(g_value, t_value, s_value) g_value
#elif PLATFORM_TYPE == PLATFORM_TOOL
	#define PLATFORM_VALUE(g_value, t_value, s_value) t_value
#elif PLATFORM_TYPE == PLATFORM_SAPIEN
	#define PLATFORM_VALUE(g_value, t_value, s_value) s_value
#endif

#define PLATFORM_PTR(type, g_value, t_value, s_value) CAST_PTR(type, PLATFORM_VALUE(g_value, t_value, s_value))
