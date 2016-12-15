/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/input/input_windows.hpp>
#include <blamlib/Halo1/memory/data.hpp>

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
		typedef Memory::DataArray<s_terminal_output_datum, 32> terminal_output_data_t;

		struct s_terminal_state
		{
			Input::s_buffered_key keys[32+1];

			real_argb_color color;
			tag_string prefix; // prefix string, eg "halo( "
			char token_buffer[256];

			struct {
				char* text;
				UNKNOWN_TYPE(int16); // index in [text]
				int16 length;
				UNKNOWN_TYPE(int16); // index in [text]
				PAD16;
			}edit_text;
		}; BOOST_STATIC_ASSERT( sizeof(s_terminal_state) == 0x1C0 );

		struct s_terminal_globals
		{
#if !PLATFORM_IS_STUBBS
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
#endif

			bool initialized;
			PAD24;
			terminal_output_data_t* output_data;
			datum_index first_line;
			datum_index last_line;
			s_terminal_state* current_state;
			bool receiving_input;
			PAD24;
			uint32 receiving_input_start_tick; // local time, when input began to be inputed

#if !PLATFORM_IS_STUBBS
			bool is_active;
			PAD24;
			int32 rcon_machine_index; // machine that is executing the current rcon command, or NONE
#endif
		};
	};
};