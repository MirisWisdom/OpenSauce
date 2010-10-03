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
#include "Common/Precompile.hpp"
#include "Common/StringEditing.hpp"

namespace Yelo
{
	namespace StringEditing
	{
		// search a string for a substring starting with start_control and ending with end_control
		bool GetStringSegment(
			std::string& string_in, 
			std::string& string_out,
			const char* start_control,
			const char* end_control,
			const bool remove_first_char)
		{
			if(string_in.size() == 0)
				return false;

			std::string::size_type index_start = (start_control == NULL ? 0 : string_in.find_first_of(start_control));
			std::string::size_type index_end = string_in.find_first_of(end_control);

			// if the start control was not found, return
			// if we want to remove the start control character increment index_start
			if(index_start == std::string::npos)
				return false;

			else if((index_start != 0) && remove_first_char)
				index_start++;

			// if the end control was not found, go to the end of the string
			if(index_end == std::string::npos)
				index_end = string_in.size();
			// set the output string to the substring that was found
			string_out.assign(string_in, index_start, index_end - index_start);
			// if the output string length is 0, no substring was found, so return false
			return true;
		}
		// cuts a section of a string out that starts with start_control and ends with end_control
		void RemoveStringSegment(
			std::string& string_in,
			const char* start_control,
			const char* end_control,
			const bool remove_end_char)
		{
			std::string::size_type index_start = (start_control == NULL ? 0 : string_in.find_first_of(start_control));
			std::string::size_type index_end = string_in.find_first_of(end_control);

			if(index_start == std::string::npos)
				return;
			if(index_end == std::string::npos)
				index_end = string_in.size();

			string_in.replace(index_start, index_end + (remove_end_char ? 1 : 0), "");
		}
	};
};