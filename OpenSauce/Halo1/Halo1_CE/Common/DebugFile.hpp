/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#ifdef API_DEBUG

namespace Yelo
{
	namespace DebugFile
	{
		void Initialize();
		void Dispose();

		void Write(cstring str = "", bool write_time_stamp = true);
		void WriteFormat(cstring format, ...);
	};
};

	// Writes [str] with a time stamp if 
	// [stamp] is true, to the debug file
	#define YELO_DEBUG(str, stamp) Yelo::DebugFile::Write( str, stamp );

	// Use in this way:
	// YELO_DEBUG_FORMAT( "%s", "hello world" );
	#define YELO_DEBUG_FORMAT(...) Yelo::DebugFile::WriteFormat(__VA_ARGS__);

	// Writes a new line without a timestamp
	// to the debug file
	#define YELO_DEBUG_NL() Yelo::DebugFile::Write("\n", false);

#else
	#define YELO_DEBUG(str, stamp) __noop
	#define YELO_DEBUG_FORMAT(...) __noop
	#define YELO_DEBUG_NL() __noop
#endif