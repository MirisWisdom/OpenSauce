/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/memory/array.hpp>

namespace Yelo
{
	namespace Interface
	{
		struct s_map_list_map_info
		{
			int32 map_index; // index used for things like the UI map list
			cstring name;
			bool is_original; // ie, bungie made it
			PAD24;
		}; BOOST_STATIC_ASSERT( sizeof(s_map_list_map_info) == 0xC );

		struct s_map_list_map
		{
			char* name;
			int32 map_index;
			bool valid;
			// HACK: YELO ONLY FIELD
			// This field is for yelo's use only, the game doesn't use it. Enables us to later go thru and set which maps are built for 
			// yelo and which aren't. Just for useful house keeping. However, we currently don't set this...
			bool is_yelo_based_map;
			PAD16;
			uint32 crc;
		}; BOOST_STATIC_ASSERT( sizeof(s_map_list_map) == 0x10 );
		typedef Memory::GbxArray<s_map_list_map> map_list_data_t;

		map_list_data_t* MultiplayerMaps();
	};

	namespace blam
	{
#if !PLATFORM_IS_EDITOR
		extern cstring map_list_ignored_map_names[10+1+3];
		extern const Interface::s_map_list_map_info k_original_multiplayer_maps[19];
#endif

		cstring map_list_map_name(int32 index);
		int32 map_list_map_index(int32 index);
		bool map_list_map_valid(int32 index);
		uint32 map_list_map_get_crc(int32 index);
		bool map_list_map_is_original(int32 index);
		int32 map_list_map_get_index(cstring map_name);
		int32 map_list_map_count();
		void map_list_add_map(cstring map_name, int32 map_index);
		bool map_list_map_is_ignored(cstring map_path);

		void map_list_initialize();
		void map_list_dispose();
	};
};