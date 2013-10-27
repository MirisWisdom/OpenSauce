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

		//////////////////////////////////////////////////////////////////////////
		// xbox support
		struct s_cache_header_xbox : public s_cache_header_base
		{
			enum {
				k_version = 5,

				k_pad_size = 485 * sizeof(int32),
			};

			tag header_signature;

			int32 version;
			int32 size; // file size
			uint32 compressed_file_length;

			uint32 offset_to_index;
			uint32 tag_memory_size;
			PAD64;
			tag_string name;
			tag_string build_string;
			Enums::cache_file_type cache_type;
			PAD16;
			uint32 crc;

			byte padding[k_pad_size];

			tag footer_signature;
		}; BOOST_STATIC_ASSERT( sizeof(s_cache_header_xbox) == 0x800 );

		typedef s_cache_tag_instance
			s_cache_tag_instance_xbox;
		BOOST_STATIC_ASSERT( sizeof(s_cache_tag_instance_xbox) == 0x20 );

		struct s_cache_tag_header_xbox
		{
			void* tags_address;			// 0x0
			datum_index scenario_index;	// 0x4
			uint32 checksum;			// 0x8
			int32 count;				// 0xC
			struct {
				int32 count;
				void* base_address;
			}vertices, indices;			// 0x10, 0x18
			tag signature;				// 0x20
		}; BOOST_STATIC_ASSERT( sizeof(s_cache_tag_header_xbox) == 0x24 );
		//////////////////////////////////////////////////////////////////////////

		bool ScenarioLoadForCacheBuild(cstring scenario_name, cstring globals_name);
	};

	namespace blam
	{
		size_t stream_tag_to_buffer(datum_index tag_index, void* stream, size_t& return_stream_offset, uintptr_t virtual_base_address, tag_reference_name_reference* tag_names);
	};
};