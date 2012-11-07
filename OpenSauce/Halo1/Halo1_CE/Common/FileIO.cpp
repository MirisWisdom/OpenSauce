/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common\Precompile.hpp"
#include "Common\FileIO.hpp"

#include <YeloLib/files/files.hpp>
#include <YeloLib/main/main_yelo_base.hpp>

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
		static void ReadIDFileDefinitions()
		{
			// get the resouce from the module
			HRSRC resource = FindResource(Main::YeloModuleHandle(), MAKEINTRESOURCE(OS_CHECKSUMS_XML), "XML");
			if(!resource) return;
			HGLOBAL resource_handle = LoadResource(Main::YeloModuleHandle(), resource);
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
				file_def.m_flags.file_exists = PathExists(file_def.m_location);

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

				bool md5_matches = Engine::CompareMD5(CAST_PTR(const char*, mapping_pointer), info_out.file_size, file_def.m_md5);

				UnmapViewOfFile(mapping_pointer);
				CloseHandle(mapped_file);

				if(!md5_matches)
					return Enums::_file_io_open_error_md5_mismatch;
			}
#endif
			return Enums::_file_io_open_error_none;
		}

	};
};