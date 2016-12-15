/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum predicted_resource : _enum {
			_predicted_resource_bitmap,
			_predicted_resource_sound,

			// TODO: Halo 2 defines more types of predicted resources (tho, not all of them are actually put to use)

			_predicted_resource
		};
	};

	namespace TagGroups
	{
		struct predicted_resource
		{
			_enum type;
			int16 resource_index;
			datum_index tag_index;
		}; BOOST_STATIC_ASSERT( sizeof(predicted_resource) == 0x8 );
	};
};