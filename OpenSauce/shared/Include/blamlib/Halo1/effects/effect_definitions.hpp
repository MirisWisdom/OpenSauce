/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_effect_locations_count = 32,

			k_maximum_effect_particles_per_event_count = 32,
		};
	};

	namespace TagGroups
	{
		struct effect_definition
		{
			enum { k_group_tag = 'effe' };

			TAG_FIELD(long_flags, flags);
			TAG_FIELD(int16, loop_start_event_index, effect_event_definition);
			TAG_FIELD(int16, loop_stop_event_index, effect_event_definition);
			UNKNOWN_TYPE(real); // runtime field
			TAG_PAD(int32, 7);
			TAG_BLOCK(locations, effect_location_definition);
			TAG_BLOCK(events, effect_event_definition);
		}; BOOST_STATIC_ASSERT( sizeof(effect_definition) == 0x40 );
	};
};