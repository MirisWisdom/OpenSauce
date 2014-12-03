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
		void PrintNewLine()
		{
			puts("");
		}

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

		static void GetScreenBufferInfo(CONSOLE_SCREEN_BUFFER_INFO& info)
		{
			HANDLE console_out = GetStdHandle(STD_OUTPUT_HANDLE);
			GetConsoleScreenBufferInfo(console_out, &info);
		}

		int32 GetConsoleWidth()
		{
			CONSOLE_SCREEN_BUFFER_INFO buffer_info;
			GetScreenBufferInfo(buffer_info);

			return (buffer_info.srWindow.Right - buffer_info.srWindow.Left) + 1;
		}

		int32 GetConsoleHeight()
		{
			CONSOLE_SCREEN_BUFFER_INFO buffer_info;
			GetScreenBufferInfo(buffer_info);

			return (buffer_info.srWindow.Bottom - buffer_info.srWindow.Top) + 1;
		}
	}; };
};
#endif