/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#if PLATFORM_IS_EDITOR || defined(API_DEBUG)

namespace Yelo
{
	namespace Debug
	{
		void FileInitialize();
		void FileDispose();

		void Write(cstring str = "", bool write_time_stamp = true);
		void WriteFormat(cstring format, ...);
	};
};
	// Writes [str] with a time stamp if 
	// [stamp] is true, to the debug file
	#define YELO_DEBUG(str, stamp) Yelo::Debug::Write( str, stamp );

	// Use in this way:
	// YELO_DEBUG_FORMAT( "%s", "hello world" );
	#define YELO_DEBUG_FORMAT(...) Yelo::Debug::WriteFormat(__VA_ARGS__);

	// Writes a new line without a timestamp
	// to the debug file
	#define YELO_DEBUG_NL() Yelo::Debug::Write("\n", false);
#else
	#define YELO_DEBUG(str, stamp) __noop
	#define YELO_DEBUG_FORMAT(...) __noop
	#define YELO_DEBUG_NL() __noop
#endif