/*
    Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#define PLATFORM_GUERILLA	0x00000001
#define PLATFORM_TOOL		0x00000002
#define PLATFORM_SAPIEN		0x00000003

#if !defined(PLATFORM_ID)
	#if defined(PLATFORM_TYPE_GUERILLA)
		#define PLATFORM_ID PLATFORM_GUERILLA
	#elif defined(PLATFORM_TYPE_TOOL)
		#define PLATFORM_ID PLATFORM_TOOL
	#elif defined(PLATFORM_TYPE_SAPIEN)
		#define PLATFORM_ID PLATFORM_SAPIEN
	#else
		#error PLATFORM_TYPE not defined. Step 1: Bash head on keyboard. Step 2: Repeat.
	#endif
#endif

// Calling convention of guerilla\tool\sapien
#define PLATFORM_API __cdecl

#define PLATFORM_IS_EDITOR 1


// See this header for more "Engine pointer markup system" documentation
#include <Memory/MemoryInterfaceShared.hpp>

//////////////////////////////////////////////////////////////////////////
// Engine pointer markup system
//
// [g_addr]	: Guerilla memory address
// [t_addr]	: Tool memory address
// [s_addr]	: Sapien memory address
//
//////////////////////////////////////////////////////////////////////////

#if PLATFORM_ID == PLATFORM_GUERILLA
	#define ENGINE_DPTR(type, name, g_addr, t_addr, s_addr) \
		static type##** const pp##name = CAST_PTR(type##**, g_addr);							BOOST_STATIC_ASSERT( g_addr != NULL );

	#define ENGINE_PTR(type, name, g_addr, t_addr, s_addr) \
		static type##* const p##name = CAST_PTR(type##*, g_addr);								BOOST_STATIC_ASSERT( g_addr != NULL );

	#define FUNC_PTR(name, g_addr, t_addr, s_addr) enum FUNC_PTR_##name { PTR_##name = g_addr };BOOST_STATIC_ASSERT( PTR_##name != NULL );
	#define DATA_PTR(name, g_addr, t_addr, s_addr) enum DATA_PTR_##name { PTR_##name = g_addr };BOOST_STATIC_ASSERT( PTR_##name != NULL );

	#define PLATFORM_VALUE(g_value, t_value, s_value) g_value
#elif PLATFORM_ID == PLATFORM_TOOL
	#define ENGINE_DPTR(type, name, g_addr, t_addr, s_addr) \
		static type##** const pp##name = CAST_PTR(type##**, t_addr);							BOOST_STATIC_ASSERT( t_addr != NULL );

	#define ENGINE_PTR(type, name, g_addr, t_addr, s_addr) \
		static type##* const p##name = CAST_PTR(type##*, t_addr);								BOOST_STATIC_ASSERT( t_addr != NULL );

	#define FUNC_PTR(name, g_addr, t_addr, s_addr) enum FUNC_PTR_##name { PTR_##name = t_addr };BOOST_STATIC_ASSERT( PTR_##name != NULL );
	#define DATA_PTR(name, g_addr, t_addr, s_addr) enum DATA_PTR_##name { PTR_##name = g_addr };BOOST_STATIC_ASSERT( PTR_##name != NULL );

	#define PLATFORM_VALUE(g_value, t_value, s_value) t_value
#elif PLATFORM_ID == PLATFORM_SAPIEN
	#define ENGINE_DPTR(type, name, g_addr, t_addr, s_addr) \
		static type##** const pp##name = CAST_PTR(type##**, s_addr);							BOOST_STATIC_ASSERT( s_addr != NULL );

	#define ENGINE_PTR(type, name, g_addr, t_addr, s_addr) \
		static type##* const p##name = CAST_PTR(type##*, s_addr);								BOOST_STATIC_ASSERT( s_addr != NULL );

	#define FUNC_PTR(name, g_addr, t_addr, s_addr) enum FUNC_PTR_##name { PTR_##name = s_addr };BOOST_STATIC_ASSERT( PTR_##name != NULL );
	#define DATA_PTR(name, g_addr, t_addr, s_addr) enum DATA_PTR_##name { PTR_##name = s_addr };BOOST_STATIC_ASSERT( PTR_##name != NULL );

	#define PLATFORM_VALUE(g_value, t_value, s_value) s_value
#endif