/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <Blam/Halo1/BlamMemoryUpgrades.hpp>
#include <TagGroups/Halo1/structure_bsp_definitions.hpp>
#include "Memory/Data.hpp"

namespace Yelo
{
	namespace DataFiles
	{
		void Initialize();
		void Dispose();
	};

	namespace Cache
	{
		struct s_original_multipler_map
		{
			int32 index; // index used for things like the UI map list
			cstring name;
			bool is_original; // ie, bungie made it
			PAD24;
		};

		// 0x497EC0, 0x48D8B0 - gets a map's description string
		// 0x629B3C, 0x59F4DC - ignored cache file names
		struct s_multiplayer_map_entry
		{
			cstring name;
			int32 index;
			bool exists;
			// HACK: YELO ONLY FIELD
			// This field is for yelo's use only, the game doesn't use it. 
			// Enables us to later go thru and set which maps are built for 
			// yelo and which aren't. Just for useful house keeping.
			bool is_yelo_basd_map;
			PAD16;
			uint32 crc;
		};
		typedef Memory::GbxArray<s_multiplayer_map_entry> t_multiplayer_map_data;

		t_multiplayer_map_data* MultiplayerMaps();

		// 0x642F78, 0x5B4DB8
		struct s_cache_file_globals
		{
			bool initialized;
			PAD24;
			struct {
				s_cache_header header;
				s_cache_tag_header* tag_index;
				TagGroups::structure_bsp* structure_bsp;
			}current_cache;
			PAD32;

			byte threading_data[0xAC8];

			struct {
				struct {
					HANDLE file_handle;
					PAD32; // ?
					PAD32; // ?
				}runtime;
				s_cache_header header;
			}cache_files[6]; //0x444220, 0x4449D0 - get cache_files index by scenario name

			struct {
				UNKNOWN_TYPE(bool);
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

		void Initialize();
		void Dispose();

		void MultiplayerMapsReIntialize();
	};
};