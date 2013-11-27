/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/interface/terminal.hpp>

namespace Yelo
{
	namespace Console
	{
		struct s_console_globals
		{
			bool active;
			bool enabled; // HaloPC only, for -console
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
	};

	namespace blam
	{
		bool PLATFORM_API console_process_command(cstring command);


		void PLATFORM_API console_printf(bool clear_screen, cstring format, ...);

		// Prints no matter what developer_mode is set to
		void PLATFORM_API console_printf_info(bool clear_screen, cstring format, ...);

		void PLATFORM_API console_warning(cstring format, ...);

		bool PLATFORM_API console_process_remote_command(cstring command, int32 machine_index);
	};
};