/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/scenario/scenario.hpp>

// Terminology:
// maps_path: The system directory path to a "maps\" folder. 
//				Generally one should be under the CWD, or the user profile. Dedicated server's default behavior is to
//				query the registry for the client's "EXE Path" before it checks the CWD.
// map_path: The system file path to a given cache file
// map_name: The name of a cache file without path information or extensions

namespace Yelo
{
	namespace Enums
	{
		enum cache_file_request_source : byte_enum {
			_cache_file_request_source_open_map,
			_cache_file_request_source_bitmaps,
			_cache_file_request_source_sounds,
			_cache_file_request_source_loc,
		};

		enum cache_file_type : _enum {
			_cache_file_type_campaign		= _scenario_type_campaign,
			_cache_file_type_multiplayer	= _scenario_type_multiplayer,
			_cache_file_type_main_menu		= _scenario_type_main_menu,
		};
	};

	namespace Cache
	{
		struct s_cache_file_globals;
		struct s_cache_header;
		struct s_cache_tag_header;
		struct s_cache_tag_instance;

		struct s_cache_file_request_params
		{
			typedef void (PLATFORM_API* proc_finished)(void* params);

			bool* finished_flag;// NOT optional
			void* finished_proc_params;	// optional
			proc_finished finished_proc;// optional
		}; BOOST_STATIC_ASSERT( sizeof(s_cache_file_request_params) == 0xC );

		extern cstring K_MAP_FILE_EXTENSION; // ".maps"
		extern cstring K_MAP_FILES_DIRECTORY;// "maps\"


		s_cache_file_globals* CacheFileGlobals();
		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Directory to the maps\ folder for the defined PLATFORM_TYPE. </summary>
		///
		/// <remarks>For editors, this is the maps path as defined by the editor profile settings. For the runtime, this is
		/// 	just "maps\".
		/// </remarks>
		///
		/// <returns>	. </returns>
		cstring MapsDirectory();

		// if true, data which is in a data file won't be loaded during the cache_file's tags load process
		bool DontLoadExternalData();
	};

	namespace TagGroups
	{
		struct structure_bsp_header;
		struct scenario_structure_bsp_reference;

		Cache::s_cache_tag_header* Index();
		Cache::s_cache_tag_instance const* Instances(); // Don't directly use this for getting tags
	};

	namespace blam
	{
		bool PLATFORM_API cache_file_read_request(/*datum_index tag_index,*/ // unused, and optimized out, at runtime
			uint32 offset, uint32 size, void* buffer, const Cache::s_cache_file_request_params& params, 
			bool block = true, Enums::cache_file_request_source source = Enums::_cache_file_request_source_open_map);

		// made up names; H1 didn't have a "geometry_cache" like it does for textures and sounds
		void PLATFORM_API cache_file_geometry_cache_for_models_open(Cache::s_cache_tag_header* tag_header);
		void PLATFORM_API cache_file_geometry_cache_for_models_close();
		void PLATFORM_API cache_file_geometry_cache_for_bsp_open(TagGroups::structure_bsp_header* bsp_header);
		void PLATFORM_API cache_file_geometry_cache_for_bsp_close(TagGroups::structure_bsp_header* bsp_header);

		bool PLATFORM_API cache_file_header_verify(Cache::s_cache_header* header, cstring scenario_name, bool critical);

		datum_index cache_file_tags_load(cstring scenario_name);

		void cache_file_tags_unload();

		void cache_file_structure_bsp_unload(TagGroups::scenario_structure_bsp_reference* reference);

		bool cache_file_structure_bsp_load(TagGroups::scenario_structure_bsp_reference* reference);
	};
};