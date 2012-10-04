/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <Memory/DataShared.hpp>
#include <Memory/Halo1/DataShared.hpp>

namespace Yelo
{
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
	};
};