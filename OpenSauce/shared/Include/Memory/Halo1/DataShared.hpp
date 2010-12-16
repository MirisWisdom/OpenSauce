/*
    Yelo: Open Sauce SDK

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <Memory/DataShared.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_circular_queue_signature = 'circ',
		};
	};

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
			void* data;

			static void* IteratorNext(void* iter);
		};

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


		struct s_circular_queue
		{
			cstring name;
			tag signature;
			int32 read_offset;
			int32 write_offset;
			int32 buffer_size;
			void* buffer;
		};
	};
};