/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/objects/object_structures.hpp>
#include <blamlib/items/item_structures.hpp>

#include <blamlib/items/equipment_definitions.hpp>

namespace Yelo
{
	namespace Objects
	{
		struct s_equipment_datum_yelo_network_data
		{
		};
		struct s_equipment_data_yelo
		{
			enum { k_max_size = 0x18 }; // size of the unknown/useless data in s_equipment_data

		}; BOOST_STATIC_ASSERT( sizeof(s_equipment_data_yelo) <= s_equipment_data_yelo::k_max_size );
	};
};