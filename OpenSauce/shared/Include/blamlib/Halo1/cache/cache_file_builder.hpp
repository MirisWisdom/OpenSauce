/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once
#if PLATFORM_TYPE == PLATFORM_TOOL

#include <blamlib/Halo1/cache/cache_files_structures.hpp>
#include <blamlib/Halo1/cache/data_file.hpp>
#include <blamlib/Halo1/cache/data_file_structures.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_cache_file_page_alignment_bit = Flags::k_alignment_4096bit,
			k_cache_file_page_size = FLAG(k_cache_file_page_alignment_bit),
			k_cache_file_page_size_mask = FLAG(k_cache_file_page_alignment_bit) - 1,
		};
	};

	namespace Flags
	{
		enum {
			_build_cache_file_begin_building_yelo_bit,
			_build_cache_file_begin_mod_sets_create_anew_bit,
			_build_cache_file_begin_mod_sets_store_scenario_resources_bit,
			_build_cache_file_begin_use_memory_upgrades_bit,
			_build_cache_file_begin_dump_tag_group_allocation_stats_bit,

			k_number_of_build_cache_file_begin_flags
		}; BOOST_STATIC_ASSERT(k_number_of_build_cache_file_begin_flags <= BIT_COUNT(byte));
	}

	namespace Cache
	{
		struct s_build_cache_file_globals
		{
			static cstring k_temp_cache_file_name;
			static cstring k_cache_file_extension;

			bool building;
			char scenario_name[Enums::k_max_tag_name_length+1];
			byte_flags begin_flags; // NOTE: non-standard member
			bool canceled; // NOTE: non-standard member
			PAD8;
			uint32 crc;
			HANDLE file_handle;
			int32 cache_stream_size;
			s_data_file_globals data_files;

			DWORD GetFileSize() const;

			bool WriteToFile(const void* buffer, int32 buffer_size);

			bool TemporaryFileOpen(cstring filename = k_temp_cache_file_name);
			void TemporaryFileClose(cstring filename = k_temp_cache_file_name);
			bool TemporaryFileCopy(cstring new_filename, cstring filename = k_temp_cache_file_name);

			void ScenarioNameToCacheFilePath(_Out_ std::string& cache_file_path);
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

		s_build_cache_file_globals* BuildCacheFileGlobals();

		// Is the cache file that is being built in the .yelo format?
		bool BuildCacheFileForYelo();

		bool ScenarioLoadForCacheBuild(cstring scenario_name);
	};

	namespace blam
	{
		int32 build_cache_file_size();

		uint32 build_cache_file_checksum();

		bool build_cache_file_begin(cstring scenario_name,
			byte_flags flags);

		bool build_cache_file_add_resource(const void* buffer, int32 buffer_size,
			int32* return_file_offset = nullptr, bool include_in_crc = true);

		void build_cache_file_cancel();

		bool build_cache_file_end(Cache::s_cache_header& header);

		size_t stream_tag_to_buffer(datum_index tag_index, void* stream, size_t& return_stream_offset, uintptr_t virtual_base_address, tag_reference_name_reference* tag_names);

		bool PLATFORM_API scenario_load_all_structure_bsps();

		void build_cache_file_for_scenario(cstring scenario_path,
			byte_flags begin_flags);
	};
};
#endif