/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/cache/cache_files.hpp>
#include <blamlib/Halo1/cache/data_file.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>

namespace Yelo
{
	namespace Cache
	{
		struct s_build_cache_file_globals
		{
			static cstring k_temp_cache_file_name;

			bool building;
			char scenario_name[Enums::k_max_tag_name_length+1];
			PAD24;
			uint32 crc;
			HANDLE file_handle;
			uint32 cache_stream_size;
			Cache::s_data_file sounds_data_file;
			Cache::s_data_file locale_data_file;
			Cache::s_data_file bitmaps_data_file;

			void DataFilesSave();
			void DataFilesPreprocessForSave();

			void TemporaryFileOpen(cstring filename = k_temp_cache_file_name);
			void TemporaryFileClose(cstring filename = k_temp_cache_file_name);
			void TemporaryFileCopy(cstring new_filename, cstring filename = k_temp_cache_file_name);
		};

		bool ScenarioLoadForCacheBuild(cstring scenario_name, cstring globals_name);
	};

	namespace blam
	{
		size_t stream_tag_to_buffer(datum_index tag_index, void* stream, size_t& return_stream_offset, uintptr_t virtual_base_address, tag_reference_name_reference* tag_names);
	};
};