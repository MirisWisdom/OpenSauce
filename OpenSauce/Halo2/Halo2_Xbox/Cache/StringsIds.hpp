/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

namespace Yelo
{
	namespace StringId
	{
		enum string_id {
			_null												 = 0x00000000, // _string_id_empty_string
#if PLATFORM_ID == PLATFORM_H2_ALPHA
			#include "Cache/StringIds_Alpha.inl"
#else
			#include "Cache/StringIds.inl"
#endif
			_string_id,
			k_string_id_invalid = NONE,
		};
	};
};