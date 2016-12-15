/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/cseries/debug_memory.hpp>

namespace Yelo
{
	namespace Debug
	{
		struct s_debug_memory_header
		{
			enum {
				k_disposed_signature = '<BAD',
				k_header_signature = '--->',
				k_footer_signature = '<---',
			};

			tag signature;
			s_debug_memory_header* next;
			s_debug_memory_header* previous;
			size_t size;
			cstring file;
			uint32 line;
			int32 marker;
			uint32 checksum;

			static size_t AllocationSize(size_t size)
			{
				return sizeof(s_debug_memory_header) + size + sizeof(tag); // header + size + footer
			}
		}; BOOST_STATIC_ASSERT( sizeof(s_debug_memory_header) == 0x20 );

		struct s_debug_memory_globals
		{
			enum {
				k_signature = 'SAFT',
				k_maximum_files_with_pointers = 512,
			};

			tag header;
			size_t current_heap_size;
			size_t largest_heap_size;
			s_debug_memory_header* heap;
			s_debug_memory_header* first;
			s_debug_memory_header* last;
			int32 next_marker;				// increments with each malloc and realloc, never decrements
			tag footer;
		}; BOOST_STATIC_ASSERT( sizeof(s_debug_memory_globals) == 0x20 );

		void BlamMemoryManagerDispose()
		{
			// TODO: check for possible memory leaks with s_debug_memory_globals
		}
	};
};