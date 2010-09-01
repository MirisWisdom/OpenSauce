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
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/PostProcessingDebug.hpp"

#include "Common/YeloSettings.hpp"
#include "Game/EngineFunctions.hpp"
#include "Game/GameState.hpp"

namespace Yelo
{
	namespace Postprocessing
	{ 
		namespace Debug 
		{
			void		Write(cstring string)
			{			
				byte previous_dev_mode = *GameState::DeveloperMode();
				*GameState::DeveloperMode() = 4;
				Engine::Console::TerminalPrint(string);
				*GameState::DeveloperMode() = previous_dev_mode;
			}

			void		WriteLine(cstring format, ...)
			{
				va_list argptr;
				va_start(argptr, format);

				size_t length = (size_t)vsnprintf(NULL, 0, format, argptr) + 1; 
				char* buffer = new char[length];
				memset(buffer, 0, length);

				vsprintf_s(buffer, length, format, argptr);
				va_end(argptr);
				
				Write(buffer);
				delete [] buffer;
			}
			void		WriteD3DXErrors(LPD3DXBUFFER pBuffer, int32 error_count)
			{
				if(!pBuffer)
					return;
				char* string_pointer = CAST_PTR(char*, pBuffer->GetBufferPointer());

				bool remove_path = false;
				size_t work_dir_path_len = strlen(Settings::WorkingDirectoryPath());

				size_t string_length = strlen(string_pointer);
				int32 count = 0;
				while(strlen(string_pointer) > 0 && count < error_count)
				{
					if(strlen(string_pointer) < work_dir_path_len)
						remove_path = false;
					else
					{			
						// determine whether the line starts with the HaloCE path
						// if it does, chop it out
						char* line_start = new char[work_dir_path_len + 1];
						memset(line_start, 0, work_dir_path_len);
						memcpy_s(line_start, work_dir_path_len, string_pointer, work_dir_path_len);
						line_start[work_dir_path_len] = '\0'; 
						if(strcmp(line_start, Settings::WorkingDirectoryPath()) == 0)
							remove_path = true;
						delete [] line_start;
					}

					//Get the line
					string_pointer += (remove_path ? work_dir_path_len + 7 : 0);
					size_t line_length = strcspn(string_pointer, "\n") + 1;
					char* buffer = new char[line_length];
					memcpy_s(buffer, line_length, string_pointer, line_length);
					buffer[line_length - 1] = '\0';
				
					Write(buffer);

					delete[] buffer;
					buffer = NULL;
					//probably not safe, but should be ok
					string_pointer += line_length;

					count++;
				}
			}
		};
	};
};