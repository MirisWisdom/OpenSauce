/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Common/FileIO.hpp"

#include <YeloLib/files/files.hpp>
#include <YeloLib/main/main_yelo_base.hpp>
#include <YeloLib/configuration/c_configuration_file_factory.hpp>
#include <YeloLib/configuration/c_configuration_container.hpp>
#include <YeloLib/configuration/c_configuration_container_list.hpp>
#include <YeloLib/configuration/c_configuration_value.hpp>

#include "Memory/MemoryInterface.hpp"
#include "Settings/Settings.hpp"

#include "Game/EngineFunctions.hpp"

namespace Yelo
{
	namespace FileIO
	{
#pragma region XML containers
		class c_file_container
			: public Configuration::c_configuration_container
		{
		public:
			Configuration::c_configuration_value<std::string> m_id;
			Configuration::c_configuration_value<std::string> m_md5;
			Configuration::c_configuration_value<std::string> m_path;

			c_file_container()
				: Configuration::c_configuration_container("File")
				, m_id("ID", "")
				, m_md5("MD5", "")
				, m_path("Path", "")
			{ }

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets whether the checksum check is required. </summary>
			///
			/// <returns>	true if it is required, false if it is not. </returns>
			bool GetChecksumRequired() const
			{
				return !m_md5.GetConst().empty();
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the file's absolute path. </summary>
			///
			/// <returns>	The files's absolute path. </returns>
			std::string GetAbsolutePath() const
			{
				std::string path(m_path.GetConst());
				Settings::ParseEnvironmentVariables(path);

				return path;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets whether the file exists. </summary>
			///
			/// <returns>	true if it exists, false if it does not. </returns>
			bool GetFileExists() const
			{
				return FileIO::PathExists(GetAbsolutePath().c_str());
			}

		protected:
			const std::vector<Configuration::i_configuration_value* const> GetMembers() final override
			{
				return std::vector<Configuration::i_configuration_value* const> { &m_id, &m_md5, &m_path };
			}
		};

		class c_checksums_container
			: public Configuration::c_configuration_container
		{
		public:
			Configuration::c_configuration_container_list<c_file_container> m_files;

			c_checksums_container()
				: Configuration::c_configuration_container("Checksums")
				, m_files("File", []() { return c_file_container(); })
			{ }

		protected:
			const std::vector<Configuration::i_configuration_value* const> GetMembers() final override
			{
				return std::vector<Configuration::i_configuration_value* const> { &m_files };
			}
		};
#pragma endregion

		static c_checksums_container g_checksums;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Reads file ID definitions from an embedded XML resource. </summary>
		///
		/// <remarks>
		/// 	The XML file has the files location and its MD5 checksum for later validation.
		/// </remarks>
		static void ReadIDFileDefinitions()
		{
			// Get the resource from the module
			HRSRC resource = FindResource(Main::YeloModuleHandle(), MAKEINTRESOURCE(OS_CHECKSUMS_XML), "XML");
			if(!resource)
			{
				return;
			}

			HGLOBAL resource_handle = LoadResource(Main::YeloModuleHandle(), resource);
			if(!resource_handle)
			{
				return;
			}

			void* resource_pointer = LockResource(resource_handle);
			if(!resource_pointer)
			{
				return;
			}

			// Read the xml from the resource pointer
			auto config_file = Configuration::c_configuration_file_factory::CreateConfigurationFile(CAST_PTR(const char*, resource_pointer), "xml");
			if(!config_file || !config_file->Load())
			{
				return;
			}

			auto checksums_node = config_file->Root()->GetChild("OpenSauce.Checksums");
			if(!checksums_node)
			{
				return;
			}

			g_checksums.GetValue(*checksums_node);
		}

		void Initialize()
		{
			ReadIDFileDefinitions();
		}

		void Dispose() { }

		Enums::file_io_open_error OpenFileByID(s_file_info& info_out, const char* file_id)
		{
			auto found_file = std::find_if(g_checksums.m_files.begin(), g_checksums.m_files.end(),
				[file_id](const c_file_container& entry)
				{
					return entry.m_id.GetConst() == file_id;
				}
			);

			if(found_file == g_checksums.m_files.end())
			{
				return Enums::_file_io_open_error_file_does_not_exist;
			}

			auto& file_entry = *found_file;
			if(!file_entry.GetFileExists())
			{
				return Enums::_file_io_open_error_file_does_not_exist;
			}

			// Open the file
			// ID files are always read only
			info_out.file_handle = CreateFile(file_entry.GetAbsolutePath().c_str()
				, GENERIC_READ
				, 0
				, nullptr
				, OPEN_EXISTING
				, FILE_ATTRIBUTE_NORMAL
				, nullptr);

			if(info_out.file_handle == INVALID_HANDLE_VALUE)
			{
				return GetOpenErrorEnum(GetLastError());
			}

			info_out.file_size = GetFileSize(info_out.file_handle, nullptr);

			if(info_out.file_size == 0)
			{
				return Enums::_file_io_open_error_file_is_empty;
			}

			info_out.m_flags.is_readable = true;

#ifdef FILE_IO_DO_MD5_CHECK
			if(file_entry.GetChecksumRequired())
			{
				// map the file as we don't need to read it into memory
				HANDLE mapped_file = CreateFileMapping(info_out.file_handle, nullptr, PAGE_READONLY, 0, 0, nullptr);

				void* mapping_pointer = MapViewOfFile(mapped_file, FILE_MAP_READ, 0, 0, 0);

				bool md5_matches = Engine::CompareMD5(CAST_PTR(byte*, mapping_pointer)
					, info_out.file_size
					, file_entry.m_md5.GetConst().c_str());

				UnmapViewOfFile(mapping_pointer);
				CloseHandle(mapped_file);

				if(!md5_matches)
				{
					return Enums::_file_io_open_error_md5_mismatch;
				}
			}
#endif
			return Enums::_file_io_open_error_none;
		}

	};
};