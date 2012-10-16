/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#define ASSERTS_ENABLED

#if defined(API_DEBUG) && defined(ASSERTS_ENABLED)

// Assert the condition is true. Run assertion logic when it's false
#define ASSERT(value, message) if(!(value)) Yelo::Assert(#value, message, __FILE__, __LINE__, __FUNCTION__)

namespace Yelo
{
	void Assert(cstring assertion, cstring message, cstring file, const int line, cstring function);
};
#else
#define ASSERT(value, message) __noop;
#endif