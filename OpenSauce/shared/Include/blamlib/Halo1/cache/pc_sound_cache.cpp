/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/cache/sound_cache.hpp>

#include <blamlib/Halo1/cache/pc_sound_cache.hpp>
#include <blamlib/Halo1/cache/physical_memory_map.hpp>
#include <blamlib/Halo1/sound/sound_definitions.hpp>

namespace Yelo
{
	namespace blam
	{
		void PLATFORM_API sound_cache_open()
		{
#if PLATFORM_USES_CACHE_FILES && !PLATFORM_IS_DEDI
			YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");
#else
			// do nothing
#endif
		}

		void PLATFORM_API sound_cache_close()
		{
#if PLATFORM_USES_CACHE_FILES && !PLATFORM_IS_DEDI
			YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");
#else
			// do nothing
#endif
		}

		bool PLATFORM_API sound_cache_sound_request_impl(TagGroups::s_sound_permutation* sound,
			bool block_thread, bool load, bool reference)
		{
			bool success = false;

#if PLATFORM_USES_CACHE_FILES && !PLATFORM_IS_DEDI
			YELO_ASSERT(load || !block_thread);
			YELO_ASSERT(load || !reference);
			YELO_ASSERT(sound->cache_tag_index.handle != 0); // yes, the engine checks against 0, which is why we have to use .handle

			if (sound->cache_block_index.IsNull() && load)
			{
				YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");
			}

			if (sound->cache_block_index.IsNull())
				return false;

			YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");
#else
			// do nothing
#endif
			return success;
		}

		void PLATFORM_API sound_cache_sound_finished(TagGroups::s_sound_permutation* sound)
		{
#if PLATFORM_USES_CACHE_FILES && !PLATFORM_IS_DEDI
			YELO_ASSERT_DISPLAY(false, "this isn't implemented yet");
#else
			// do nothing
#endif
		}
	};
};