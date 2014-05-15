/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/cache/cache_files_yelo.hpp>

#include <blamlib/Halo1/cache/cache_files.hpp>
#include <YeloLib/Halo1/cache/cache_files_structures_yelo.hpp>
#include <YeloLib/files/files.hpp>
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

		bool c_map_file_finder::SearchPath(cstring maps_path)
		{
			char map_file_path[MAX_PATH] = "";
			strcpy_s(map_file_path, maps_path);
			// pointer into map_file_path that starts after the maps_path string
			char* map_file_name = map_file_path + strlen(maps_path);

			cstring first_map_extension = m_search_for_yelo_first
				? K_MAP_FILE_EXTENSION_YELO
				: K_MAP_FILE_EXTENSION;
			cstring second_map_extension = m_search_for_yelo_first
				? K_MAP_FILE_EXTENSION
				: K_MAP_FILE_EXTENSION_YELO;

			*map_file_name = '\0';
			strcat(map_file_name, m_map_name);
			strcat(map_file_name, first_map_extension);
			if (!FileIO::PathExists(map_file_path))
			{
				m_map_extension = first_map_extension;
				return true;
			}

			*map_file_name = '\0';
			strcat(map_file_name, m_map_name);
			strcat(map_file_name, second_map_extension);
			if (!FileIO::PathExists(map_file_path))
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
		c_map_file_finder::c_map_file_finder(cstring map_name, bool search_for_yelo_first)
			: m_map_name(map_name)
			, m_map_extension(nullptr)
			, m_search_for_yelo_first(search_for_yelo_first)
		{
			m_maps_path.environment = Cache::MapsDirectory();
			m_maps_path.user_profile = Settings::PlatformUserMapsPath();
			m_maps_path.final = nullptr;
		}
		bool c_map_file_finder::TryAndFind(std::string& map_path)
		{
			if (SearchEnvironment() || SearchUserProfile())
			{
				map_path.reserve(MAX_PATH);

				map_path.append(m_maps_path.final);

				map_path.append(m_map_name);

				map_path.append(m_map_extension);

				map_path.shrink_to_fit();

				return true;
			}

			map_path.clear();

			return false;
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
			sprintf_s(build_info.build_string, "%06u." "%02i" "%02i.%02i." "%02i%02i." "%s", 
				revision,
				date_tm.tm_year - 100, // days since 1900, and we want a number relative to 2000
				date_tm.tm_mon, date_tm.tm_mday, 
				date_tm.tm_hour, date_tm.tm_sec,
				stage_string);

			build_info.cheape.maj = CAST(byte, K_OPENSAUCE_VERSION_BUILD_MAJ);
			build_info.cheape.min = CAST(byte, K_OPENSAUCE_VERSION_BUILD_MIN);
			build_info.cheape.build = CAST(uint16, K_OPENSAUCE_VERSION_BUILD);

			ArrayCopy(build_info.uuid_buffer, uuid_buffer);
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
		bool s_cache_header_yelo::IsValid() const
		{
			if(HasHeader())
				return	signature == k_signature && 
						version == k_version && 
						k_memory_upgrade_increase_amount <= K_MEMORY_UPGRADE_INCREASE_AMOUNT && 
						TagVersioningIsValid();

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