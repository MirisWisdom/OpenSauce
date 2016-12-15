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
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Process a fully qualified string of script functions. </summary>
		/// 
		/// <remarks>	Script objects with higher access flags than provided can't be accessed. </remarks>
		/// 
		/// <param name="access_flags">	the valid hs function and global access flags. </param>
		bool PLATFORM_API console_process_command(
#if !PLATFORM_IS_STUBBS // HaloCE parameter only
			long_flags access_flags,
#endif
			cstring command);


		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Print information to the console. </summary>
		///
		/// <remarks>	Prints only when developer_mode >= 4. </remarks>
		///
		/// <param name="clear_screen">	true to clear the console screen before printing. </param>
		/// <param name="format">	   	Describes the buffer format to use. </param>
		void PLATFORM_API console_printf(bool clear_screen, cstring format, ...);
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Print a response for the user to the console. </summary>
		///
		/// <remarks>	Prints no matter what developer_mode is set to, but console must already be active (open). </remarks>
		///
		/// <param name="clear_screen">	true to clear the console screen before printing. </param>
		/// <param name="format">	   	Describes the buffer format to use. </param>
		void PLATFORM_API console_response_printf(bool clear_screen, cstring format, ...);
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Print a warning (red) message to the console. </summary>
		///
		/// <remarks>	Prints only when developer_mode >= 4. </remarks>
		/// 
		/// <param name="format">	Describes the buffer format to use. </param>
		void PLATFORM_API console_warning(cstring format, ...);

#if !PLATFORM_IS_STUBBS
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <remarks>	access_flags = 0. </remarks>
		bool PLATFORM_API console_process_remote_command(cstring command, int32 machine_index);
#endif
	};
};