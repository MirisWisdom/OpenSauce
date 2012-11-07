/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

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
		enum file_io_delete_error
		{
			_file_io_delete_error_none = 0,
			_file_io_delete_error_does_not_exist,
			_file_io_delete_error_in_use,
			_file_io_delete_error_directory_not_empty,
			_file_io_delete_error_denied,
			_file_io_delete_error_unknown = 0,

			_file_io_delete_error
		};
	};
	namespace FileIO
	{
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

		struct s_file_io_globals_base
		{
			uint32 m_id_file_count;
		};

		Enums::file_io_open_error GetOpenErrorEnum(DWORD error);
		Enums::file_io_delete_error GetDeleteErrorEnum(DWORD error);

		void BuildDirectoryTree(cstring directory_tree);
		bool PathExists(cstring path);
		bool PathBuild(char* destination, bool append_slash, uint32 path_count, ...);
		bool AppendDirectorySlash(char* path, uint32 length, const char separator_char = '\\');
		bool GetDirectoryPath(char* destination, uint32 size, cstring path);
		bool GetFileExtension(char* destination, uint32 size, cstring path);

		Enums::file_io_delete_error Delete_File(cstring directory);
		Enums::file_io_delete_error Delete_Directory(cstring directory, const bool delete_contents, const bool recursive);

		void UpdateFileSize(s_file_info& info);

		void CloseFile(s_file_info& info_out);

		Enums::file_io_open_error OpenFile(s_file_info& info_out,
			cstring file_path,
			const DWORD access_type = (DWORD)Enums::_file_io_open_access_type_read,
			const Enums::file_io_open_create_option creation_type = Enums::_file_io_open_create_option_open_existing);

		Enums::file_io_read_error MemoryMapFile(s_file_info& info_out, DWORD offset = 0, DWORD mapping_size = 0);
		Enums::file_io_read_error ReadFileToInfoMemory(s_file_info& info, DWORD offset = 0, DWORD length = 0);
		Enums::file_io_read_error ReadFileToMemory(s_file_info& info, void** data_out, DWORD offset = 0, DWORD length = 0);

		Enums::file_io_write_error WriteToFile(s_file_info& info, const void* data, DWORD length, DWORD offset = NONE);
	};
};