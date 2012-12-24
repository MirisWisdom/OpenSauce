/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/cache/cache_files_base.hpp>
#include <blamlib/Halo1/cache/cache_constants.hpp>
#include <blamlib/Halo1/cache/data_file.hpp>
#include <YeloLib/Halo1/cache/cache_files_yelo.hpp>

#include <zlib/zlib.h>
BOOST_STATIC_ASSERT( sizeof(z_stream) == 0x38 );

namespace Yelo
{
	namespace TagGroups
	{
		struct structure_bsp_header;
	};

	namespace Enums
	{
		enum cache_file_request_source : byte_enum {
			_cache_file_request_source_open_map,
			_cache_file_request_source_bitmaps,
			_cache_file_request_source_sounds,
			_cache_file_request_source_loc,
		};
	};

	namespace Cache
	{
		struct s_cache_header : public s_cache_header_base
		{
			enum {
				k_version = 609,

				k_pad_size = 484 * sizeof(int32),
			};

			tag header_signature;

			int32 version;
			int32 size; // file size
			uint32 compressed_file_length;

			uint32 offset_to_index;
			uint32 tag_memory_size;
			PAD32; // ?
			PAD32; // ?
			tag_string name;
			tag_string build_string;
			_enum cache_type;
			PAD16;
			uint32 crc;
			PAD32; // ?

			PAD32; // Align so the yelo header starts on a 16B boundary
			s_cache_header_yelo yelo;
			byte padding[k_pad_size - (sizeof(uint32) + sizeof(s_cache_header_yelo))];

			tag footer_signature;

			API_INLINE bool ValidSignatures() const			{ return header_signature == k_header_signature && footer_signature == k_footer_signature; }
			API_INLINE bool ValidFileSize(int32 max) const	{ return size >= 0 && size <= max; }
			API_INLINE bool ValidName() const				{ return strnlen_s(name, Enums::k_tag_string_length) <= Enums::k_tag_string_length; }
		}; BOOST_STATIC_ASSERT( sizeof(s_cache_header) == 0x800 );

		struct s_cache_tag_instance
		{
			tag group_tag;			// 0x0
			tag parent_groups[2];	// 0x4
			datum_index datum;		// 0xC
			cstring name;			// 0x10
			void* definition;		// 0x14
			BOOL bool_in_data_file;	// 0x18
			UNUSED_TYPE(int32);		// 0x1C

			template<typename T> API_INLINE T* Definition() const { return CAST_PTR(T*, definition); }

			// Is this an instance of a certain tag group?
			// If this instance a child of a certain tag group?
			API_INLINE bool MatchesGroup(tag group_tag) const
			{
				return this->group_tag == group_tag ||
					// test the hierarchy graph
					parent_groups[0] == group_tag ||
					parent_groups[1] == group_tag;
			}
		}; BOOST_STATIC_ASSERT( sizeof(s_cache_tag_instance) == 0x20 );

		struct s_cache_tag_header
		{
			void* tags_address;		// 0x0
			datum_index scenario;	// 0x4
			UNKNOWN_TYPE(int32);	// 0x8
			int32 count;			// 0xC
			struct {
				int32 count;
				int32 offset;
			}vertices, indices;		// 0x10, 0x18
			uint32 model_data_size;	// 0x20
			tag signature;			// 0x24

			// nasty hack assumption
#pragma warning(push)
#pragma warning(disable : 4200)
			s_cache_tag_instance tags[];
#pragma warning(pop)
		}; BOOST_STATIC_ASSERT( sizeof(s_cache_tag_header) == 0x28 );


		struct s_cache_file_request_params
		{
			typedef void(PLATFORM_API* proc_finished)(void* params);

			bool* finished_flag;		// NOT optional
			void* finished_proc_params;	// optional
			proc_finished finished_proc;// optional
		}; BOOST_STATIC_ASSERT( sizeof(s_cache_file_request_params) == 0xC );

		struct s_cache_file_request
		{
			OVERLAPPED overlapped;
			uint32 size;
			void* address;
			bool block;
			UNKNOWN_TYPE(bool); // initialized?
			bool running;
			PAD8;
			Enums::cache_file_request_source source;
			PAD24;
			s_cache_file_request_params params;
		}; BOOST_STATIC_ASSERT( sizeof(s_cache_file_request) == 0x30 );
		struct s_cache_file_globals
		{
			// reversed engineered based on halo xbox code...PC code seems to have some slight differences, but it doesn't really use this anyway so who cares
			struct s_decompression_state
			{
				struct s_read_request { int16 read_sequence_index; };
				struct s_write_request { int16 write_sequence_index; };

				char file_name[_MAX_PATH];
				s_cache_header header;
				long_flags flags;
				z_stream zstream;
				void* zlib_buffer;
				uint32 zlib_buffer_size;
				void* next_allocation;
				HANDLE	copy_start_event, 
						copy_stop_event,
						copy_complete_event,
						progress_update_event;
				HANDLE copy_thread;
				void* buffer;
				void* read_buffers[Enums::k_number_of_cache_read_buffers];
				void* write_buffers[Enums::k_number_of_cache_write_buffers];
				UNKNOWN_TYPE(bool); PAD24;
				HANDLE write_file_handle, read_file_handle;
				long_flags overlapped_in_use_flags[BIT_VECTOR_SIZE_IN_DWORDS(Enums::k_number_of_cache_overlapped_structures)];
				long_flags overlapped_completed_flags[BIT_VECTOR_SIZE_IN_DWORDS(Enums::k_number_of_cache_overlapped_structures)];
				OVERLAPPED overlapped[Enums::k_number_of_cache_overlapped_structures];
				s_read_request read_requests[Enums::k_number_of_cache_read_buffers];
				s_write_request write_requests[Enums::k_number_of_cache_write_buffers];
				PAD16;
				uint32 read_file_size;
				uint32 async_read_bytes_left, read_bytes_left;
				uint32 async_write_bytes_left, write_bytes_left;
				real_fraction progress;
				int32 current_write_offset, current_read_offset;
				s_read_request* current_request;
				s_write_request* current_write_request;
				int32 write_requests_pending;
				UNKNOWN_TYPE(int16); // a read_sequence_index
				UNKNOWN_TYPE(int16); // a read_sequence_index
				int16 current_write_buffer_index;
				UNKNOWN_TYPE(int16); // current_write_sequence_count?
				int16 next_write_sequence_index;
				int16 current_read_sequence_count;
				PAD16; // more than likely an unused next_read_sequence_index
				PAD16;
			}; BOOST_STATIC_ASSERT( sizeof(s_decompression_state) == 0xAC8 );

			bool tags_loaded; PAD24;
			s_cache_header cache_header;
			s_cache_tag_header* tag_index;
			TagGroups::structure_bsp_header* structure_bsp_header;
			PAD32;
			s_decompression_state decompression_state;

			struct {
				struct {
					HANDLE file_handle;
					FILETIME time;
				}runtime;
				s_cache_header header;
			}map_files[Enums::k_number_of_cached_map_files];

			bool copy_in_progress;
			PAD8;
			int16 copy_map_file_index;
			tag_string copy_map_file_name;
			int16 open_map_file_index;
			PAD16;

			HANDLE sleep_event;
			HANDLE thread;
			HGLOBAL requests; // pointer to s_request[k_maximum_simultaneous_cache_requests]
			PAD32;

			s_data_file data_files[Enums::k_number_of_data_file_types];
		}; BOOST_STATIC_ASSERT( sizeof(s_cache_file_globals) == 0x4418 );
	};
};