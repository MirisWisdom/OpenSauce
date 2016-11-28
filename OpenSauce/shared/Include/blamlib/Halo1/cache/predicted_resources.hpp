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
		enum predicted_resource : _enum {
			_predicted_resource_bitmap,
			_predicted_resource_sound,

			k_number_of_predicted_resources
		};
	};

	namespace TagGroups
	{
		struct predicted_resource
		{
			_enum type;
			int16 resource_index;
			datum_index tag_index;
		}; BOOST_STATIC_ASSERT( sizeof(predicted_resource) == 0x8 ); // max count: 1024
	};

	namespace blam
	{
#if PLATFORM_TYPE == PLATFORM_TOOL
		void predicted_resources_add_resource(TagBlock<TagGroups::predicted_resource>& predicted_resources,
			long_enum resource_type, datum_index tag_index, int32 resource_index = NONE);
#endif

		// NOTE: function is actually defined in the engine's physical_memory_map.c
		// May be 'physical_memory_map_precache_resources' or something
		void predicted_resources_precache(TagBlock<TagGroups::predicted_resource>& resources);
	};
};