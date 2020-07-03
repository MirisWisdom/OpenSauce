/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo2/memory/data.hpp>

namespace Yelo
{
	namespace Memory
	{
		template<typename DatumT,	size_t MaxDatumCount, 
									size_t MaxDatumCountUpgrade = MaxDatumCount>
		struct DataArray {
		private:
			typedef DataArray<DatumT,MaxDatumCount,MaxDatumCountUpgrade> DataArrayT;

		public:
			struct Iterator {
			private:
				s_data_iterator iterator;

			public:
				Iterator(DataArrayT* data) {
					blam::data_iterator_new(iterator, &data->Header);
				}

				DatumT* Next() {
					return CAST_PTR(DatumT*, blam::data_iterator_next( *CAST_PTR(s_data_iterator*, this) ));
				}

				datum_index Current() const { return this->iterator.index; }
			};

			s_data_array Header;

		public:

			Iterator& IteratorNew(Iterator& iter) {
				return iter = Iterator(this);
			}

			API_INLINE DatumT* Datums() {
				return CAST_PTR(DatumT*, this->Header.data);
			}

			API_INLINE operator s_data_array*() {
				return &this->Header;
			}

			API_INLINE DatumT* operator [](datum_index handle)
			{
#if PLATFORM_IS_EDITOR
				return CAST_PTR(DatumT*, blam::datum_get(&this->Header, handle));
#else
				return &CAST_PTR(DatumT*, this->Header.data)[handle.index];
#endif
			}
		};
	};
};