/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/memory/data.hpp>

struct IDirect3DBaseTexture9;

namespace Yelo
{
	namespace TagGroups
	{
		struct bitmap_data;
	};

	namespace Enums
	{
		enum {
			k_maximum_number_of_cached_textures = 4096,
		};
	};

	namespace Cache
	{
		struct s_texture_cache_datum : Memory::s_datum_base
		{
			int16 cache_read_request_index;
			bool finished_loading_flag;
			UNKNOWN_TYPE(bool);
			PAD16;
			TagGroups::bitmap_data* bitmap;
			IDirect3DBaseTexture9* hardware_format; // the address of this field is returned by the texture request function
		}; BOOST_STATIC_ASSERT( sizeof(s_texture_cache_datum) == 0x10 );
	};
};