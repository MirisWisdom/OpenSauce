/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/cseries/cseries_base.hpp>
#include <YeloLib/Halo1/cseries/cseries_yelo.hpp>

namespace Yelo
{
	namespace blam
	{
		extern char g_display_assert_buffer[512]; // not the engine's 'temporary' buffer (which is actually 256), internal to OS
		// warn = halt == false
		void PLATFORM_API display_assert(cstring reason, cstring file, const uint32 line, bool halt);

		void* PLATFORM_API system_malloc(size_t size);
		void PLATFORM_API system_free(void* pointer);
		void* PLATFORM_API system_realloc(void* pointer, size_t size);
	};
};

#define YELO_ASSERT_ENABLED TRUE

#if PLATFORM_IS_EDITOR
	#define __YELO_ASSERT_PROLOGUE() ExitProcess(NONE)

	#define YELO_ASSERT(expression)															\
		if( !(expression) )																	\
		{																					\
			Yelo::blam::display_assert( #expression , __FILE__, __LINE__, true);			\
			__YELO_ASSERT_PROLOGUE();														\
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
			__YELO_ASSERT_PROLOGUE();																		\
		}

	#define YELO_ASSERT_CASE_UNREACHABLE()										\
		default: Yelo::blam::display_assert(nullptr, __FILE__, __LINE__, true);	\
			__YELO_ASSERT_PROLOGUE();											\
			__assume(false);													\
			break;

#elif !PLATFORM_IS_EDITOR && defined(API_DEBUG) && defined(ASSERTS_ENABLED)
	#define __YELO_ASSERT_PROLOGUE() __noop

	#define YELO_ASSERT(expression)															\
		if( !(expression) )																	\
		{																					\
			Yelo::Assert( #expression , nullptr, __FILE__, __LINE__, __FUNCTION__);			\
			__YELO_ASSERT_PROLOGUE();														\
		}
	#define YELO_ASSERT_WARN(expression)													\
		if( !(expression) )																	\
		{																					\
			Yelo::Assert( #expression , nullptr, __FILE__, __LINE__, __FUNCTION__, false);	\
		}

	#define YELO_ASSERT_DISPLAY(expression, format, ...)									\
		if( !(expression) )																	\
		{																					\
			sprintf_s(Yelo::blam::g_display_assert_buffer, format, __VA_ARGS__);			\
			Yelo::Assert( #expression , Yelo::blam::g_display_assert_buffer,				\
				__FILE__, __LINE__, __FUNCTION__);											\
			__YELO_ASSERT_PROLOGUE();														\
		}

	#define YELO_ASSERT_CASE_UNREACHABLE()													\
		default: Yelo::Assert("unreachable", nullptr, __FILE__, __LINE__, __FUNCTION__);	\
			__YELO_ASSERT_PROLOGUE();														\
			__assume(false);																\
			break;
#else
	#undef YELO_ASSERT_ENABLED
	#define YELO_ASSERT_ENABLED FALSE

	#define YELO_ASSERT(expression)							__noop;
	#define YELO_ASSERT_WARN(expression)					__noop;
	#define YELO_ASSERT_DISPLAY(expression, format, ...)	__noop;
	#define YELO_ASSERT_CASE_UNREACHABLE()					default: __assume(false); break;
#endif

// used to be in cseries_yelo_base, but wanted to make use of YELO_ASSERT_ENABLED to disable API
#if YELO_ASSERT_ENABLED
namespace Yelo
{
	void Assert(cstring assertion, cstring message, cstring file, const int line, cstring function, bool halt = true);
};
#endif