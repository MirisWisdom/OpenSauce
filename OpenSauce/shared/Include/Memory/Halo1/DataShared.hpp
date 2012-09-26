/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <Memory/DataShared.hpp>

#include <blamlib/Halo1/game/simple_circular_queue.hpp>
#include <blamlib/Halo1/memory/array.hpp>
#include <blamlib/Halo1/memory/circular_queue.hpp>
#include <blamlib/Halo1/memory/data.hpp>
#include <blamlib/Halo1/memory/lruv_cache.hpp>
#include <blamlib/Halo1/memory/memory_pool.hpp>

namespace Yelo
{
	namespace Memory
	{
		template<typename DatumT, uint32 DatumCount> struct DataArray {
		private:
			typedef DataArray<DatumT,DatumCount> DataArrayT;

		public:
			struct Iterator {
			private:
				DataArrayT* Array;
				int16 CurrentIndex;
				PAD16;
				datum_index Datum;
				tag Signature;

			public:
				Iterator(DataArrayT* array)
				{
					Array = array;
					CurrentIndex = 0;
					Datum = datum_index::null;
					Signature = CAST_PTR(uint32, this) ^ Enums::k_data_iterator_signature;
				}

				DatumT* Next() {
					return CAST_PTR(DatumT*, s_data_array::IteratorNext(this));
				}

				datum_index Current() const { return this->Datum; }
			};

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

//			API_INLINE DatumT* Datums()					{ return CAST_PTR(DatumT*, this->Header.data); }
		};
	};
};