/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/memory/data_base.hpp>

namespace Yelo
{
	namespace Flags
	{
		enum data_array
		{
			_data_array_can_disconnect_bit,
			_data_array_disconnected_bit,

			_data_array_allocation_bit,
			_data_array,

			_data_array_can_disconnect_flag =	FLAG(_data_array_can_disconnect_bit),
			_data_array_disconnected_flag =		FLAG(_data_array_disconnected_bit),

			_data_array_allocation_flag =	FLAG(_data_array_allocation_bit),
		};
	};

	namespace Memory
	{
		struct s_data_array {
			tag_string name;		// Name of the data array
			int32 maximum_count;	// How many datums this array can hold
			uint32 datum_size;		// Size of one datum
			byte alignment_bit;
			bool is_valid;			// True if this array is usable
			word_flags flags;
			tag signature;			// 'd@t@'
			void** allocator;		// allocation object
			struct {
				int32 next_index;	// Next available bit index in the bit vector that can be used
				int32 length;		// Current length (in bits) of the bit index
			}active_indices;
			int32 actual_count;		// Number of datums in use
			datum_index next_datum;
			void* data;
			byte* active_indices_bit_vector;

			inline int32 GetLength() const { return actual_count; }
		}; BOOST_STATIC_ASSERT( sizeof(s_data_array) == 0x4C );

		struct s_data_iterator
		{
			s_data_array* data;
			datum_index index;
			int32 absolute_index;
			tag signature;
		}; BOOST_STATIC_ASSERT( sizeof(s_data_iterator) == 0x10 );
	};

	namespace blam
	{
		using namespace Yelo::Memory;

		void data_iterator_new(s_data_iterator& iterator, s_data_array* data);

		void* data_iterator_next(s_data_iterator& iterator);

		void* datum_get(s_data_array* data, datum_index index);
	};
};

#include <YeloLib/Halo2/memory/data_yelo.hpp>