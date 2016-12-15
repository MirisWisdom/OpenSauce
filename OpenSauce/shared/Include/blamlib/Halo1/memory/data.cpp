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

		int16 s_data_array::NumberOfInvalidDatums() const
		{
			assert(base_address);

			auto* datum_address = CAST_PTR(const byte*, base_address);
			int16 invalid_count = 0;

			for (int x = 0, max_count = max_datum; x < max_count; x++, datum_address += datum_size)
				if (CAST_PTR(const s_datum_base*, datum_address)->IsNull())
					invalid_count++;

			return invalid_count;
		}
		int16 s_data_array::NumberOfValidDatums() const
		{
			return max_datum - NumberOfInvalidDatums();
		}

		bool s_data_iterator::operator!=(const s_data_iterator& other) const
		{
			auto last_datum = this->data->last_datum;

			if (other.IsEndHack())
				return !this->finished_flag;
			else if (this->IsEndHack())
				return !other.finished_flag;

			return this->index != other.index;
		}

		s_data_array* DataNewAndMakeValid(cstring name, int32 maximum_count, size_t datum_size)
		{
			s_data_array* data = blam::data_new(name, maximum_count, datum_size);

			if (data != nullptr)
			{
				data->is_valid = true;
				blam::data_delete_all(data);
			}

			return data;
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

		void PLATFORM_API data_iterator_new(s_data_iterator& iterator, s_data_array* data)
		{
			data_verify(data);
			YELO_ASSERT_DISPLAY(data->is_valid, "invalid data array passed to " __FUNCTION__);

			iterator.data = data;
			iterator.next_index = 0;
			iterator.finished_flag = FALSE;
			iterator.index = datum_index::null;
			iterator.signature = CAST_PTR(uintptr_t, data) ^ Enums::k_data_iterator_signature;
		}

		void* PLATFORM_API data_iterator_next(s_data_iterator& iterator)
		{
			YELO_ASSERT_DISPLAY(iterator.signature == (CAST_PTR(uintptr_t, iterator.data) ^ Enums::k_data_iterator_signature),
				"uninitialized iterator passed to " __FUNCTION__);

			const s_data_array* data = iterator.data;
			data_verify(data);

			YELO_ASSERT_DISPLAY(data->is_valid, "tried to iterate %s when it was in an invalid state",
				data->name);

			datum_index::index_t absolute_index = iterator.next_index;
			int32 datum_size = data->datum_size;
			byte* pointer = CAST_PTR(byte*, data->base_address) + (datum_size * absolute_index);

			for (int16 last_datum = data->last_datum; absolute_index < last_datum; pointer += datum_size, absolute_index++)
			{
				auto datum = CAST_PTR(const s_datum_base*, pointer);

				if (!datum->IsNull())
				{
					iterator.next_index = absolute_index+1;
					iterator.index = datum_index::Create(absolute_index, datum->GetHeader());
					return pointer;
				}
			}
			iterator.next_index = absolute_index; // will equal last_datum at this point
			iterator.finished_flag = TRUE;
			iterator.index = datum_index::null;
			return nullptr;
		}
	};
};