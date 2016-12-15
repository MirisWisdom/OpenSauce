/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/cache/cache_files.hpp>
#include <blamlib/Halo1/cache/cache_files_structures.hpp>
#include <blamlib/Halo1/cache/data_file_structures.hpp>

#include <zlib/zlib.h>
BOOST_STATIC_ASSERT( sizeof(z_stream) == 0x38 );

namespace Yelo
{
	namespace TagGroups
	{
		struct structure_bsp_header;
	};

	namespace Cache
	{
		struct s_cache_file_request
		{
			OVERLAPPED overlapped;
			uint32 size;
			void* address;
			bool block;
			bool reading;
			bool running;
			PAD8;
			Enums::cache_file_request_source source;
			PAD24;
			s_cache_file_request_params params;
		}; BOOST_STATIC_ASSERT( sizeof(s_cache_file_request) == 0x30 );
		struct s_cached_map_file
		{
			HANDLE file_handle;
			FILETIME file_time;
			s_cache_header header;

			void Close()
			{
				CloseHandle(file_handle);
				file_handle = INVALID_HANDLE_VALUE; // NOT: engine doesn't actually do this
			}
		}; BOOST_STATIC_ASSERT( sizeof(s_cached_map_file) == 0x80C );
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

			s_cached_map_file map_files[Enums::k_number_of_cached_map_files];

			bool copy_in_progress;
			PAD8;
			int16 copy_map_file_index;
			tag_string copy_map_file_name;
			int16 open_map_file_index;
			PAD16;

			HANDLE sleep_event;
			HANDLE thread;
			HGLOBAL requests; // pointer to s_cache_file_request[k_maximum_simultaneous_cache_requests]
			PAD32;

			s_data_file_globals data_files;

			int16 FindMapFileIndexByName(cstring scenario_name);

			s_cached_map_file* OpenMapFile()
			{
				return open_map_file_index != NONE
					? &map_files[open_map_file_index]
					: nullptr;
			}

			bool OpenMapFileOpen(cstring scenario_name, s_cache_header* header);

			void OpenMapFileClose();

			s_cache_file_request* Requests()
			{
				return CAST_PTR(s_cache_file_request*, requests);
			}

			// blocks the thread until all read requests have finished
			void RequestsWaitAll();
		}; BOOST_STATIC_ASSERT( sizeof(s_cache_file_globals) == 0x4418 );
	};
};