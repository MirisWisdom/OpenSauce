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
		template<typename DatumT, const size_t DatumCount> struct DataArray {
		private:
			typedef DataArray<DatumT,DatumCount> DataArrayT;

		public:
			struct Iterator {
			private:
				DataArrayT* Array;
				datum_index Datum;
				int32 CurrentIndex;

			public:
				Iterator(DataArrayT* array)
				{
					Array = array;
					Datum = datum_index::null;
					CurrentIndex = NONE;
				}

				DatumT* Next() {
					return CAST_PTR(DatumT*, s_data_array::IteratorNext(this));
				}

				datum_index Current() const { return this->Datum; }
			}; BOOST_STATIC_ASSERT( sizeof(Iterator) == sizeof(s_data_array_iterator) );

			s_data_array Header;

		public:

			Iterator& IteratorNew(Iterator& iter) {
				iter = Iterator(this);
			}

			API_INLINE DatumT* operator [](datum_index handle) { return &CAST_PTR(DatumT*, this->Header.data)[handle.index]; }

			API_INLINE operator DatumT*()
			{ 
				return CAST_PTR(DatumT*, this->Header.data);
			}

			API_INLINE size_t GetDatumCount() const { return DatumCount; }
		};
	};
};