/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

struct IDirect3DBaseTexture9; // a pc_texture_cache implementation detail, but that's all we support anyway

namespace Yelo
{
	namespace TagGroups
	{
		struct s_bitmap_data;
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