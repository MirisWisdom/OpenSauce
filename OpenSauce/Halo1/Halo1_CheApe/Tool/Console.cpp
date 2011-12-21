/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/Console.hpp"
#if PLATFORM_ID == PLATFORM_TOOL

namespace Yelo
{
	namespace Tool { namespace Console
	{
		/*!
		 * \brief
		 * Prints a formatted string to the console in a specified color.
		 * 
		 * \param color
		 * The color to print the string in.
		 * 
		 * \param format
		 * The format string to use.
		 * 
		 * \param 
		 * Variable list containing the format values.
		 * 
		 * Prints a formatted string to the console in a specified color.
		 */
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
		/*!
		 * \brief
		 * Prints a string to the console in a specified color.
		 * 
		 * \param color
		 * The color to print the string in.
		 * 
		 * \param string
		 * The string to print.
		 * 
		 * \param new_line
		 * Set to true if you want a newline character added to the end of the string. 
		 * Defaults to false.
		 * 
		 * Prints a string to the console in a specified color.
		 */
		void ColorPrint(const WORD color,
			const char* string, bool new_line)
		{
			// get a handle to the console
			HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

			// set the console color
			SetConsoleTextAttribute(console_handle, color);

			// if new_line is true, print the string usint puts to add a newline character
			// otherwise use fputs
			if(new_line)
				puts(string);
			else
				fputs(string, stdout);

			// reset the console color back to the default
			SetConsoleTextAttribute(console_handle, 0x07);
		}
	};};
};
#endif