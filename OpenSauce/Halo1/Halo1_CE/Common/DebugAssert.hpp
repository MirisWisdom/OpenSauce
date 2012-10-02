/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#define ASSERTS_ENABLED

#if defined(API_DEBUG) && defined(ASSERTS_ENABLED)
#include "Common/DebugFile.hpp"

// Assert the condition is true. Run assertion logic when it's false
#define ASSERT(value, message) if(!(value)) Yelo::Assert(#value, message, __FILE__, __LINE__, __FUNCTION__)

namespace Yelo
{
	static void Assert(const char* assertion, const char* message, const char* file, const INT line, const char* function)
	{
		YELO_DEBUG_FORMAT("Assertion: %s", assertion);
		YELO_DEBUG_FORMAT("Message: %s", message);
		YELO_DEBUG_FORMAT("File: %s", file);
		YELO_DEBUG_FORMAT("Line: %i", line);
		YELO_DEBUG_FORMAT("Function: %s", function);

		throw;
	}
};
#else
#define ASSERT(value, message) __noop;
#define ASSERT_TRUE(value, message) __noop;
#endif