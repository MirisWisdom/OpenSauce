/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	// The engine uses this for things (objects, ai, etc) which get dis/connected to the scenario's bsp
	struct s_scenario_location
	{
		int32 leaf_index;
		int16 cluster_index;
		UNKNOWN_TYPE(int16);
	}; BOOST_STATIC_ASSERT( sizeof(s_scenario_location) == 0x8 );
};