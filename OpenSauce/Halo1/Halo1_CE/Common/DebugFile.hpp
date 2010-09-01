/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
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