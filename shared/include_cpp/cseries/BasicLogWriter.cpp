/*
    Kornner Studios: Shared Code

    Copyright (C)  Kornner Studios (http://kornner.com)

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
#include "Precompile.hpp"
#include "BasicLogWriter.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

void c_basic_log_writer::Initialize(LPCSTR log_name)
{
	// Get the time stamp and format it in a non-military time
	SYSTEMTIME time = { 0 };
	GetLocalTime(&time);

	WORD hour12 = time.wHour;
	hour12 += hour12 == 0 ? 
		12 : 
		hour12 > 12 ? 
			-12 : 
			0;

	Write("***********************************************************\n");
	Write(" %s: %hu/%hu/%hu %hu:%hu\n", log_name, time.wMonth, time.wDay, time.wYear, hour12, time.wMinute);
	Write("***********************************************************\n");
}

void c_basic_log_writer::Open()
{
	if(this->m_handle != INVALID_HANDLE_VALUE)
		Close();

	// Open the log
	this->m_handle = CreateFile(
#if _XBOX
		"game:\\"
#else
		""
#endif
		"DefaultLog.txt",
		GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// Always seek to the end (in case there is already something in the log)
	DWORD size = GetFileSize(this->m_handle, NULL);
	SetFilePointer(this->m_handle, size, NULL, FILE_BEGIN);
}

void c_basic_log_writer::Close()
{
	CloseHandle(this->m_handle);
	this->m_handle = INVALID_HANDLE_VALUE;
}

void c_basic_log_writer::Write(LPCSTR format, ...) 
{
	CHAR buffer[1024];
	{
		va_list arg_list;
		va_start( arg_list, format );

		vsprintf_s(buffer, format, arg_list);

		va_end( arg_list );
	}

	Open();
	{
		DWORD bytes_written = 0;
		WriteFile(this->m_handle, buffer, lstrlenA(buffer), &bytes_written, NULL);
	}
	Close();
}