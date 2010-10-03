/*
    Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition
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

namespace Yelo
{
	namespace StringEditing
	{
		// search a string for a substring starting with start_control and ending with end_control
		bool GetStringSegment(
			std::string& string_in, 
			std::string& string_out,
			const char* start_control = NULL,
			const char* end_control = "\0",
			const bool remove_start_char = true);
		// cuts a section of a string out
		void RemoveStringSegment(
			std::string& string_in,
			const char* start_control = NULL,
			const char* end_control = "\0",
			const bool remove_end_char = true);
	};
};