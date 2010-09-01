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

#include "Memory/Data.hpp"
#include "Interface/ControlTypes.hpp"

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_terminal_line_max_characters = 254,
		};
	};

	namespace Console
	{
		struct s_terminal_state
		{
			int16 keys_count;
			PAD16;
			Input::s_buffered_key keys[32+1];

			real_argb_color color;
			tag_string prefix; // prefix string, ie "halo( "
			char token_buffer[256];

			struct {
				char* text;
				UNKNOWN_TYPE(int16); // index in [text]
				int16 length;
				UNKNOWN_TYPE(int16); // index in [text]
				PAD16;
			}edit_text;
		};

		struct s_terminal_output_datum : Memory::s_datum_base_aligned
		{
			datum_index previous;	// previous line
			datum_index next;		// next line
			bool tab_stops;			// true if "|t" appears in the text. terminal tab stops: 0xA0, 0x140, 0x1D6
			char text[Enums::k_terminal_line_max_characters+1];
			UNKNOWN_TYPE(int32);	// I don't think this is used...
			real_argb_color color;
			uint32 display_time;	// how long this has been alive, in tick counts
		};
		typedef Memory::DataArray<s_terminal_output_datum, 32> t_terminal_output_data;


		struct s_terminal_globals
		{
			// I really don't think the following is really part 
			// of the actual terminal globals, but this data appears 
			// conveniently in the same place in both client and 
			// dedi builds so why not vOv
			struct {
				HANDLE handle, output;
				PAD32;

				// these just seem to be taken from current_state anyway, so wtf...
				tag_string prefix; // prefix string, ie "halo( "
				char token_buffer[256];
				int32 token_buffer_length;
			}platform;

			bool initialized;
			PAD24;
			t_terminal_output_data* output_data;
			datum_index first_line;
			datum_index last_line;
			s_terminal_state* current_state;
			bool receiving_input;
			PAD24;
			uint32 receiving_input_start_tick; // local time, when input began to be inputed

			UNKNOWN_TYPE(bool);
			PAD24;
			int32 rcon_client_index; // client number who is executing the current rcon command, or NONE
		};
		s_terminal_globals* TerminalGlobals();


		struct s_console_globals
		{
			bool active;
			bool unk1;
			PAD16;
			
			s_terminal_state terminal_state;

			struct {
				char entries[8][Enums::k_terminal_line_max_characters+1];
				int16 entries_count;
				int16 current_index;
				UNKNOWN_TYPE(int16);
				PAD16;
			}history;

		}; BOOST_STATIC_ASSERT( sizeof(s_console_globals) == 0x9C8 );
		s_console_globals* Globals();


		void Initialize();
		void Dispose();
		void PLATFORM_API Update(uint32 access_flags);
	};
};