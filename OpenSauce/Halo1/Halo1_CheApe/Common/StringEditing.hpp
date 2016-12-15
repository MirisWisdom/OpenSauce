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
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	search a string for a substring starting with start_control and ending with end_control. </summary>
		///
		/// <param name="string_in">			The input string. </param>
		/// <param name="string_out">			[out] The output substring. </param>
		/// <param name="start_control">		(Optional) the character sequence which prefixes the portion to substring. </param>
		/// <param name="end_control">			the character sequence which suffixes (follows) the portion to substring. </param>
		/// <param name="remove_start_char">	TODO. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		bool GetStringSegment(
			const std::string& string_in, 
			std::string& string_out,
			const char* start_control = nullptr,
			const char* end_control = "",
			const bool remove_start_char = true);
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	cuts a section of a string out. </summary>
		///
		/// <param name="string_in">	  	[in,out] The string to cut. </param>
		/// <param name="start_control">  	(Optional) the character sequence which prefixes the portion to cut. </param>
		/// <param name="end_control">	  	the character sequence which suffixes (follows) the portion to cut. </param>
		/// <param name="remove_end_char">	TODO. </param>
		void RemoveStringSegment(
			std::string& string_in,
			const char* start_control = nullptr,
			const char* end_control = "",
			const bool remove_end_char = true);
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Removes a section of a string to bring it down to a maximum size. </summary>
		///
		/// <param name="string_in">		   	[in,out] The string to truncate. </param>
		/// <param name="max_length">		   	The maximum length of the string. </param>
		/// <param name="start_segment_length">	The length of the start segment. </param>
		/// <param name="seperator">		   	The seperator string that will replace the removed section. </param>
		void StringTruncate(
			std::string& string_in,
			const size_t max_length,
			const size_t start_segment_length,
			const char* seperator = "...");

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Appends the char [c] if [str] doesn't already end with it. </summary>
		///
		/// <param name="str">	[in,out] The string to append. </param>
		/// <param name="c">  	The character to append. </param>
		template<class _Elem, class _Traits, class _Ax>
		void AppendCharIfAbsent(
			std::basic_string<_Elem, _Traits, _Ax>& str,
			_Elem c)
		{
			auto r_iter = str.rbegin();
			if(*r_iter != c)
				str.append(1, c);
		}
	};
};