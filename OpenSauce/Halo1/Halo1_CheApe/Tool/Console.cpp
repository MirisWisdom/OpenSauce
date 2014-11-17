/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/Console.hpp"
#if PLATFORM_TYPE == PLATFORM_TOOL

namespace Yelo
{
	namespace Tool { namespace Console
	{
		/// <summary>	Print's an empty new line. </summary>
		void PrintNewLine()
		{
			puts("");
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Prints a formatted string to the console in a specified color. </summary>
		///
		/// <param name="color"> 	The color to print the string in. </param>
		/// <param name="format">	The format string to use. </param>
		/// <param name="...">		Variable list containing the format values. </param>
		void ColorPrintF(const WORD color,
			const char* format, ...)
		{
			// get a handle to the console
			HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

			// set the console color
			SetConsoleTextAttribute(console_handle, color);

			// print the formatted string
			va_list list;
			va_start(list, format); 
			vfprintf_s(stdout, format, list);
			va_end(list);

			// reset the console color back to the default
			SetConsoleTextAttribute(console_handle, 0x07);
		}

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
			const char* string, bool new_line)
		{
			// get a handle to the console
			HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

			// set the console color
			SetConsoleTextAttribute(console_handle, color);

			// if new_line is true, print the string using puts to add a newline character
			// otherwise use fputs
			if(new_line)
				puts(string);
			else
				fputs(string, stdout);

			// reset the console color back to the default
			SetConsoleTextAttribute(console_handle, 0x07);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the console width in characters. </summary>
		///
		/// <returns>	The console width. </returns>
		int32 GetConsoleWidth()
		{
			CONSOLE_SCREEN_BUFFER_INFO buffer_info;
			HANDLE console_out = GetStdHandle(STD_OUTPUT_HANDLE);
			GetConsoleScreenBufferInfo(console_out, &buffer_info);

			return (buffer_info.srWindow.Right - buffer_info.srWindow.Left) + 1;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the console height in characters. </summary>
		///
		/// <returns>	The console height. </returns>
		int32 GetConsoleHeight()
		{
			CONSOLE_SCREEN_BUFFER_INFO buffer_info;
			HANDLE console_out = GetStdHandle(STD_OUTPUT_HANDLE);
			GetConsoleScreenBufferInfo(console_out, &buffer_info);

			return (buffer_info.srWindow.Bottom - buffer_info.srWindow.Top) + 1;
		}
	}; };
};
#endif