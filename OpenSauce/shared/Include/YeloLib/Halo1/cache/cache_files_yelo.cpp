/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/cache/cache_files_yelo.hpp>

#include <YeloLib/Halo1/open_sauce/blam_memory_upgrades.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_global_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/yelo_version.hpp>

namespace Yelo
{
	namespace Cache
	{
#if PLATFORM_IS_EDITOR
		void s_cache_file_string_id_storage_header::Initialize()
		{
			memset(this, 0, sizeof(*this));

			this->signature = k_signature;
			this->set_count = _string_id::k_last_valid_set;
		}
#endif
		bool s_cache_file_string_id_storage_header::IsValid() const
		{
			return signature == k_signature && set_count <= _string_id::k_number_of_sets;
		}


#if PLATFORM_IS_EDITOR
		void s_cache_header_yelo::InitializeForNewMap()
		{
			memset(this, 0, sizeof(*this));

			this->signature = k_signature;
			this->version = k_version;
		}

		void s_cache_header_yelo::InitializeBuildInfo(_enum stage, uint32 revision)
		{
			build_info.stage = stage;
			build_info.revision = revision;
			time(&build_info.timestamp);

			cstring stage_string = "";
			switch(stage)
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
		// Is the yelo header valid?
		bool s_cache_header_yelo::IsValid() const
		{
			if(HasHeader())
				return signature == k_signature && version == k_version && 
					k_memory_upgrade_increase_amount <= K_MEMORY_UPGRADE_INCREASE_AMOUNT && 
					TagVersioningIsValid();

			return true;
		}

		bool s_cache_header_yelo::BuiltWithOlderTools() const
		{
			return build_info.cheape.maj < K_OPENSAUCE_VERSION_BUILD_MAJ ||
				build_info.cheape.min < K_OPENSAUCE_VERSION_BUILD_MIN;
		}
		bool s_cache_header_yelo::BuiltWithNewerTools() const
		{
			return build_info.cheape.maj > K_OPENSAUCE_VERSION_BUILD_MAJ ||
				build_info.cheape.min > K_OPENSAUCE_VERSION_BUILD_MIN;
		}
	};
};