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
				PAD8;
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
				data_pointer(nullptr),
				data_length(0)
			{ m_flags.is_readable = false; m_flags.is_writable = false; m_flags.is_memory_mapped = false;}
		};

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Converts a windows file open error number to a file io enum. </summary>
		/// <param name="error">	The error number. </param>
		/// <returns>	The file io enum value. </returns>
		///-------------------------------------------------------------------------------------------------
		Enums::file_io_open_error GetOpenErrorEnum(DWORD error);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Converts a windows file delete error number to a file io enum. </summary>
		/// <param name="error">	The error number. </param>
		/// <returns>	The file io enum value. </returns>
		///-------------------------------------------------------------------------------------------------
		Enums::file_io_delete_error GetDeleteErrorEnum(DWORD error);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Builds a directory tree. </summary>
		/// <param name="directory_tree">	The directory tree to create. </param>
		///-------------------------------------------------------------------------------------------------
		bool BuildDirectoryTree(cstring directory_tree);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Queries if a given path exists. </summary>
		/// <param name="path">	Path to test. </param>
		/// <returns>	true if it exists, false if it doesn't. </returns>
		///-------------------------------------------------------------------------------------------------
		bool PathExists(cstring path);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Combines a set of directories into a single path string. </summary>
		/// <param name="destination"> 	[out] The destination buffer for the path. </param>
		/// <param name="append_slash">	true to append a slash to the end. </param>
		/// <param name="path_count">  	The number of paths being combined. </param>
		/// <returns>	true if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------
		bool PathBuild(char* destination, bool append_slash, uint32 path_count, ...);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Appends a directory slash to a path string. </summary>
		/// <param name="path">			 	[in,out] The character buffer containing the path. </param>
		/// <param name="length">		 	The length of the buffer. </param>
		/// <param name="separator_char">	(Optional) The separator character to append. </param>
		/// <returns>	true if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------
		bool AppendDirectorySlash(char* path, uint32 length, const char separator_char = '\\');

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the directory path from a path with a file name. </summary>
		/// <param name="destination">	[out] The destination buffer for the path. </param>
		/// <param name="size">		  	The size of the destination buffer. </param>
		/// <param name="path">		  	Full pathname of the file. </param>
		/// <returns>	true if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------
		bool GetDirectoryPath(char* destination, uint32 size, cstring path);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets a file extension from a path. </summary>
		/// <param name="destination">	[out] The destination buffer for the extension. </param>
		/// <param name="size">		  	The size of the destination buffer. </param>
		/// <param name="path">		  	Full pathname of the file. </param>
		/// <returns>	true if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------
		bool GetFileExtension(char* destination, uint32 size, cstring path);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Deletes a file from the disc. </summary>
		/// <param name="path">	Full pathname of the file. </param>
		/// <returns>	An Enums::file_io_delete_error describing the result. </returns>
		///-------------------------------------------------------------------------------------------------
		Enums::file_io_delete_error FileDelete(cstring path);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Deletes a directory from the disc. </summary>
		/// <param name="path">			  	Full path of the directory. </param>
		/// <param name="delete_contents">
		/// 	True to delete the contents before deleting the directory.
		/// </param>
		/// <param name="recursive">
		/// 	True to delete all files and directories within the directory.
		/// </param>
		/// <returns>	An Enums::file_io_delete_error describing the result. </returns>
		///-------------------------------------------------------------------------------------------------
		Enums::file_io_delete_error DirectoryDelete(cstring path, const bool delete_contents, const bool recursive);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Updates the file size of the file described by info. </summary>
		/// <param name="info">	[in,out] The file info to update. </param>
		///-------------------------------------------------------------------------------------------------
		void UpdateFileSize(s_file_info& info);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Closes a file. </summary>
		/// <param name="info_out">	[in,out] The file info containing the file handle to close. </param>
		///-------------------------------------------------------------------------------------------------
		void CloseFile(s_file_info& info_out);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Opens a file. </summary>
		/// <param name="info_out">			[out] The file info struct to fill. </param>
		/// <param name="file_path">		Full pathname of the file. </param>
		/// <param name="access_type">  	(Optional) Type of file access to open the file with. </param>
		/// <param name="creation_type">
		/// 	(Optional) The behaviour to take with missing/existing files.
		/// </param>
		/// <returns>	An Enums::file_io_open_error describing the result. </returns>
		///-------------------------------------------------------------------------------------------------
		Enums::file_io_open_error OpenFile(s_file_info& info_out,
			cstring file_path,
			const Enums::file_io_open_access_type access_type = Enums::_file_io_open_access_type_read,
			const Enums::file_io_open_create_option creation_type = Enums::_file_io_open_create_option_open_existing);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Memory maps a file on disc. </summary>
		/// <param name="info_out">		[out] The file info struct to fill. </param>
		/// <param name="offset">	   	(Optional) The offset in the file to start the map at. </param>
		/// <param name="mapping_size">	(Optional) The size of the mapping. </param>
		/// <returns>	An Enums::file_io_read_error describing the result. </returns>
		///-------------------------------------------------------------------------------------------------
		Enums::file_io_read_error MemoryMapFile(s_file_info& info_out, DWORD offset = 0, DWORD mapping_size = 0);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Reads a file to memory stored in the info struct. </summary>
		/// <remarks>
		/// 	Reads a section of a file into memory. This function will allocate memory for the data,
		/// 	with the pointer stored in the s_file_info struct. If memory is already allocated it is
		/// 	deleted and replaced.The memory allocated here is deleted when the file is closed. This
		/// 	function is meant for reading a single block from a file that rarely changes if
		/// 	ever.< / remarks>
		/// </remarks>
		/// <param name="info">  	[in,out] The file info struct to read. </param>
		/// <param name="offset">	The offset in the file to read from. </param>
		/// <param name="length">	The length of data to read. </param>
		/// <returns>	An Enums::file_io_read_error describing the result. </returns>
		///-------------------------------------------------------------------------------------------------
		Enums::file_io_read_error ReadFileToInfoMemory(s_file_info& info, DWORD offset = 0, DWORD length = 0);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Reads a file to memory handled outside of the file io system. </summary>
		/// <param name="info">	   	[in,out] The file info to read from. </param>
		/// <param name="data_out">	[out] Pointer to the pointer to read the data to. </param>
		/// <param name="offset">	(Optional) The offset in the file to read from. </param>
		/// <param name="length">	(Optional) The length of data to read. </param>
		/// <returns>	An Enums::file_io_read_error describing the result. </returns>
		///-------------------------------------------------------------------------------------------------
		Enums::file_io_read_error ReadFileToMemory(s_file_info& info, void** data_out, DWORD offset = 0, DWORD length = 0);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Writes to a file. </summary>
		/// <param name="info">  	[in,out] The file info to write with. </param>
		/// <param name="data">  	The data to write. </param>
		/// <param name="length">	The length of data to write. </param>
		/// <param name="offset">	(Optional) The offset in the file to start writing from. </param>
		/// <returns>	An Enums::file_io_write_error describing the result. </returns>
		///-------------------------------------------------------------------------------------------------
		Enums::file_io_write_error WriteToFile(s_file_info& info, const void* data, DWORD length, DWORD offset = NONE);
	};
};