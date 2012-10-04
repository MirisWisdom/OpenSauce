/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
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