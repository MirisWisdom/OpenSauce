/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/cache/cache_files.hpp>

#include <blamlib/Halo1/cache/cache_files_globals.hpp>
#include <blamlib/Halo1/cache/cache_files_structures.hpp>
#include <blamlib/Halo1/cache/data_file_structures.hpp>
#include <YeloLib/Halo1/open_sauce/settings/che_ape_settings.hpp>

namespace Yelo
{
	namespace Cache
	{
		cstring K_MAP_FILE_EXTENSION = ".map";
		cstring K_MAP_FILES_DIRECTORY = R"(maps\)";

		cstring MapsDirectory()
		{
#if PLATFORM_IS_EDITOR
			return Settings::Get().GetMapsPath();
#else
			return K_MAP_FILES_DIRECTORY;
#endif
		}

		bool s_cache_header::ValidSignatures() const
		{
			return	header_signature == k_header_signature && 
					footer_signature == k_footer_signature;
		}

		bool s_cache_header::ValidFileSize(int32 max) const
		{
			return size >= 0 && size <= max;
		}

		bool s_cache_header::ValidName() const
		{
			return strnlen_s(name, Enums::k_tag_string_length+1) <= Enums::k_tag_string_length;
		}

		bool s_cache_tag_instance::MatchesGroup(tag group_tag) const
		{
			return this->group_tag == group_tag ||
				// test the hierarchy graph
				parent_groups[0] == group_tag ||
				parent_groups[1] == group_tag;
		}

		bool s_cache_header::ValidForStock() const
		{
			if (!ValidSignatures())
				return false;

			if (!ValidFileSize(Enums::k_max_cache_size))
				return false;

			if (!ValidName())
				return false;

			if (version != k_version)
				return false;

			return true;
		}

		bool s_cache_header::ValidForYelo() const
		{
			if (!ValidSignatures())
				return false;

			if (!ValidFileSize(Enums::k_max_cache_size_upgrade))
				return false;

			if (!ValidName())
				return false;

			if (version != k_version)
				return false;

			return true;
		}

#if !PLATFORM_IS_EDITOR
#endif
	};
};