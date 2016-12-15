/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/memory/data.hpp>

namespace Yelo
{
	namespace Scripting
	{
		struct s_recorded_animations_datum;

		typedef Memory::DataArray<s_recorded_animations_datum, 64>
			recorded_animations_data_t;
	};
};