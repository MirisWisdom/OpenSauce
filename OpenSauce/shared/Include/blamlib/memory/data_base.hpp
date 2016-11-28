/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
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
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	for data structures which DO use the proceeding 16 bits after the 'salt' header.
		/// </summary>
		struct s_datum_base
		{
		private:
			datum_index::salt_t header; // salt for this datum
		public:
			datum_index::salt_t GetHeader() const	{ return header; }
			bool IsNull() const						{ return header == 0; }
		}; BOOST_STATIC_ASSERT( sizeof(s_datum_base) == 2 );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	for data structures which DON'T make use of the proceeding 16 bits after the 'salt'
		/// 	header.
		/// </summary>
		struct s_datum_base_aligned : s_datum_base
		{
		private:
			PAD16;
		}; BOOST_STATIC_ASSERT( sizeof(s_datum_base_aligned) == 4 );
	};
};