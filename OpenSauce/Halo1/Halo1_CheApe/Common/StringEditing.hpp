/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
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
		void StringTruncate(
			std::string& string_in,
			const DWORD max_length,
			const DWORD start_segment_length,
			const char* seperator = "...");
	};
};