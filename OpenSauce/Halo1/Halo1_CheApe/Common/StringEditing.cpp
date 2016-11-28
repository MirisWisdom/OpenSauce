/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Common/StringEditing.hpp"

namespace Yelo
{
	namespace StringEditing
	{
		bool GetStringSegment(
			const std::string& string_in, 
			std::string& string_out,
			const char* start_control,
			const char* end_control,
			const bool remove_first_char)
		{
			if(string_in.empty())
				return false;

			std::string::size_type index_start = start_control == nullptr
				? 0
				: string_in.find_first_of(start_control);
			std::string::size_type index_end = string_in.find_first_of(end_control, index_start);

			// if the start control was not found, return
			// if we want to remove the start control character increment index_start
			if(index_start == std::string::npos)
				return false;
			else if(index_start != 0 && remove_first_char)
				index_start++;

			// if the end control was not found, go to the end of the string
			if(index_end == std::string::npos)
				index_end = string_in.size();
			// set the output string to the substring that was found
			string_out.assign(string_in, index_start, index_end - index_start);
			// if the output string length is 0, no substring was found, so return false
			return true;
		}

		void RemoveStringSegment(
			std::string& string_in,
			const char* start_control,
			const char* end_control,
			const bool remove_end_char)
		{
			std::string::size_type index_start = start_control == nullptr
				? 0
				: string_in.find_first_of(start_control);
			std::string::size_type index_end = string_in.find_first_of(end_control);

			if(index_start == std::string::npos)
				return;
			if(index_end == std::string::npos)
				index_end = string_in.size();

			index_end += remove_end_char
				? 1
				: 0;
			string_in.replace(index_start, index_end, "");
		}

		void StringTruncate(
			std::string& string_in,
			const size_t max_length,
			const size_t start_segment_length,
			const char* seperator)
		{
			// the string is less than or equal to the max size, no truncation needed
			if(string_in.length() <= max_length)
				return;

			// calculate the area to replace
			int seperator_length = strlen(seperator);
			int end_segment_length = max_length - (start_segment_length + seperator_length);

			int replace_length = string_in.length() - end_segment_length - start_segment_length;

			// replace the string segment with the seperator
			string_in.replace(start_segment_length, 
				replace_length,
				seperator);
		}
	};
};