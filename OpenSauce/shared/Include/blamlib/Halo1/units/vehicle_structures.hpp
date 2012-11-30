/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/units/unit_structures.hpp>

namespace Yelo
{
	namespace Enums
	{
	};

	namespace Flags
	{
	};

	namespace Objects
	{
		struct s_vehicle_datum_network_data
		{
			bool at_rest_bit;
			PAD24;
			real_point3d position;
			real_vector3d transitional_velocity;
			real_vector3d angular_velocity;
			real_vector3d forward;
			real_vector3d up;
		}; BOOST_STATIC_ASSERT( sizeof(s_vehicle_datum_network_data) == 0x40 );
	};
};