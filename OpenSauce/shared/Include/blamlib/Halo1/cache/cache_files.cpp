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

		cstring MapsDirectory()
		{
#if PLATFORM_IS_EDITOR
			return Settings::Get().GetMapsPath();
#else
			return "maps\\";
#endif
		}

#if !PLATFORM_IS_EDITOR
#endif
	};
};