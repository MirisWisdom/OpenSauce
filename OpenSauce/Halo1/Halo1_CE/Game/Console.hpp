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
	namespace Players
	{
		struct s_player_datum;
	};

	namespace Enums
	{
		enum {
			k_terminal_line_max_characters = 254,
		};

		enum hud_chat_type : long_enum
		{
			_hud_chat_type_none = CAST(long_enum,NONE),

			_hud_chat_type_all = 0,
			_hud_chat_type_team,
			_hud_chat_type_vehicle,
			_hud_chat_type_server,
			_hud_chat_type_info_msg, // used for messages pulled from Strings.dll. Convert 'hud_chat_network_data.message' to a long using 'wtol' to get the resource ID
		};
	};

	namespace Console
	{
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

			bool is_active;
			PAD24;
			int32 rcon_machine_index; // machine that is executing the current rcon command, or NONE
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

		}; BOOST_STATIC_ASSERT( sizeof(s_console_globals) == 0x9C4 );
		s_console_globals* Globals();

		struct s_hud_chat_globals_data
		{
			bool active; PAD24;
			Enums::hud_chat_type chat_type;

			s_terminal_state terminal_state;

			uint32 line_timers[8];
		}; BOOST_STATIC_ASSERT( sizeof(s_hud_chat_globals_data) == 0x1E8 );
		s_hud_chat_globals_data*		HudChatGlobals();
#if !PLATFORM_IS_DEDI
		int32 HudChatLineCount();
#endif


		void Initialize();
		void Dispose();
		void PLATFORM_API Update(uint32 access_flags);

		// Predicate to use for determining when to send a message to players
		typedef bool (* proc_send_hud_chat_predicate)(Players::s_player_datum* player, 
			Players::s_player_datum* src_player, datum_index src_player_vehicle_index);
		// Predicate for always sending a message to 
		bool SendHudChatToEveryonePredicate(Players::s_player_datum* player, 
			Players::s_player_datum* src_player, datum_index src_player_vehicle_index);

		// Send [messsage] to all valid players (machine_index != NONE) and who pass the [send_predicate]
		void SendHudChat(Enums::hud_chat_type msg_type, wcstring message, byte player_number,
			proc_send_hud_chat_predicate send_predicate = SendHudChatToEveryonePredicate);
	};
};