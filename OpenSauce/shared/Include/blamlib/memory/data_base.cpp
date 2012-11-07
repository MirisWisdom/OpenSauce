/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/memory/data_base.hpp>

#include <blamlib/memory/datum_index.hpp>

namespace Yelo
{
	const datum_index datum_index::null = { CAST(uint32, NONE) };

	datum_index datum_index::Create(index_t index, salt_t salt)
	{
		datum_index result; result.handle = (CAST(uint32,salt) << 16) | index;
		return result;
	}

	datum_index datum_index::Create(index_t index, void* header)
	{
		if(header == NULL) return null;

		return Create(index, *CAST_PTR(salt_t*, header));
	}
};