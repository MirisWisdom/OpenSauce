/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/cache/cache_files_base.hpp>
#include <blamlib/Halo2/memory/hashtable.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_number_of_relative_tag_instances = 10000,
		};
	};

	namespace Cache
	{
		struct s_cache_header : public s_cache_header_base
		{
			enum {
				k_version = 8,

				k_pad_size = 321 * sizeof(int32),
			};

			tag header_signature;

			int32 version;
			uint32 file_length;
			uint32 compressed_file_length;

			uint32 offset_to_index;
			uint32 index_stream_size;
			uint32 tag_buffer_size;
			uint32 total_stream_size;
			uint32 virtual_base_address;

			struct _tag_dependency_graph {
				uint32 offset;
				uint32 size;
			}tag_dependency_graph;

			long_string source_file;
			tag_string build;
			long_enum cache_type;
			uint32 crc;

			PAD32;
			PAD32; PAD32;
			PAD32; // count of some sort? only on the xbox
			PAD32;
			PAD32;

			struct _debug_string_id {
				PAD32;
				uint32 count;
				uint32 buffer_size;
				uint32 indices_offset;
				uint32 buffer_offset;
			}string_ids;

			PAD8; PAD8; PAD8; PAD8; // 4 bools

			FILETIME cache_build_dates[4];

			tag_string cache_name;
			PAD32;
			long_string scenario_name;
			uint32 minor_version;

			struct _debug_tag_names {
				uint32 count;
				uint32 buffer_offset;
				uint32 buffer_size;
				uint32 indices_offset;
			}tag_names;

			struct _language_pack {
				uint32 offset;
				uint32 size;
			}language_pack;

			datum_index secondary_sound_gestalt_index;

			struct _fast_geometry_load_region {
				uint32 cache_block_offset;
				uint32 cache_block_size;
			}fast_geometry_load_region;

			uint32 checksum;
			uint32 mopp_code_checksum;

			byte padding[k_pad_size /*- sizeof(s_cache_header_yelo)*/];

			tag footer_signature;
		}; BOOST_STATIC_ASSERT( sizeof(s_cache_header) == 0x800 );

		struct s_cache_tag_instance
		{
			tag group_tag;
			datum_index handle;
			void* address;
			uint32 size;

			template<typename T> API_INLINE T* Definition() const { return CAST_PTR(T*, definition); }

		}; BOOST_STATIC_ASSERT( sizeof(s_cache_tag_instance) == 0x10 );
		struct s_cache_tag_header
		{
			struct _debug_group_tags {
				void* address;
				int32 count;
			}group_tags;
			s_cache_tag_instance* instances;
			datum_index scenario_index;
			datum_index globals_index;
			uint32 checksum;
			int32 tag_count;
			tag signature;
		}; BOOST_STATIC_ASSERT( sizeof(s_cache_tag_header) == 0x20 );


		struct s_cache_file_resource_database_item {
			void* owner;
			uint32 size;
			void* data;
			uint32 hash;
			void* resource_ptr;
		}; BOOST_STATIC_ASSERT( sizeof(s_cache_file_resource_database_item) == 0x14 );
		struct s_cache_file_resource_database {
			bool initialized;
			tag_string name;
			PAD24;
			Memory::s_hash_table* hash_table;
			PAD32;
			int32 maximum_count;
			s_cache_file_resource_database_item* items;
			FILE* file_handle;
		}; BOOST_STATIC_ASSERT( sizeof(s_cache_file_resource_database) == 0x38 );


		struct s_shared_tag_index_entry {
			tag group_tag;
			long_string name;
			datum_index tag_index;
		}; BOOST_STATIC_ASSERT( sizeof(s_shared_tag_index_entry) == 0x108 );
		struct s_shared_tag_index {
			s_shared_tag_index_entry entries[Enums::k_number_of_relative_tag_instances];
			int32 count;
		}; BOOST_STATIC_ASSERT( sizeof(s_shared_tag_index) == sizeof(int32) + (sizeof(s_shared_tag_index_entry)*Enums::k_number_of_relative_tag_instances) );

		// Dependency range description for a single shared tag
		struct s_shared_dependency_graph_entry {
			int32 start; // Index of the first dependency of this tag
			int32 count; // How many tag dependencies this tag has

			API_INLINE bool IsNull() const { return start == NONE; }
			API_INLINE void Nullify()
			{
				start = NONE;
				count = 0;
			}
		};
		struct s_shared_dependency_graph_buffer {
			int32 count;

			// nasty hack gagger
#pragma warning(push)
#pragma warning(disable : 4200)
			s_shared_dependency_graph_entry entries[];
#pragma warning(pop)
		};
		struct s_shared_dependency_graph {
			s_shared_dependency_graph_buffer* buffer;
			// For each shared tag (any tag indexed at 0x2710 or higher) there is an entry in this graph 
			// which gives the range values in [dependencies] which define its immediate tag references
			s_shared_dependency_graph_entry* entries;
			datum_index* dependencies;
		}; BOOST_STATIC_ASSERT( sizeof(s_shared_dependency_graph) == 0xC );
	};
};