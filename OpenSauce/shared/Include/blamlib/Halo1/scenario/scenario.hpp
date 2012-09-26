/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Game
	{
		struct s_scenario_globals
		{
			int16 current_structure_bsp_index;
			PAD16;
			byte UNKNOWN(0)[0x2C][1]; // only one element on pc, so related to each local player then something about the fog around them?
			bool UNKNOWN(1);
			PAD24;
			byte sound_environment[0x48]; // if I gave a fuck about defining the tag structure, I'd use it here
		}; BOOST_STATIC_ASSERT( sizeof(s_scenario_globals) == 0x7C );
	};
};