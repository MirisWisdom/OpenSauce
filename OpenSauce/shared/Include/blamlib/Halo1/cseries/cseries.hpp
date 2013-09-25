/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/cseries/cseries_base.hpp>
#include <YeloLib/cseries/cseries_yelo_base.hpp>

namespace Yelo
{
	namespace blam
	{
		extern char g_display_assert_buffer[512]; // not the engine's 'temporary' buffer (which is actually 256), internal to OS
		// warn = halt == false
		void PLATFORM_API display_assert(cstring reason, cstring file, const uint32 line, bool halt);
	};
};

#define YELO_ASSERT(expression)															\
	if( !(expression) )																	\
	{																					\
		Yelo::blam::display_assert( #expression , __FILE__, __LINE__, true);			\
	}
#define YELO_ASSERT_WARN(expression)													\
	if( !(expression) )																	\
	{																					\
		Yelo::blam::display_assert( #expression , __FILE__, __LINE__, false);			\
	}

#define YELO_ASSERT_DISPLAY(expression, format, ...)													\
	if( !(expression) )																					\
	{																									\
		sprintf_s(Yelo::blam::g_display_assert_buffer, format, __VA_ARGS__);							\
		Yelo::blam::display_assert(Yelo::blam::g_display_assert_buffer, __FILE__, __LINE__, true);		\
	}

#define YELO_ASSERT_CASE_UNREACHABLE()										\
	default: Yelo::blam::display_assert(nullptr, __FILE__, __LINE__, true);	\
		break;