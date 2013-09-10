/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/memory/data.hpp>

namespace Yelo
{
	namespace Memory
	{
		bool s_data_iterator::operator!=(const s_data_iterator& other) const
		{
			if(other.IsEndHack())
				return !index.IsNull();
			else if(this->IsEndHack())
				return !other.index.IsNull();

			return this->index != other.index;
		}
	};

	namespace blam
	{
		using namespace Yelo::Memory;

#if !PLATFORM_IS_EDITOR
		void PLATFORM_API data_verify(s_data_array* data)
		{
		}

		void PLATFORM_API data_make_valid(s_data_array* data)
		{
			data_verify(data);
			data->is_valid = true;
			data_delete_all(data);
		}
#endif

		void data_iterator_new(s_data_iterator& iterator, s_data_array* data)
		{
			data_verify(data);

			iterator.data = data;
			iterator.signature = CAST_PTR(uintptr_t, data) ^ Enums::k_data_iterator_signature;
			iterator.absolute_index = 0;
			iterator.index = datum_index::null;
		}
	};
};