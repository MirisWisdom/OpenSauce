/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/cache/cache_files_yelo.hpp>

#include <blamlib/Halo1/cache/cache_files.hpp>
#include <blamlib/Halo1/cache/cache_files_structures.hpp>
#include <blamlib/Halo1/cache/data_file.hpp>
#include <blamlib/Halo1/main/main.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <YeloLib/Halo1/cache/cache_files_structures_yelo.hpp>
#include <YeloLib/files/files.hpp>
#include <YeloLib/memory/memory_interface_base.hpp>
#include <YeloLib/Halo1/open_sauce/blam_memory_upgrades.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_global_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/yelo_version.hpp>
#include <YeloLib/Halo1/open_sauce/settings/yelo_shared_settings.hpp>

namespace Yelo
{
	namespace Cache
	{
		cstring K_MAP_FILE_EXTENSION_YELO = ".yelo";

		bool c_map_file_finder::g_search_for_yelo_first;

		bool c_map_file_finder::SearchPath(cstring maps_path)
		{
			char map_path[MAX_PATH] = "";
			strcpy_s(map_path, maps_path);
			// pointer into map_file_path that starts after the maps_path string
			char* map_file_name = map_path + strlen(maps_path);

			cstring first_map_extension = g_search_for_yelo_first
				? K_MAP_FILE_EXTENSION_YELO
				: K_MAP_FILE_EXTENSION;
			cstring second_map_extension = g_search_for_yelo_first
				? K_MAP_FILE_EXTENSION
				: K_MAP_FILE_EXTENSION_YELO;

			*map_file_name = '\0';
			strcat(map_file_name, m_map_name);
			strcat(map_file_name, first_map_extension);
			if (FileIO::PathExists(map_path))
			{
				m_map_extension = first_map_extension;
				return true;
			}

			*map_file_name = '\0';
			strcat(map_file_name, m_map_name);
			strcat(map_file_name, second_map_extension);
			if (FileIO::PathExists(map_path))
			{
				m_map_extension = second_map_extension;
				return true;
			}

			return false;
		}
		bool c_map_file_finder::SearchEnvironment()
		{
			if (SearchPath(m_maps_path.environment))
				m_maps_path.final = m_maps_path.environment;

			return m_maps_path.final == m_maps_path.environment;
		}
		bool c_map_file_finder::SearchUserProfile()
		{
			if (SearchPath(m_maps_path.user_profile))
				m_maps_path.final = m_maps_path.user_profile;

			return m_maps_path.final == m_maps_path.user_profile;
		}
		bool c_map_file_finder::SearchExePath()
		{
#if PLATFORM_IS_DEDI
			if (SearchPath(m_maps_path.game_exe))
				m_maps_path.final = m_maps_path.game_exe;

			return m_maps_path.final == m_maps_path.game_exe;
#else
			return false;
#endif
		}
		c_map_file_finder::c_map_file_finder(cstring map_name)
			: m_map_name(map_name)
			, m_map_extension(nullptr)
		{
			m_maps_path.environment = Cache::MapsDirectory();
			m_maps_path.user_profile = Settings::PlatformUserMapsPath();
			m_maps_path.game_exe = nullptr;
			m_maps_path.final = nullptr;

#if PLATFORM_IS_DEDI
			m_maps_path.game_exe = Main::RegistryGetGameExePath().c_str();
#endif
		}
		bool c_map_file_finder::TryAndFind(_Out_ std::string& map_path, _Out_opt_ errno_t* return_access)
		{
			bool file_found = false;

			if (SearchExePath() || SearchEnvironment() || SearchUserProfile())
			{
				map_path.reserve(MAX_PATH);

				map_path.append(m_maps_path.final);
				map_path.append(m_map_name);
				map_path.append(m_map_extension);

				map_path.shrink_to_fit();

				file_found = true;
			}
			else
				map_path.clear();

			// populate the emulated _access() result
			if (return_access != nullptr)
			{
				*return_access = file_found
					? k_errnone
					: ENOENT; // File name or path not found.
			}

			return file_found;
		}

		s_cache_file_data_load_state::s_cache_file_data_load_state(s_cache_header* cache_header, s_cache_tag_header* tag_header)
			: base_address(CAST_PTR(byte*, tag_header) + cache_header->tag_memory_size)
			, memory_available(Enums::k_tag_allocation_size_upgrade - cache_header->tag_memory_size)
		{
		}

		byte* s_cache_file_data_load_state::Alloc(s_cache_tag_instance* tag_instance, size_t bytes)
		{
			byte* address = base_address;
			if (bytes > memory_available)
			{
				YELO_ASSERT_DISPLAY(false, "need to log the tag which we can't load here");
			}

			memory_available -= bytes;
			base_address += bytes;
			return address;
		}

		byte* s_cache_file_data_load_state::ReadExternalData(s_cache_tag_instance* tag_instance, Enums::data_file_reference_type data_file)
		{
			int32 data_offset, data_size;
			if (!DataFileGetItemDataInfo(data_file, tag_instance->index_in_data_file, data_offset, data_size))
				return nullptr;

			size_t buffer_size = CAST(size_t, data_size);
			byte* buffer = Alloc(tag_instance, buffer_size);

			if (buffer != nullptr &&
				!DataFileReadItemData(data_file, CAST(uint32, data_offset), buffer, buffer_size))
				return nullptr;

			return buffer;
		}

		e_map_path_file_type GetMapNameFromPath(_Out_ char (&map_name)[_MAX_FNAME], cstring map_path)
		{
			auto map_file_type = e_map_path_file_type::invalid;

			char map_extension[_MAX_EXT];
			_splitpath_s(map_path,
				nullptr, 0,
				nullptr, 0,
				map_name, _countof(map_name),
				map_extension, _countof(map_extension));

			_strlwr_s(map_name);

			if (!strcmp(map_extension, Cache::K_MAP_FILE_EXTENSION))
			{
				map_file_type = e_map_path_file_type::map;
			}
			else if (!strcmp(map_extension, Cache::K_MAP_FILE_EXTENSION_YELO))
			{
				map_file_type = e_map_path_file_type::yelo;
			}

			return map_file_type;
		}

		e_cache_read_header_result ReadHeader(cstring map_path, _Out_ s_cache_header& header)
		{
			// try to open the file
			{
				FileIO::s_file_info map_file_info;

				auto open_error = FileIO::OpenFile(map_file_info, map_path);
				if (open_error != Enums::_file_io_open_error_none)
					return e_cache_read_header_result::open_failed;

				// try to read a buffer the size of a header
				auto read_error = FileIO::ReadFileToInfoMemory(map_file_info, 0, sizeof(header));

				if (map_file_info.data_length == sizeof(header))
					memcpy(&header, map_file_info.data_pointer, sizeof(header));

				FileIO::CloseFile(map_file_info);

				if (read_error != Enums::_file_io_read_error_none)
					return e_cache_read_header_result::read_failed;
			}

			bool valid = header.ValidForYelo();

			if (!valid)
				return e_cache_read_header_result::header_invalid;
			if (header.yelo.HasHeader() && !header.yelo.IsValid())
				return e_cache_read_header_result::yelo_header_invalid;

			return e_cache_read_header_result::success;

		}
		e_cache_read_header_result FindMapFileAndReadHeader(cstring map_name, _Out_ s_cache_header& header)
		{
			// first, try to find the map file
			std::string map_path;
			{
				auto map_finder = c_map_file_finder(map_name);

				if (!map_finder.TryAndFind(map_path))
					return e_cache_read_header_result::file_not_found;
			}

			return ReadHeader(map_path.c_str(), header);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Calculates a map cache's crc using a memory mapped view of the cache file. </summary>
		///
		/// <param name="cache_file">	The memory map view base address of the cache file. </param>
		///
		/// <returns>	The calculated checksum (CRC32) of the cache file. </returns>
		static uint32 CalculateChecksumFromMemoryMap(const byte* cache_file)
		{
			uint32 crc = NONE;

			// get pointers to the necessary cache data
			auto* header = 
				CAST_PTR(const s_cache_header*, cache_file);
			auto* tag_header = 
				CAST_PTR(const s_cache_tag_header*, cache_file + header->offset_to_index);
			auto* tag_instances = 
				CAST_PTR(const s_cache_tag_instance*, CAST_PTR(const byte*,tag_header) + sizeof(s_cache_tag_header));

			// the tag address needs correcting to match the data's starting point
			// we need to remove the actual runtime base address and instead apply the address we're using in our memory map (the virtual address)
			uintptr_t base_address = (uintptr_t)tag_header->tags_address;
			uintptr_t virtual_address = (uintptr_t)tag_instances;

			// get the scenario tag
			const s_cache_tag_instance& scenario_tag_instance = tag_instances[tag_header->scenario_index.index];
			auto* scnr = Memory::RebasePointer(
				scenario_tag_instance.Definition<const TagGroups::scenario>(), base_address, virtual_address);

			int bsp_count = scnr->structure_bsps.Count;
			if (bsp_count > 0)
			{
				// can't access the block normally as its pointer is not correct for the cache's starting point
				auto* structure_bsps_block = Memory::RebasePointer(
					scnr->structure_bsps.Definitions, base_address, virtual_address);

				// this isn't correct for non-PC maps apparently, but we ARE on a pc...AREN'T we
				const byte* bsp_data_start = cache_file + sizeof(s_cache_header);

				// crc the data for all bsp's
				size_t bsp_data_offset = 0;
				for (int i = 0; i < bsp_count; i++)
				{
					int32 bsp_data_size = structure_bsps_block[i].bsp_data_size;
					Memory::CRC(crc, bsp_data_start + bsp_data_offset, bsp_data_size);
					bsp_data_offset += bsp_data_size;
				}
			}

			// crc model vertices and indices data
			Memory::CRC(crc, cache_file + tag_header->vertices.offset, tag_header->model_data_size);

			// crc tag data
			Memory::CRC(crc, tag_header, header->tag_memory_size);

			return crc;
		}
		uint32 CalculateChecksum(cstring map_path)
		{
			uint32 map_crc = NONE;

			FileIO::s_file_info map_file;
			//do-while-false for simpler cleanup
			do
			{
				if (Enums::_file_io_open_error_none != FileIO::OpenFile(map_file, map_path))
					break;

				if (Enums::_file_io_read_error_none != FileIO::MemoryMapFile(map_file))
					break;

				map_crc = CalculateChecksumFromMemoryMap(CAST_PTR(const byte*,map_file.data_pointer));
			} while (false);

			FileIO::CloseFile(map_file);

			return map_crc;
		}
		uint32 FindMapFileAndCalculateChecksum(cstring map_name)
		{
			uint32 map_crc = NONE;

			// first, try to find the map file
			std::string map_path;
			{
				auto map_finder = c_map_file_finder(map_name);

				if (!map_finder.TryAndFind(map_path))
					return map_crc;
			}

			map_crc = CalculateChecksum(map_path.c_str());
			return map_crc;
		}

#if PLATFORM_IS_EDITOR && PLATFORM_TYPE == PLATFORM_TOOL
		void s_cache_file_resource_string_id_storage_header::Initialize()
		{
			memset(this, 0, sizeof(*this));

			this->signature = k_signature;
			this->set_count = _string_id::k_last_valid_set;
		}
#endif
		bool s_cache_file_resource_string_id_storage_header::IsValid() const
		{
			return signature == k_signature && set_count <= _string_id::k_number_of_sets;
		}


#if PLATFORM_IS_EDITOR && PLATFORM_TYPE == PLATFORM_TOOL
		void s_cache_header_yelo::InitializeForNewMap()
		{
			memset(this, 0, sizeof(*this));

			this->signature = k_signature;
			this->version = k_version;
		}

		void s_cache_header_yelo::InitializeForCacheBuild(bool using_mod_sets, cstring mod_name, bool use_memory_upgrades)
		{
			//////////////////////////////////////////////////////////////////////////
			// Setup the core tag versions
			this->tag_versioning.project_yellow = TagGroups::project_yellow::k_version;
			this->tag_versioning.project_yellow_globals = TagGroups::project_yellow_globals::k_version;
			//////////////////////////////////////////////////////////////////////////
			
			if (flags.uses_mod_data_files = using_mod_sets)
			{
				YELO_ASSERT(mod_name);
				strcpy_s(this->mod_name, mod_name);
			}

			if (this->flags.uses_memory_upgrades = use_memory_upgrades)
				this->k_memory_upgrade_increase_amount = K_MEMORY_UPGRADE_INCREASE_AMOUNT;
		}

		void s_cache_header_yelo::InitializeBuildInfo(_enum stage, uint32 revision, const byte (&uuid_buffer)[Enums::k_uuid_buffer_size])
		{
			build_info.stage = stage;
			build_info.revision = revision;
			time(&build_info.timestamp);

			cstring stage_string = "";
			switch (stage)
			{
			case Enums::_production_build_stage_ship:	stage_string = "ship";	break;
			case Enums::_production_build_stage_alpha:	stage_string = "alpha";	break;
			case Enums::_production_build_stage_beta:	stage_string = "beta";	break;
			case Enums::_production_build_stage_delta:	stage_string = "delta";	break;
			case Enums::_production_build_stage_epsilon:stage_string = "epsilon";	break;
			case Enums::_production_build_stage_release:stage_string = "release";	break;
			}

			tm date_tm;
			localtime_s(&date_tm, &build_info.timestamp); // Convert time to struct tm form
			// ######.YY.MM.DD.HHMM.stage
			sprintf_s(build_info.build_string, "%06u.%02i.%02i.%02i.%02i%02i.%s", 
				revision,
				date_tm.tm_year - 100, // years since 1900, and we want a number relative to 2000
				date_tm.tm_mon + 1, date_tm.tm_mday, 
				date_tm.tm_hour, date_tm.tm_min,
				stage_string);

			build_info.cheape.maj = CAST(byte, K_OPENSAUCE_VERSION_BUILD_MAJ);
			build_info.cheape.min = CAST(byte, K_OPENSAUCE_VERSION_BUILD_MIN);
			build_info.cheape.build = CAST(uint16, K_OPENSAUCE_VERSION_BUILD);

			ArrayCopy(build_info.uuid_buffer, uuid_buffer);
		}
		void s_cache_header_yelo::InitializeBuildInfo()
		{
			byte nil_uuid[Enums::k_uuid_buffer_size] = {};
			this->InitializeBuildInfo(Enums::_production_build_stage_ship, 0, nil_uuid);
		}
		void s_cache_header_yelo::InitializeBuildInfo(const TagGroups::s_project_yellow_scenario_build_info& build_info)
		{
			this->InitializeBuildInfo(build_info.build_stage, build_info.revision, build_info.uuid_buffer);
		}
		void s_cache_header_yelo::InitializeMinimumBuildInfo(const byte major, const byte minor, const uint16 build)
		{
			if(major == 0)
			{
				return;
			}
			
			build_info.minimum_os_build.maj = major;
			build_info.minimum_os_build.min = minor;
			build_info.minimum_os_build.build = build;

			if(version == 1)
			{
				version = k_version_minimum_build;
			}
		}
#endif

		bool s_cache_header_yelo::HasHeader() const
		{
			return signature != 0 && version != 0;
		}
		bool s_cache_header_yelo::TagVersioningIsValid() const
		{
			return	tag_versioning.project_yellow == TagGroups::project_yellow::k_version &&
					tag_versioning.project_yellow_globals == TagGroups::project_yellow_globals::k_version;
		}
		bool s_cache_header_yelo::BuildVersionIsValid() const
		{
			// If the major build number is zero, there is no minimum build version
			if(build_info.minimum_os_build.maj == 0)
			{
				return true;
			}

			// If the minimum build is less than the current build, return true
			if((build_info.minimum_os_build.maj <= K_OPENSAUCE_VERSION_BUILD_MAJ)
				&& (build_info.minimum_os_build.min <= K_OPENSAUCE_VERSION_BUILD_MIN)
				&& (build_info.minimum_os_build.build <= K_OPENSAUCE_VERSION_BUILD))
			{
				return true;
			}

			return false;
		}
		bool s_cache_header_yelo::IsValid() const
		{
			if(HasHeader())
				return	signature == k_signature && 
						((version == k_version) || (version == k_version_minimum_build)) && 
						k_memory_upgrade_increase_amount <= K_MEMORY_UPGRADE_INCREASE_AMOUNT && 
						TagVersioningIsValid() &&
						BuildVersionIsValid();

			return true;
		}
		bool s_cache_header_yelo::HasUuid() const
		{
			return ArrayIsZero(build_info.uuid_buffer);
		}

		bool s_cache_header_yelo::BuiltWithOlderTools() const
		{
			return	build_info.cheape.maj < K_OPENSAUCE_VERSION_BUILD_MAJ ||
					build_info.cheape.min < K_OPENSAUCE_VERSION_BUILD_MIN;
		}
		bool s_cache_header_yelo::BuiltWithNewerTools() const
		{
			return	build_info.cheape.maj > K_OPENSAUCE_VERSION_BUILD_MAJ ||
					build_info.cheape.min > K_OPENSAUCE_VERSION_BUILD_MIN;
		}
	};
};