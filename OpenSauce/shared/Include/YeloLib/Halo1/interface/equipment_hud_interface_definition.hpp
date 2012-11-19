/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/interface/hud_definitions.hpp>

#include <blamlib/Halo1/tag_files/tag_groups.hpp>

namespace Yelo
{
	namespace Enums
	{
	};

	namespace Flags
	{
	};

	namespace TagGroups
	{
		struct equipment_hud_interface_definition
		{
			enum { k_group_tag = 'eqhi' };
		}; //BOOST_STATIC_ASSERT( sizeof(equipment_hud_interface_definition) == 0x );
	};
};