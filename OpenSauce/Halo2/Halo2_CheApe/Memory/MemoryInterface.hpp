/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\Halo2_CheApe for specific license information
*/
#pragma once

// See this header for more "Engine pointer markup system" documentation
#include <YeloLib/memory/memory_interface_base.hpp>

//////////////////////////////////////////////////////////////////////////
// Engine pointer markup system
//
// [g_addr]	: H2Guerilla memory address
// [t_addr]	: H2Tool memory address
// [s_addr]	: H2Sapien memory address
//
//////////////////////////////////////////////////////////////////////////

#if PLATFORM_ID == PLATFORM_GUERILLA
	#define FUNC_PTR(name, g_addr, t_addr, s_addr) enum FUNC_PTR_##name { PTR_##name = g_addr };	BOOST_STATIC_ASSERT( PTR_##name != NULL );
	#define PLATFORM_VALUE(g_value, t_value, s_value) g_value
#elif PLATFORM_ID == PLATFORM_TOOL
	#define FUNC_PTR(name, g_addr, t_addr, s_addr) enum FUNC_PTR_##name { PTR_##name = t_addr };	BOOST_STATIC_ASSERT( PTR_##name != NULL );
	#define PLATFORM_VALUE(g_value, t_value, s_value) t_value
#elif PLATFORM_ID == PLATFORM_SAPIEN
	#define FUNC_PTR(name, g_addr, t_addr, s_addr) enum FUNC_PTR_##name { PTR_##name = s_addr };	BOOST_STATIC_ASSERT( PTR_##name != NULL );
	#define PLATFORM_VALUE(g_value, t_value, s_value) s_value
#endif