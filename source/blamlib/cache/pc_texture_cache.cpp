/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/cache/texture_cache.hpp>

#include <blamlib/bitmaps/bitmap_definition.hpp>
#include <blamlib/cache/pc_texture_cache.hpp>
#include <blamlib/cache/physical_memory_map.hpp>

namespace Yelo
{
	namespace blam
	{
		using namespace TagGroups;

		void PLATFORM_API texture_cache_open()
		{
#if PLATFORM_USES_CACHE_FILES && !PLATFORM_IS_DEDI
			YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");
#else
			// do nothing
#endif
		}

		void PLATFORM_API texture_cache_close()
		{
#if PLATFORM_USES_CACHE_FILES && !PLATFORM_IS_DEDI
			YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");
#else
			// do nothing
#endif
		}

		IDirect3DBaseTexture9** PLATFORM_API texture_cache_bitmap_get_hardware_format_impl(TagGroups::s_bitmap_data* bitmap, 
			// block
			bool block_thread, bool load)
		{
			YELO_ASSERT(bitmap && bitmap->signature== s_bitmap_definition::k_group_tag);

#if PLATFORM_USES_CACHE_FILES && !PLATFORM_IS_DEDI
			YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");
#else
			// do nothing
#endif

			return CAST_PTR(IDirect3DBaseTexture9**, bitmap->hardware_format);
		}
	}
}