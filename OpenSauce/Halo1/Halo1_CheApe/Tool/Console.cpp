/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/Console.hpp"

#if PLATFORM_TYPE == PLATFORM_TOOL

#include "Common/StringEditing.hpp"

namespace Yelo
{
	namespace Tool { namespace Console
	{
		int32 EnterCommand(const char* command_list
			, _Out_opt_ std::string* arguments_string
			, const char* line_start)
		{
			std::vector<std::string> command_array;
			std::string commands(command_list);

			bool newline_command = false;

			// split the command list into an array
			while(commands.length())
			{
				std::string segment;
				// get the next command in the list and add it to the command array
				StringEditing::GetStringSegment(commands, segment, nullptr, ";");
				command_array.push_back(segment);

				// remove the command from the commands string
				StringEditing::RemoveStringSegment(commands, nullptr, ";");

				// if the user can just press enter, we need to know to interpret that as a command
				if(!segment.compare("\n") && !newline_command)
				{
					newline_command = true;
				}
			}

			char command[128];
			puts("");
			// get the users command
			Console::ColorPrintF(Enums::_console_color_lightyellow, "%s: ", line_start);
			do
			{
				fgets(command, sizeof(command), stdin);
				//ignore newlines unless it is a valid command
			}while(!newline_command && command[0] == '\n');

			// seperate into command and arguments
			std::string command_string(command);

			// if the command is just a newline no parsing is necessary
			if(command[0] != '\n')
			{
				StringEditing::GetStringSegment(command_string, command_string, nullptr, " \n");
			}

			// if arguments are requested, set the passed strings value
			if(arguments_string)
			{
				// set the arguments string to the whole string, then remove the command
				arguments_string->assign(command);
				StringEditing::RemoveStringSegment(*arguments_string, nullptr, " ");
				StringEditing::GetStringSegment(*arguments_string, *arguments_string, nullptr, "\n");
			}

			int index = 0;
			bool command_found = false;

			// iterate through the commands until a matching one is/is not found
			for( auto& command : command_array )
			{
				if(!command_string.compare(command))
				{
					command_found = true;
					break;
				}
				index++;
			}

			// return the index of the matching command, or -1 if it did not match
			return command_found ? index : -1;
		}

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