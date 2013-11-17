/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/memory/data.hpp>

static const Yelo::datum_index::salt_t k_datum_index_salt_msb = 
	1U << 
	(BIT_COUNT(Yelo::datum_index::salt_t) - 1);

namespace Yelo
{
	namespace Memory
	{
		datum_index::salt_t s_data_array::GetInitialSalt() const
		{
			datum_index::salt_t result = 0;
			// NOTE: engine uses strncpy
			std::memcpy(&result, this->name, sizeof(result));

			result |= k_datum_index_salt_msb;

			return result;
		}
		datum_index::salt_t s_data_array::GetNextSalt(datum_index::salt_t cursor) const
		{
			return ++cursor != 0 
				? cursor
				: k_datum_index_salt_msb;
		}

		static bool DataIteratorIsNotEndHack(const s_data_iterator& iter, int16 last_datum)
		{
			// we treat 'pad' as our "have we already seen the last datum" sentinel
			if (iter.pad == FALSE)
			{
				auto& _iter = CAST_QUAL(s_data_iterator&, iter);
				_iter.pad = _iter.absolute_index == last_datum;
				return true;
			}

			// the last operator!= call matched last_datum, so in this call we're saying we've reached the end
			return false;
		}
		bool s_data_iterator::operator!=(const s_data_iterator& other) const
		{
			auto last_datum = this->data->last_datum;

			if(other.IsEndHack())
				return DataIteratorIsNotEndHack(*this, last_datum);//absolute_index != last_datum;
			else if(this->IsEndHack())
				return DataIteratorIsNotEndHack(other, last_datum);//other.absolute_index != last_datum;

			return this->absolute_index != other.absolute_index;
		}
	};

	namespace blam
	{
		using namespace Yelo::Memory;

#if !PLATFORM_IS_EDITOR
		void PLATFORM_API data_verify(const s_data_array* data)
		{
		}

		void PLATFORM_API data_make_valid(s_data_array* data)
		{
			data_verify(data);
			data->is_valid = true;
			data_delete_all(data);
		}
#endif

		void PLATFORM_API data_make_invalid(s_data_array* data)
		{
			data_verify(data);
			data->is_valid = false;
		}

		void data_iterator_new(s_data_iterator& iterator, s_data_array* data)
		{
			data_verify(data);
			ASSERT(data->is_valid, "invalid data array passed to " __FUNCTION__);

			iterator.data = data;
			iterator.absolute_index = 0;
			iterator.pad = FALSE;
			iterator.index = datum_index::null;
			iterator.signature = CAST_PTR(uintptr_t, data) ^ Enums::k_data_iterator_signature;
		}
	};
};