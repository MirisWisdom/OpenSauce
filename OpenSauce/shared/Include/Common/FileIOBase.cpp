/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common\Precompile.hpp"
#include "Common\FileIOBase.hpp"

#include <string>
#include <direct.h>

namespace Yelo
{
	namespace FileIO
	{
		s_file_io_globals_base g_file_io_globals_base;

		Enums::file_io_open_error GetOpenErrorEnum(DWORD error)
		{
			if((error == ERROR_FILE_NOT_FOUND) || (error == ERROR_PATH_NOT_FOUND))
				return Enums::_file_io_open_error_file_does_not_exist;
			else if((error == ERROR_SHARING_VIOLATION) || (error == ERROR_LOCK_VIOLATION))
				return Enums::_file_io_open_error_file_in_use;
			else
				return Enums::_file_io_open_error_unknown;
		}

		Enums::file_io_delete_error GetDeleteErrorEnum(DWORD error)
		{
			if((error == ERROR_FILE_NOT_FOUND) || (error == ERROR_PATH_NOT_FOUND))
				return Enums::_file_io_delete_error_does_not_exist;
			else if((error == ERROR_SHARING_VIOLATION) || (error == ERROR_LOCK_VIOLATION) || (error == ERROR_CURRENT_DIRECTORY))
				return Enums::_file_io_delete_error_in_use;
			else if((error == ERROR_ACCESS_DENIED))
				return Enums::_file_io_delete_error_denied;
			else
				return Enums::_file_io_delete_error_unknown;
		}

		void BuildDirectoryTree(const char* directory_tree)
		{
			std::string directory(directory_tree);
			std::string::size_type offset = 0;
			
			// look for all instances of \ and /, and create the directory tree for each one
			while((offset = directory.find_first_of("\\/", offset)) != std::string::npos)
			{
				std::string path = directory.substr(0, offset);
				_mkdir(path.c_str());
				offset++;
			}
		}

		bool PathExists(const char* path)
		{
			if(GetFileAttributes(path) == INVALID_FILE_ATTRIBUTES)
				return false;
			return true;
		}

		bool PathBuild(char* destination, bool append_slash, uint32 path_count, ...)
		{
			destination[0] = 0;

			va_list list;
			va_start(list, path_count);

			bool success = true;
			for(uint32 i = 0; success && (i < path_count); i++)
			{
				const char* path = va_arg(list, const char*);

				if(!PathCombine(destination, destination, path))
					success = false;
			}

			va_end(list);

			if(success && append_slash)
				AppendDirectorySlash(destination, MAX_PATH);

			return success;
		}
		bool AppendDirectorySlash(char* path, uint32 length, const char separator_char)
		{
			const char* final_char = strrchr(path, '\0');

			if(!final_char)
				return false;

			char separator[2] = { 0, 0 };
			separator[0] = separator_char;

			errno_t result = 0;
			final_char--;
			if(*final_char != separator_char)
				result = strcat_s(path, length, separator);

			return (result == 0);
		}

		bool GetDirectoryPath(char* destination, uint32 size, const char* path)
		{
			if(!destination || (size <= 1))
				return false;

			// find the last occurance of '\\'
			const char* filename_offset = strrchr(path, '\\');
			if(!filename_offset)
				return false;

			destination[0] = '\0';
			// calculate the character index from the two pointers
			uint32 index = CAST(uint32, filename_offset - path);
			// include the path divider
			index++;
			// copy the directory path to the destination
			errno_t error = strncpy_s(destination, size, path, index);

			if(error)
				return false;
			return true;
		}

		bool GetFileExtension(char* destination, uint32 size, const char* path)
		{
			const char* string_end = strrchr(path, '\0');
			const char* extension_start = strrchr(path, '.');

			if(!string_end || !extension_start)
				return false;

			if((string_end - 1) == extension_start)
				return true;

			if(0 != strcpy_s(destination, size, extension_start))
				return false;

			return true;
		}

		Enums::file_io_delete_error Delete_File(const char* file_path)
		{
			if(!PathExists(file_path))
				return Enums::_file_io_delete_error_does_not_exist;

			BOOL success = DeleteFile(file_path);

			if(!success)
			{
				DWORD error = GetLastError();
				
				return GetDeleteErrorEnum(error);
			}

			return Enums::_file_io_delete_error_none;
		}

		Enums::file_io_delete_error Delete_Directory(const char* directory, const bool delete_contents, const bool recursive)
		{
			if(!PathExists(directory))
				return Enums::_file_io_delete_error_does_not_exist;

			if(delete_contents)
			{
				WIN32_FIND_DATAA file_search;

				char search_filter[MAX_PATH] = "";
				PathCombine(search_filter, directory, "*");

				// returns the "." directory
				HANDLE search_handle = FindFirstFileA(search_filter, &file_search);
				if(search_handle != INVALID_HANDLE_VALUE)
				{
					// returns the ".." directory
					FindNextFile(search_handle, &file_search);

					char deletion_path[MAX_PATH] = "";
					// returns the first actual file/directory
					while(FindNextFile(search_handle, &file_search))
					{
						PathCombine(deletion_path, directory, file_search.cFileName);

						if(recursive && ((file_search.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY))
						{
							Enums::file_io_delete_error recursive_error = Delete_Directory(deletion_path, delete_contents, recursive);

							if(Enums::_file_io_delete_error_none != recursive_error)
								return recursive_error;
						}
						else
						{
							Enums::file_io_delete_error file_delete_error = Delete_File(deletion_path);

							if(Enums::_file_io_delete_error_none != file_delete_error)
								return file_delete_error;
						}
					}

					FindClose(search_handle);
				}
			}

			BOOL success = RemoveDirectory(directory);

			if(!success)
			{
				DWORD error = GetLastError();
				
				return GetDeleteErrorEnum(error);
			}

			return Enums::_file_io_delete_error_none;
		}

		void UpdateFileSize(s_file_info& info)
		{
			info.file_size = GetFileSize(info.file_handle, NULL);
		}

		/*!
		 * \brief
		 * Closes a file that is currently open.
		 * 
		 * \param info
		 * The file info struct contaning the files handles and pointers.
		 * 
		 * Closes a file that is currently open. If a file has been read into memory it's memory is deleted, if the file was memory mapped it is unmapped.
		 * In either case the file handle is closed.
		 */
		void CloseFile(s_file_info& info)
		{
			// reset flags
			info.m_flags.is_readable = false;
			info.m_flags.is_writable = false;

			// either delete the file in memory or unmap it
			if(info.m_flags.is_memory_mapped)
			{
				UnmapViewOfFile(info.data_pointer);
				CloseHandle(info.file_mapping_handle);
				info.file_mapping_handle = INVALID_HANDLE_VALUE;
			}
			else
				delete [] info.data_pointer;
			info.data_pointer = NULL;
			info.file_size = 0;
			info.data_length = 0;

			// release the file
			CloseHandle(info.file_handle);
			info.file_handle = INVALID_HANDLE_VALUE;
		}

		/*!
		 * \brief
		 * Opens a file located on disc for reading and/or writing.
		 * 
		 * \param info_out
		 * A file information struct for storing handles and pointers.
		 * 
		 * \param file_path
		 * The absolute location of the file to open.
		 * 
		 * \param access_type
		 * The type of acces to open the file for, read and/or write.
		 * 
		 * \param creation_type
		 * How to handle when the requested file is missing.
		 * 
		 * \returns
		 * A non-zero value if an error occurred.
		 * 
		 * Opens a file located on disc for reading and/or writing. Access type and creation type default to reading existing files only.
		 */
		Enums::file_io_open_error OpenFile(s_file_info& info_out,
			const char* file_path,
			const DWORD access_type,
			const Enums::file_io_open_create_option creation_type)
		{
			if(!PathExists(file_path) && (creation_type == Enums::_file_io_open_create_option_open_existing))
				return Enums::_file_io_open_error_file_does_not_exist;

			// create the access flags value for reading and/or writing
			DWORD access_flags = 0;
			if(Enums::_file_io_open_access_type_read & access_type)
			{
				info_out.m_flags.is_readable = true;
				access_flags |= GENERIC_READ;
			}
			if(Enums::_file_io_open_access_type_write & access_type)
			{
				info_out.m_flags.is_writable = true;
				access_flags |= GENERIC_WRITE;
			}

			// create the creation flags for when a file does not exist
			DWORD creation_flags = 0;
			switch(creation_type)
			{
			case Enums::_file_io_open_create_option_open_existing:
				creation_flags = OPEN_EXISTING;
				break;
			case Enums::_file_io_open_create_option_new:
				creation_flags = CREATE_ALWAYS;
				break;
			case Enums::_file_io_open_create_options_create_if_missing:
				creation_flags = OPEN_ALWAYS;
				break;
			}

			// create/open the file
			info_out.file_handle = CreateFile(file_path, 
				access_flags, NULL, NULL, creation_flags, FILE_ATTRIBUTE_NORMAL, NULL);

			// report errors
			if(info_out.file_handle == INVALID_HANDLE_VALUE)
				return GetOpenErrorEnum(GetLastError());

			info_out.file_size = GetFileSize(info_out.file_handle, NULL);

			return Enums::_file_io_open_error_none;
		}

		/*!
		 * \brief
		 * Memory maps an opened file for read and write access.
		 * 
		 * \param info
		 * A file info struct describing the file to memory map.
		 * 
		 * \param offset
		 * The location in the file to start the mapping from.
		 * 
		 * \param mapping_size
		 * The amount of data to map from the file.
		 * 
		 * \returns
		 * A non-zero value if an error occured.
		 * 
		 * Memory maps an opened file for read and write access. Memory mapped files cannot be write only.
		 * 
		 */
		Enums::file_io_read_error MemoryMapFile(s_file_info& info, DWORD offset, DWORD mapping_size)
		{
			// create access flags for the file mapping matching those the file was opened with
			DWORD access_flags = 0;
			if(info.m_flags.is_readable && info.m_flags.is_writable)
				access_flags = PAGE_READWRITE;
			else if(info.m_flags.is_readable && !info.m_flags.is_writable)
				access_flags |= PAGE_READONLY;
			else if(!info.m_flags.is_readable && info.m_flags.is_writable)
				return Enums::_file_io_read_error_writeonly_mapping;

			info.file_mapping_handle = CreateFileMapping(info.file_handle, NULL, access_flags, 0, 0, NULL);

			if(info.file_mapping_handle == INVALID_HANDLE_VALUE)
				return Enums::_file_io_read_error_failed_to_memory_map_file;

			// create access flags for the view mapping that match those the file were opened with
			access_flags = 0;
			if(info.m_flags.is_readable)
				access_flags |= FILE_MAP_READ;
			if(info.m_flags.is_writable)
				access_flags |= FILE_MAP_WRITE;

			// map the requested section of the file
			info.data_pointer = MapViewOfFile(info.file_mapping_handle, access_flags, offset, 0, mapping_size);
			if(!info.data_pointer)
				return Enums::_file_io_read_error_failed_to_memory_map_file;

			if(mapping_size == 0)
				info.data_length = info.file_size;
			else
				info.data_length = mapping_size;

			info.m_flags.is_memory_mapped = true;
			return Enums::_file_io_read_error_none;
		}

		/*!
		 * \brief
		 * Reads a section of a file into memory.
		 * 
		 * \param info
		 * A file info struct describing the file to read from.
		 * 
		 * \param offset
		 * The point in the file to start reading from. Defaults to 0.
		 * 
		 * \param length
		 * The length of data to read from the file. 0 will result in the entire file being read.
		 * 
		 * \returns
		 * A non-zero value if an error occured.
		 * 
		 * Reads a section of a file into memory. This function will allocate memory for the data, with the pointer stored in the s_file_info struct.
		 * If memory is already allocated it is deleted and replaced. The memory allocated here is deleted when the file is closed. This function
		 * is meant for reading a single block from a file that rarely changes if ever.
		 */
		Enums::file_io_read_error ReadFileToMemory(s_file_info& info, DWORD offset, DWORD length)
		{
			// delete the existing section
			if(info.data_pointer)
			{
				delete [] info.data_pointer;
				info.data_pointer = NULL;
			}

			info.data_length = 0;
			Enums::file_io_read_error success = ReadFileToMemory(info, CAST_PTR(char**, &info.data_pointer), offset, length);
			if(success != Enums::_file_io_read_error_none)
				return success;

			// zero length defaults to reading the entire file
			if(length == 0)
				info.data_length = info.file_size;
			else
				info.data_length = length;

			return Enums::_file_io_read_error_none;
		}

		/*!
		 * \brief
		 * Reads from an opened file into an external pointer.
		 * 
		 * \param info
		 * A file info struct describing the file to read from.
		 * 
		 * \param data_out
		 * Pointer to assign the allocated memory to.
		 * 
		 * \param offset
		 * The point in the file to start reading from. Defaults to 0.
		 * 
		 * \param length
		 * The length of data to read from the file. 0 will result in the entire file being read.
		 * 
		 * \returns
		 * A non-zero value if an error occured.
		 * 
		 * Reads from an opened file into an external pointer. This function will allocate memory for the read data and store the pointer in data_out.
		 * The memory allocated is NOT release when the file is closed so must be deleted elsewhere.
		 */
		Enums::file_io_read_error ReadFileToMemory(s_file_info& info, char** data_out, DWORD offset, DWORD length)
		{
			// check the file is valid for reading
			if(!data_out)
				return Enums::_file_io_read_error_destination_pointer_is_null;
			if(!info.m_flags.is_readable)
				return Enums::_file_io_read_error_file_is_writeonly;
			if(info.m_flags.is_memory_mapped)
				return Enums::_file_io_read_error_file_is_memory_mapped;

			UpdateFileSize(info);

			if(info.file_size == 0)
				return Enums::_file_io_read_error_file_is_empty;

			// zero length defaults to reading the entire file
			if(length == 0)
				length = info.file_size;

			// must not attempt to read beyond the end of the file
			if(info.file_size < (offset + length))
				return Enums::_file_io_read_error_reading_past_eof;

			*data_out = new char[length];

			// read the file section
			DWORD bytes_read = 0;

			OVERLAPPED file_read_setup;
			file_read_setup.hEvent = NULL;
			file_read_setup.Offset = offset;
			file_read_setup.OffsetHigh = 0;
			BOOL success = ReadFile(info.file_handle, *data_out, length, &bytes_read, &file_read_setup);

			if(!success || (bytes_read != length))
			{
				delete [] *data_out;
				*data_out = NULL;
				return Enums::_file_io_read_error_failed_to_read_from_file;
			}

			return Enums::_file_io_read_error_none;
		}

		/*!
		 * \brief
		 * Writes bytes to an opened file.
		 * 
		 * \param info
		 * A file info struct describing the file to write to.
		 * 
		 * \param data
		 * Pointer to the data to write.
		 * 
		 * \param length
		 * The length of data to write to the file.
		 * 
		 * \param offset
		 * The point in the file to start writing to. Defaults to NONE, which results in writing to the end of the file.
		 * 
		 * \returns
		 * A non-zero value if an error occured.
		 * 
		 * Writes bytes to an opened file.
		 */
		Enums::file_io_write_error WriteToFile(s_file_info& info, const char* data, DWORD length, DWORD offset)
		{
			if(!info.m_flags.is_writable)
				return Enums::_file_io_write_error_file_is_readonly;

			UpdateFileSize(info);

			// if no offset is set, write from the end of the file
			if(offset == NONE)
				offset = info.file_size;

			// must not attempt to write to an offset beyond the end of the file
			if(offset > info.file_size)
				return Enums::_file_io_write_error_write_offset_past_eof;

			DWORD bytes_written = 0;
			OVERLAPPED file_read_setup;
			file_read_setup.hEvent = NULL;
			file_read_setup.Offset = offset;
			file_read_setup.OffsetHigh = 0;
			BOOL success = WriteFile(info.file_handle, data, length, &bytes_written, &file_read_setup);

			UpdateFileSize(info);

			if(!success || (bytes_written != length))
				return Enums::_file_io_write_error_failed_to_write_to_file;

			return Enums::_file_io_write_error_none;
		}
	};
};