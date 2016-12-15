/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/memory/data_base.hpp>

namespace Yelo
{
	namespace Memory
	{
		struct s_data_array
		{
			tag_string name;
			int16 max_datum;
			int16 datum_size;
			bool is_valid;
			bool identifier_zero_invalid;
			PAD16;
			tag signature;
			int16 next_index;		// Next index to use when creating a new datum
			int16 last_datum;		// Last used datum index
			datum_index next_datum;	// Next datum value to use
			/// <summary>	Base address of the array's datum. </summary>
			void* base_address;

			datum_index::salt_t GetInitialSalt() const;
			datum_index::salt_t GetNextSalt(datum_index::salt_t cursor) const;

			int16 NumberOfInvalidDatums() const;
			int16 NumberOfValidDatums() const;
		}; BOOST_STATIC_ASSERT(sizeof(s_data_array) == 0x38);

		struct s_data_iterator
		{
			s_data_array* data;
			int16 next_index;
			int16 finished_flag; // actually alignment, unused by the engine
			datum_index index;
			tag signature;

			//////////////////////////////////////////////////////////////////////////
			// stl end() hacks
			enum { k_end_hack_signature = 'hack' };
			inline void SetEndHack()		{ signature = k_end_hack_signature; }
			inline bool IsEndHack() const	{ return signature == k_end_hack_signature; }
			// HACK: don't use this unless the s_data_iterator was created in the OS codebase!
			// engine's iterator_new doesn't initialize 'finished_flag' and we use it for end() hacks
			bool operator!=(const s_data_iterator& other) const;
		}; BOOST_STATIC_ASSERT( sizeof(s_data_iterator) == 0x10 );

		s_data_array* DataNewAndMakeValid(cstring name, int32 maximum_count, size_t datum_size);
	};

	namespace blam
	{
		using namespace Yelo::Memory;

		void PLATFORM_API data_verify(const s_data_array* data);

		s_data_array* PLATFORM_API data_new(cstring name, int32 maximum_count, size_t datum_size);
		template<typename T, const int k_maximum_count>
		s_data_array* data_new(cstring name)
		{
			return data_new(name, k_maximum_count, sizeof(T));
		}

		void PLATFORM_API data_dispose(s_data_array* data);

		void PLATFORM_API data_make_valid(s_data_array* data);

		void PLATFORM_API data_make_invalid(s_data_array* data);

		void PLATFORM_API data_delete_all(s_data_array* data);

		datum_index PLATFORM_API data_next_index(const s_data_array* data, datum_index cursor);

		void PLATFORM_API data_iterator_new(s_data_iterator& iterator, s_data_array* data);

		void* PLATFORM_API data_iterator_next(s_data_iterator& iterator);

		datum_index PLATFORM_API datum_new_at_index(s_data_array* data, datum_index index);

		// creates a new element in [data] and returns the datum index
		datum_index PLATFORM_API datum_new(s_data_array* data);

		// Delete the data associated with the [index] handle in [data]
		void PLATFORM_API datum_delete(s_data_array* data, datum_index index);

		// Get the data associated with [index] from the [data] array
		void* PLATFORM_API datum_get(s_data_array* data, datum_index index);

		// Get the data associated with [index] from the [data] array
		// Returns NULL if the handle is invalid
		void* PLATFORM_API datum_try_and_get(s_data_array* data, datum_index index);
	};
};

#include <YeloLib/Halo1/memory/data_yelo.hpp>