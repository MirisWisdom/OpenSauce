/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/memory/data.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct bitmap_data;
	};

	namespace Enums
	{
		enum {
			k_rasterizer_maximum_texture_stages = 4,
		};
	};

	namespace blam
	{
		bool PLATFORM_API rasterizer_set_texture_bitmap_data(_enum stage, TagGroups::s_bitmap_data* bitmap);
	};
};