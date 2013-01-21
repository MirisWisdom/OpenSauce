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
// [g_value/addr]	: value for use in Guerilla builds
// [t_valueaddr]	: value for use in Tool builds
// [s_valueaddr]	: value for use in Sapien builds
//
//////////////////////////////////////////////////////////////////////////

#if PLATFORM_ID == PLATFORM_GUERILLA
	#define PLATFORM_VALUE(g_value, t_value, s_value) g_value
#elif PLATFORM_ID == PLATFORM_TOOL
	#define PLATFORM_VALUE(g_value, t_value, s_value) t_value
#elif PLATFORM_ID == PLATFORM_SAPIEN
	#define PLATFORM_VALUE(g_value, t_value, s_value) s_value
#endif

#define ENGINE_DPTR(type, name, g_addr, t_addr, s_addr) \
	static type##** const pp##name = CAST_PTR(type##**, PLATFORM_VALUE(g_addr,t_addr,s_addr));								BOOST_STATIC_ASSERT( PLATFORM_VALUE(g_addr,t_addr,s_addr) != NULL );

#define ENGINE_PTR(type, name, g_addr, t_addr, s_addr) \
	static type##* const p##name = CAST_PTR(type##*, PLATFORM_VALUE(g_addr,t_addr,s_addr));									BOOST_STATIC_ASSERT( PLATFORM_VALUE(g_addr,t_addr,s_addr) != NULL );

#define FUNC_PTR(name, g_addr, t_addr, s_addr) enum FUNC_PTR_##name { PTR_##name = PLATFORM_VALUE(g_addr,t_addr,s_addr) };	BOOST_STATIC_ASSERT( GET_FUNC_PTR(name) != NULL );
#define DATA_PTR(name, g_addr, t_addr, s_addr) enum DATA_PTR_##name { PTR_##name = PLATFORM_VALUE(g_addr,t_addr,s_addr) };	BOOST_STATIC_ASSERT( GET_DATA_PTR(name) != NULL );

namespace Yelo
{
	namespace Memory
	{
	};
};