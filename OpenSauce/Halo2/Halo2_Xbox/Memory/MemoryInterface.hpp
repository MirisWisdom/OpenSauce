/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

// See this header for more "Engine pointer markup system" documentation
#include <YeloLib/memory/memory_interface_base.hpp>

//////////////////////////////////////////////////////////////////////////
// Engine pointer markup system
//
// [h2_addr]	: Halo 2 (Disc) XBE memory address
// [h2au_addr]	: Halo 2 (Last AU) XBE memory address
// [alpha_addr]	: Halo 2 (MP Alpha) XBE memory address
//
//////////////////////////////////////////////////////////////////////////

#define ENGINE_DPTR(type, name, addr) \
	static type##** const pp##name = CAST_PTR(type##**, addr); BOOST_STATIC_ASSERT( addr != NULL );

#define ENGINE_PTR(type, name, addr) \
	static type##* const p##name = CAST_PTR(type##*, addr); BOOST_STATIC_ASSERT( addr != NULL );

#define FUNC_PTR(name, addr) enum FUNC_PTR_##name { PTR_##name = addr }; BOOST_STATIC_ASSERT( PTR_##name != NULL );
#define DATA_PTR(name, addr) enum DATA_PTR_##name { PTR_##name = addr }; BOOST_STATIC_ASSERT( PTR_##name != NULL );

#if PLATFORM_ID == PLATFORM_H2

	#define PLATFORM_VALUE(h2_value, h2au_value, alpha_value) h2_value
	// For cases where h2 and h2au are the exact same
	#define PLATFORM_VALUE_(h2_value, alpha_value) h2_value

#elif PLATFORM_ID == PLATFORM_H2_AU

	#define PLATFORM_VALUE(h2_value, h2au_value, alpha_value) h2au_value
	#define PLATFORM_VALUE_(h2_value, alpha_value) h2_value

#elif PLATFORM_ID == PLATFORM_H2_ALPHA

	#define PLATFORM_VALUE(h2_value, h2au_value, alpha_value) alpha_value
	#define PLATFORM_VALUE_(h2_value, alpha_value) alpha_value

#endif