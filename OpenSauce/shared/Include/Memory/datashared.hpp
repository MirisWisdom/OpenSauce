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

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_data_signature = 'd@t@',
			k_data_iterator_signature = 'iter',

			k_lruv_cache_signature = 'weee',

			k_memory_pool_signature = 'pool',
			k_block_header_signature = 'head',
			k_block_trailer_signature = 'tail',
		};
	};

	namespace Memory
	{
		struct s_dynamic_array
		{
			size_t element_size;
			int32 count;
			void* elements;
		}; BOOST_STATIC_ASSERT( sizeof(s_dynamic_array) == 0xC );


		// for data structures which DO use the proceeding 16 bits after the 'salt' header
		struct s_datum_base
		{
		private:
			int16 header; // salt for this datum
		public:
			inline int16 GetHeader() const { return header; }
			inline bool IsNull() { return header == NONE; }
		}; BOOST_STATIC_ASSERT( sizeof(s_datum_base) == 2 );

		// for data structures which DON'T make use of the proceeding 16 bits after the 'salt' header
		struct s_datum_base_aligned : s_datum_base
		{
		private:
			PAD16;
		}; BOOST_STATIC_ASSERT( sizeof(s_datum_base_aligned) == 4 );
	};
};