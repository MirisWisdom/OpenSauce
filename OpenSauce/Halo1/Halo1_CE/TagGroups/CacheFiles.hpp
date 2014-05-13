/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/cache/cache_files.hpp>
#include <blamlib/Halo1/structures/structure_bsp_definitions.hpp>

#include <YeloLib/Halo1/open_sauce/blam_memory_upgrades.hpp>

#include "Settings/c_settings_singleton.hpp"

namespace Yelo
{
	namespace DataFiles
	{
		void Initialize();
		void Dispose();
	};

	namespace Cache
	{
		// Root directory of the "maps\" folder, or a null string if relative to the EXE
		char* RootDirectory(); // [256]

		void Initialize();
		void Dispose();

		void MapListReIntialize();

		uint32 CalculateCRC(void* cache_file);
		uint32 CalculateCRC(const char* cache_file_path, bool add_map_dir = false, bool add_extension = false);

		// Reads the cache file [relative_map_name] from the maps folder, and returns true if its header is valid
		// * If the cache file is made by OS tools, and the yelo header is invalid yelo_is_ok will be false
		// * If exception_on_fail is true, the standard game exception message for invalid maps will play
		// * If [for_map_list_add_map], will return false for non-mp maps (but won't exception)
		bool ReadHeader(cstring relative_map_name, s_cache_header& out_header, bool& yelo_is_ok, 
			bool exception_on_fail = false, bool for_map_list_add_map = false);
	};
};