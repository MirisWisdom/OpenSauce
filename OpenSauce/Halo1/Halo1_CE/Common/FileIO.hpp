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

//#define FILE_IO_DO_MD5_CHECK

namespace Yelo
{
	namespace Enums
	{
		enum file_io_open_error
		{
			_file_io_open_error_none = 0,
			_file_io_open_error_file_does_not_exist,
			_file_io_open_error_file_in_use,
			_file_io_open_error_file_is_empty,
			_file_io_open_error_md5_mismatch,
			_file_io_open_error_unknown,

			_file_io_open_error
		};
		enum file_io_open_access_type
		{
			_file_io_open_access_type_read	= 1 << 0,
			_file_io_open_access_type_write	= 1 << 1,

			_file_io_open_access_type
		};
		enum file_io_open_create_option
		{
			_file_io_open_create_option_open_existing,
			_file_io_open_create_option_new,
			_file_io_open_create_options_create_if_missing,

			_file_io_open_create_option
		};
		enum file_io_read_error
		{
			_file_io_read_error_none = 0,
			_file_io_read_error_reading_past_eof,
			_file_io_read_error_failed_to_read_from_file,
			_file_io_read_error_file_is_writeonly,
			_file_io_read_error_file_is_memory_mapped,
			_file_io_read_error_writeonly_mapping,
			_file_io_read_error_failed_to_memory_map_file,
			_file_io_read_error_destination_pointer_is_null,
			_file_io_read_error_file_is_empty,
			_file_io_read_error_unknown,

			_file_io_read_error
		};
		enum file_io_write_error
		{
			_file_io_write_error_none = 0,
			_file_io_write_error_file_is_readonly,
			_file_io_write_error_write_offset_past_eof,
			_file_io_write_error_device_is_write_protected,
			_file_io_write_error_failed_to_write_to_file,
			_file_io_write_error_unknown,

			_file_io_write_error
		};
	};
	namespace FileIO
	{
		struct s_id_file_definition
		{
			struct{
				bool do_md5_check;
				bool file_exists;
				PAD16;
			}m_flags;

			char* m_id;
			char m_md5[33];
			PAD24;
			char* m_location;
		};

		struct s_file_info
		{
			struct{
				bool is_readable;
				bool is_writable;
				bool is_memory_mapped;
				PAD8
			}m_flags;

			HANDLE file_handle;
			HANDLE file_mapping_handle;
			DWORD file_size;

			void* data_pointer;
			DWORD data_length;

			s_file_info() :
				file_handle(INVALID_HANDLE_VALUE),
				file_mapping_handle(INVALID_HANDLE_VALUE),
				file_size(0),
				data_pointer(NULL),
				data_length(0)
			{ m_flags.is_readable = false; m_flags.is_writable = false; m_flags.is_memory_mapped = false;}
		};

		struct s_file_io_globals
		{
			uint32 m_id_file_count;
			s_id_file_definition* m_id_file_definitions;
		};

		void Initialize();
		void Dispose();

		bool FileExists(const char* file_path);

		void UpdateFileSize(s_file_info& info);

		void CloseFile(s_file_info& info_out);

		Enums::file_io_open_error OpenFile(s_file_info& info_out,
			const char* file_path,
			const DWORD access_type = (DWORD)Enums::_file_io_open_access_type_read,
			const Enums::file_io_open_create_option creation_type = Enums::_file_io_open_create_option_open_existing);
		Enums::file_io_open_error OpenFileByID(s_file_info& info_out, const char* file_id);

		Enums::file_io_read_error MemoryMapFile(s_file_info& info_out, DWORD offset = 0, DWORD mapping_size = 0);
		Enums::file_io_read_error ReadFileToMemory(s_file_info& info, DWORD offset = 0, DWORD length = 0);
		Enums::file_io_read_error ReadFileToMemory(s_file_info& info, char** data_out, DWORD offset = 0, DWORD length = 0);

		Enums::file_io_write_error WriteToFile(s_file_info& info, const char* data, DWORD length, DWORD offset = NONE);
	};
};