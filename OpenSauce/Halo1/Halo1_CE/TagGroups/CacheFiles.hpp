/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/cache/cache_files.hpp>
#include <blamlib/Halo1/memory/array.hpp>
#include <blamlib/Halo1/structures/structure_bsp_definitions.hpp>

#include <YeloLib/Halo1/open_sauce/blam_memory_upgrades.hpp>

#include "Common/FileIO.hpp"

namespace Yelo
{
	namespace DataFiles
	{
		void Initialize();
		void Dispose();
	};

	namespace Cache
	{
		s_cache_file_globals* CacheFileGlobals();

		struct s_original_multipler_map
		{
			int32 index; // index used for things like the UI map list
			cstring name;
			bool is_original; // ie, bungie made it
			PAD24;
		};

		struct s_multiplayer_map_entry
		{
			char* name;
			int32 index;
			bool initialized;
			// HACK: YELO ONLY FIELD
			// This field is for yelo's use only, the game doesn't use it. Enables us to later go thru and set which maps are built for 
			// yelo and which aren't. Just for useful house keeping. However, we currently don't set this...
			bool is_yelo_based_map;
			PAD16;
			uint32 crc;
		};
		typedef Memory::GbxArray<s_multiplayer_map_entry> t_multiplayer_map_data;

		t_multiplayer_map_data* MultiplayerMaps();

		cstring* MapListIgnoredMapNames();

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
		bool ReadHeader(cstring relative_map_name, s_cache_header& out_header, bool& yelo_is_ok, bool exception_on_fail = false);

		void LoadSettings(TiXmlElement* cf_element);
		void SaveSettings(TiXmlElement* cf_element);
	};
};