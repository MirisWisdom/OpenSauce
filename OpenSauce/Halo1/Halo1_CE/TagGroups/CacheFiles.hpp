/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <Blam/Halo1/BlamMemoryUpgrades.hpp>
#include <TagGroups/Halo1/CacheDefinitions.hpp>
#include <TagGroups/Halo1/structure_bsp_definitions.hpp>
#include "Memory/Data.hpp"

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_number_of_cached_map_files = 6,
		};
	};

	namespace DataFiles
	{
		void Initialize();
		void Dispose();
	};

	namespace Cache
	{
		struct s_cache_file_globals
		{
			bool initialized;
			PAD24;
			struct {
				s_cache_header header;
				s_cache_tag_header* tag_header;
				TagGroups::structure_bsp* structure_bsp;
			}current_cache;
			PAD32;

			byte threading_data[0xAC8];

			struct {
				struct {
					HANDLE file_handle;
					FILETIME time;
				}runtime;
				s_cache_header header;
			}map_files[Enums::k_number_of_cached_map_files];

			struct {
				bool copy_in_progress;
				PAD8;
				UNKNOWN_TYPE(int16);

				tag_string name;
				int16 index;		// index to cache_files
				PAD16;
			}current_cache_files;	// currently selected cache_files item

			struct s_read_request : TStructImpl(0x30)
			{
			};
			struct {
				HANDLE event_handle;
				HANDLE thread_handle;
				HGLOBAL read_requests; // sizeof(s_read_request) * 512
				PAD32;
			}cache_read_threading;

			byte data_cache_files[0x40][3]; // s_data_file. sounds, loc, bitmaps
		}; BOOST_STATIC_ASSERT( sizeof(s_cache_file_globals) == 0x4418 );
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

		// Reads the cache file [relative_map_name] from the maps folder, and returns true if its header is valid
		// * If the cache file is made by OS tools, and the yelo header is invalid yelo_is_ok will be false
		// * If exception_on_fail is true, the standard game exception message for invalid maps will play
		bool ReadHeader(cstring relative_map_name, s_cache_header& out_header, bool& yelo_is_ok, bool exception_on_fail = false);

#if !PLATFORM_IS_DEDI
		void LoadSettings(TiXmlElement* cf_element);
		void SaveSettings(TiXmlElement* cf_element);
#endif
	};
};