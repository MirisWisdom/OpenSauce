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
#include "Common\Precompile.hpp"
#include "Common\FileIO.hpp"

#include <string>
#include "Memory\MemoryInterface.hpp"
#include "Common\YeloSettings.hpp"

#include "Game\EngineFunctions.hpp"

namespace Yelo
{
	namespace FileIO
	{
		s_file_io_globals g_file_io_globals;

		/*!
		 * \brief
		 * Reads file ID definitions from an embedded XML resource.
		 * 
		 * Reads file ID definitions from an embedded XML resource. The XML file has the files location and its md5 checksum for later validation.
		 */
		void ReadIDFileDefinitions()
		{
			// get the resouce from the module
			HRSRC resource = FindResource(Yelo::Memory::YeloModuleHandle(), MAKEINTRESOURCE(OS_CHECKSUMS_XML), "XML");
			if(!resource) return;
			HGLOBAL resource_handle = LoadResource(Yelo::Memory::YeloModuleHandle(), resource);
			if(!resource_handle) return;
			void* resource_pointer = LockResource(resource_handle);
			if(!resource_pointer) return;

			// read the xml from the resource pointer
			TiXmlDocument definition_xml;
			definition_xml.Parse(CAST_PTR(const char*, resource_pointer));

			TiXmlElement* root = definition_xml.FirstChildElement("osYeloChecksums");
			if(root == NULL)
				return;

			TiXmlElement* file_element = root->FirstChildElement("file");
			if(file_element == NULL)
				return;

			//get the file count by enumerating all file elements
			g_file_io_globals.m_id_file_count = 0;
			TiXmlElement* count_element = file_element;
			do{ g_file_io_globals.m_id_file_count++; }while(count_element = count_element->NextSiblingElement("file"));

			//.allocate the file definition memory
			g_file_io_globals.m_id_file_definitions = new s_id_file_definition[g_file_io_globals.m_id_file_count];

			// read all of the file definitions
			uint32 index = 0;
			do
			{
				s_id_file_definition& file_def = g_file_io_globals.m_id_file_definitions[index];

				const char* string_pointer = NULL;
				uint32 string_length = 0;

				// read the files ID
				string_pointer = file_element->Attribute("ID");
				string_length = strlen(string_pointer) + 1;

				file_def.m_id = new char[string_length];
				file_def.m_id[0] = 0;
				strcpy_s(file_def.m_id, string_length, string_pointer);

				// read the files MD5 if present
				string_pointer = file_element->Attribute("MD5");
				if(!string_pointer)
				{
					file_def.m_md5[0] = 0;
					file_def.m_flags.do_md5_check = false;
				}
				else
				{
					string_length = strlen(string_pointer) + 1;

					file_def.m_md5[0] = 0;
					strcpy_s(file_def.m_md5, sizeof(file_def.m_md5), string_pointer);
					file_def.m_flags.do_md5_check = true;
				}

				// read the files location
				string_pointer = file_element->GetText();

				// parse environment variables to create the absolute path
				std::string location(string_pointer);
				Settings::ParseEnvironmentVariables(location);

				string_length = location.length() + 1;
				file_def.m_location = new char[string_length];
				file_def.m_location[0] = 0;
				strcpy_s(file_def.m_location, string_length, location.c_str());

				// check the file exists
				file_def.m_flags.file_exists = FileExists(file_def.m_location);

				index++;
			}while(file_element = file_element->NextSiblingElement("file"));
		}

		void Initialize()
		{
			ReadIDFileDefinitions();
		}

		void Dispose()
		{
			// delete allocated memory in each file id definition
			for(uint32 i = 0; i < g_file_io_globals.m_id_file_count; i++)
			{
				s_id_file_definition& file_def = g_file_io_globals.m_id_file_definitions[i];

				delete [] file_def.m_id;
				delete [] file_def.m_location;
			}
			// delete the file definition array
			delete [] g_file_io_globals.m_id_file_definitions;
			g_file_io_globals.m_id_file_definitions = NULL;
			g_file_io_globals.m_id_file_count = 0;
		}

		Enums::file_io_open_error GetOpenErrorEnum(DWORD error)
		{
			if((error == ERROR_FILE_NOT_FOUND) || (error == ERROR_PATH_NOT_FOUND))
				return Enums::_file_io_open_error_file_does_not_exist;
			else if((error == ERROR_SHARING_VIOLATION) || (error == ERROR_LOCK_VIOLATION))
				return Enums::_file_io_open_error_file_in_use;
			else
				return Enums::_file_io_open_error_unknown;
		}

		bool FileExists(const char* file_path)
		{
			// shameless copy from YeloSharedSettings
			if(GetFileAttributes(file_path) != CAST(DWORD, NONE))
				return true;

			DWORD error = GetLastError();

			if(error == ERROR_FILE_NOT_FOUND || ERROR_PATH_NOT_FOUND)
				return false;

			// This should actually be unreachable...
			return false;
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
			if(!FileExists(file_path) && (creation_type != Enums::_file_io_open_create_options_create_if_missing))
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
		 * Opens a file referenced in the id file list.
		 * 
		 * \param info_out
		 * A file information struct for storing handles and pointers.
		 * 
		 * \param file_id
		 * The string ID used to reference a file in the id file list.
		 * 
		 * \returns
		 * A non-zero value if an error occurred.
		 * 
		 * Opens a file referenced in the id file list. ID files can only be readonly, otherwise the md5 would change when the file is written to.
		 * If the file id definition has an MD5 checksum the file is memory mapped and it's checksum compared. If the checksums do not match, an error is returned,
		 * but the file remains open for reading.
		 */
		Enums::file_io_open_error OpenFileByID(s_file_info& info_out, const char* file_id)
		{
			bool found = false;
			uint32 index = 0;
			for(index = 0; index < g_file_io_globals.m_id_file_count; index++)
			{
				// compare the file ids
				if(strcmp(g_file_io_globals.m_id_file_definitions[index].m_id, file_id) == 0)
				{
					found = true;
					break;
				}
			}

			if(!found)
				return Enums::_file_io_open_error_file_does_not_exist;

			s_id_file_definition& file_def = g_file_io_globals.m_id_file_definitions[index];

			// file presence was checked when initializing
			if(!file_def.m_flags.file_exists)
				return Enums::_file_io_open_error_file_does_not_exist;

			// open the file
			// ID files are always read only
			info_out.file_handle = CreateFile(file_def.m_location, 
				GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			if(info_out.file_handle == INVALID_HANDLE_VALUE)
				return GetOpenErrorEnum(GetLastError());

			info_out.file_size = GetFileSize(info_out.file_handle, NULL);

			if(info_out.file_size == 0)
				return Enums::_file_io_open_error_file_is_empty;

			info_out.m_flags.is_readable = true;

#ifdef FILE_IO_DO_MD5_CHECK
			if(file_def.m_flags.do_md5_check)
			{
				// map the file as we don't need to read it into memory
				HANDLE mapped_file = CreateFileMapping(info_out.file_handle, NULL, PAGE_READONLY, 0, 0, NULL);

				void* mapping_pointer = MapViewOfFile(mapped_file, FILE_MAP_READ, 0, 0, 0);

				char md5[33];
				Engine::GenerateMD5(CAST_PTR(const char*, mapping_pointer), info_out.file_size, md5);

				UnmapViewOfFile(mapping_pointer);
				CloseHandle(mapped_file);

				if(strcmp(file_def.m_md5, md5) != 0)
					return Enums::_file_io_open_error_md5_mismatch;
			}
#endif
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