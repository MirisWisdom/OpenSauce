/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/objects/object_structures.hpp>

namespace Yelo
{
	namespace Objects
	{
		void s_object_network_datum_data::CopyToPlacementData(s_object_placement_data& data) const
		{
			data.player_index = owner_player_index;
			data.owner_object_index = owner;
			memcpy(&data.position,				&position, sizeof(position));
			memcpy(&data.transitional_velocity,	&transitional_velocity, sizeof(transitional_velocity));
			memcpy(&data.forward,				&forward, sizeof(forward));
			memcpy(&data.up,					&up, sizeof(up));
			memcpy(&data.angular_velocity,		&angular_velocity, sizeof(angular_velocity));
		}

		bool s_object_data::VerifyType(long_flags type_mask) const
		{
			return TEST_FLAG( type_mask, type );
		}
	};
};