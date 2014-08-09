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
		struct s_bitmap_data;
	};

	namespace Cache
	{
		struct s_texture_cache_datum : Memory::s_datum_base
		{
			int16 cache_read_request_index;
			bool finished_loading_flag;
			bool valid; // set to true when finished loading and the texture request populates hardware_format
			PAD16;
			TagGroups::s_bitmap_data* bitmap;
			IDirect3DBaseTexture9* hardware_format; // the address of this field is returned by the texture request function
		}; BOOST_STATIC_ASSERT( sizeof(s_texture_cache_datum) == 0x10 );
	};

	namespace blam
	{
		void PLATFORM_API texture_cache_open();
		void PLATFORM_API texture_cache_close();

		// Setting [block_thread] to true causes the engine to immediately create the rasterizer (D3D) data for it
		IDirect3DBaseTexture9** PLATFORM_API texture_cache_bitmap_get_hardware_format(TagGroups::s_bitmap_data* bitmap, 
			// block
			bool block_thread = false, bool load = true);
	};
};