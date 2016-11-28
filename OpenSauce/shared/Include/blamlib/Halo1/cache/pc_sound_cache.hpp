/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/memory/data.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct s_sound_permutation;
	};

	namespace Cache
	{
		struct s_sound_cache_datum : Memory::s_datum_base
		{
			bool finished_loading_flag;					// 0x2
			UNKNOWN_TYPE(bool);							// 0x3 true when the sound contains decompressed xbox adpcm samples
			UNKNOWN_TYPE(byte);							// 0x4 haven't seen this used, don't know the exact type
			sbyte software_reference_count;				// 0x5
			sbyte hardware_reference_count;				// 0x6
			UNKNOWN_TYPE(byte);							// 0x7 haven't seen this used, don't know the exact type
			int16 cache_read_request_index;
			PAD16;
			TagGroups::s_sound_permutation* sound;
		}; BOOST_STATIC_ASSERT( sizeof(s_sound_cache_datum) == 0x10 );
	};
};