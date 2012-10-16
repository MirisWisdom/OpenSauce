/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Common/DebugAssert.hpp"

#if defined(API_DEBUG) && defined(ASSERTS_ENABLED)
#include <crtdbg.h>

#include "Common/DebugFile.hpp"

namespace Yelo
{
	void Assert(cstring assertion, cstring message, cstring file, const int line, cstring function)
	{
		YELO_DEBUG_FORMAT("Assertion: %s", assertion);
		YELO_DEBUG_FORMAT("Message: %s", message);
		YELO_DEBUG_FORMAT("File: %s", file);
		YELO_DEBUG_FORMAT("Line: %i", line);
		YELO_DEBUG_FORMAT("Function: %s", function);

		// based on _ASSERT_EXPR's implementation
		_CrtDbgReport(_CRT_ASSERT, file, line, NULL, assertion);

		throw;
	}
};
#endif