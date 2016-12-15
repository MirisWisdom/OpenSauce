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
// [ce_value]		: value for use in Client builds
// [cededi_value]	: value for use in Dedi builds
//
//////////////////////////////////////////////////////////////////////////

#if PLATFORM_IS_DEDI
	#define PLATFORM_VALUE(ce_value, cededi_value) cededi_value
#elif PLATFORM_IS_USER
	#define PLATFORM_VALUE(ce_value, cededi_value) ce_value
#endif

#define PLATFORM_PTR(type, ce_value, cededi_value) CAST_PTR(type, PLATFORM_VALUE(ce_value, cededi_value))
