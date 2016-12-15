/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once
#if PLATFORM_TYPE == PLATFORM_TOOL

namespace Yelo
{
	namespace Enums
	{
		enum console_color
		{
			_console_color_black = 0x0,
			_console_color_blue = 0x1,
			_console_color_green = 0x2,
			_console_color_aqua = 0x3,
			_console_color_red = 0x4,
			_console_color_purple = 0x5,
			_console_color_yellow = 0x6,
			_console_color_white = 0x7,
			_console_color_gray = 0x8,
			_console_color_lightblue = 0x9,
			_console_color_lightgreen = 0xA,
			_console_color_lightaqua = 0xB,
			_console_color_lightred = 0xC,
			_console_color_lightpurple = 0xD,
			_console_color_lightyellow = 0xE,
			_console_color_brightwhite = 0xF,

			_console_color
		};
	};
	namespace Tool { namespace Console
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Requests a command from the user. </summary>
		///
		/// <param name="command_list">
		/// 	A semi-colon delimited list of commands the user can input.
		/// </param>
		/// <param name="arguments_string">
		/// 	[out] A pointer to a std::string that arguments will be copied to.
		/// </param>
		/// <param name="line_start">
		/// 	(Optional) The text string to use at the start of the command line, defaults to "command".
		/// </param>
		///
		/// <returns>
		/// 	Returns the index of the command entered, or -1 if the command did not match.
		/// </returns>
		int32 EnterCommand(const char* command_list
			, _Out_opt_ std::string* arguments_string
			, const char* line_start = "command");

		/// <summary>	Print's an empty new line. </summary>
		void PrintNewLine();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Prints a formatted string to the console in a specified color. </summary>
		///
		/// <param name="color"> 	The color to print the string in. </param>
		/// <param name="format">	The format string to use. </param>
		/// <param name="...">		Variable list containing the format values. </param>
		void ColorPrintF(const WORD color,
			const char* format, ...);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Prints a string to the console in a specified color. </summary>
		///
		/// <param name="color">   	The color to print the string in. </param>
		/// <param name="string">  	The string to print. </param>
		/// <param name="new_line">
		/// 	Set to true if you want a newline character added to the end of the string. Defaults to
		/// 	false.
		/// </param>
		void ColorPrint(const WORD color,
			const char* string, bool new_line = false);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the console width in characters. </summary>
		///
		/// <returns>	The console width. </returns>
		int32 GetConsoleWidth();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the console height in characters. </summary>
		///
		/// <returns>	The console height. </returns>
		int32 GetConsoleHeight();
	}; };
};
#endif