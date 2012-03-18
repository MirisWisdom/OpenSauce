/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\Halo2_CheApe for specific license information
*/
#pragma once

#include <Common/PlatformShared.hpp>
#define PLATFORM_TARGET		PLATFORM_TARGET_PC
#define PLATFORM_IS_EDITOR	TRUE
#define CHEAPE_PLATFORM		CHEAPE_PLATFORM_HALO2

#if !defined(PLATFORM_ID)
	#if defined(PLATFORM_TYPE_GUERILLA)
		#define PLATFORM_ID PLATFORM_GUERILLA
	#elif defined(PLATFORM_TYPE_TOOL)
		#define PLATFORM_ID PLATFORM_TOOL
	#elif defined(PLATFORM_TYPE_SAPIEN)
		#define PLATFORM_ID PLATFORM_SAPIEN
	#else
		#error PLATFORM_TYPE not defined. Step 1: Bash head on keyboard. Step 2: Repeat. Are you selecting a build config that is just vanilla Debug or Release?
	#endif
#endif

// Calling convention of guerilla\tool\sapien
#define PLATFORM_API __cdecl


// See this header for more "Engine pointer markup system" documentation
#include <Memory/MemoryInterfaceShared.hpp>

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