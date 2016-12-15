/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/memory/array.hpp>

namespace Yelo
{
	namespace Cache
	{
		struct s_cache_header;
	};

	namespace Enums
	{
		// The stock engine uses a hard-coded list of known maps. These "system maps" are what was shipped with the game.
		enum {
			_system_multiplayer_map__first = 0,
			_system_multiplayer_map__last = 18,
			// i.e., non-system mp map, "Unknown Level"
			_system_multiplayer_map_user_created,

			k_number_of_system_multiplayer_maps = _system_multiplayer_map__last+1,
		};
	};

	namespace Interface
	{
		struct s_map_list_map_info
		{
			int32 system_map_index; // index used for things like the UI map list
			cstring name;
			bool is_original; // ie, bungie made it
			PAD24;
		}; BOOST_STATIC_ASSERT( sizeof(s_map_list_map_info) == 0xC );

		// This is the engine's struct for representing maps it found in the maps\ folder.
		// We don't define an actual ctor\dtor because of this
		struct s_map_list_map
		{
			char* name;
			int32 system_map_index;
			bool valid;
			struct {
				// is this a multiplayer map?
				byte_flags is_mp_map : 1;
				// does this map have a header with yelo data?
				byte_flags has_yelo_header : 1;
				// does corresponding file use the .yelo extension?
				byte_flags is_yelo_file : 1;

				// has the crc already been calculated yet?
				byte_flags valid_crc : 1;
			}yelo_flags;
			PAD16;
			uint32 crc;

			// Our ctor initializer for this engine struct
			void Initialize(int32 system_map_index);
			// Initializes the name field and related flags from a map_path
			bool InitializeNameFromPath(cstring map_path);
			// Verifies the map's header and calculates yelo-only specific information from the header
			// returns whether the map, and thus this entry, is valid
			bool ReadHeader(cstring map_path);
			
			void InitializeFromHeader(const Cache::s_cache_header& header);

			// Our dtor disposer for this engine struct
			void Dispose();
		}; BOOST_STATIC_ASSERT( sizeof(s_map_list_map) == 0x10 );
		typedef Memory::GbxArray<s_map_list_map> map_list_data_t;

		map_list_data_t* MultiplayerMaps();

		bool MultiplayerMapIsSystemMap(cstring map_name);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Add a map via components of a full map pathname. </summary>
		///
		/// <param name="maps_path">	   Full pathname of the maps folder, with a directory slash at the end. Or empty if
		/// 	map_file_name behaves like a map_path.
		/// </param>
		/// <param name="map_file_name">   Filename of the map file (name and extension). Or full map_path if maps_path is
		/// 	empty.
		/// </param>
		/// <param name="system_map_index">	(Optional) zero-based index of the map. </param>
		///
		/// <returns>	Index of the new s_map_list_map, or NONE if it failed to add. </returns>
		int32 MapListAddMapFromPath(cstring maps_path, cstring map_file_name,
			int32 system_map_index = Enums::_system_multiplayer_map_user_created);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Lookup the index of a s_map_list_map by a map_name. </summary>
		///
		/// <param name="map_name">	Name of the map; lowercase and without any path components. </param>
		///
		/// <returns>	Index of the found s_map_list_map, or NONE if the map isn't registered in the map list. </returns>
		int32 MapListMapGetIndexFromName(cstring map_name);
	};

	namespace blam
	{
#if !PLATFORM_IS_EDITOR
		extern cstring const k_map_list_ignored_map_names[10+1+3];
		extern const Interface::s_map_list_map_info k_map_list_mp_maps[Enums::k_number_of_system_multiplayer_maps];
#endif

		cstring map_list_map_name(int32 index);
		int32 map_list_map_index(int32 index);
		bool map_list_map_valid(int32 index);
		uint32 map_list_map_get_crc(int32 index);
		bool map_list_map_is_original(int32 index);
		int32 map_list_map_get_index(cstring map_path);
		int32 map_list_map_count();
		void map_list_add_map(cstring map_name, int32 map_index);
		bool map_list_should_ignore(cstring map_path);

		void map_list_initialize();
		void map_list_dispose();
	};
};