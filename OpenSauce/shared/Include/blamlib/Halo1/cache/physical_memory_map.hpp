/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#if !defined(PLATFORM_IS_DEDI) || !PLATFORM_IS_DEDI
	#include <blamlib/Halo1/cache/pc_sound_cache.hpp>
	#include <blamlib/Halo1/cache/pc_texture_cache.hpp>
#endif
#include <blamlib/Halo1/memory/data.hpp>
#include <blamlib/Halo1/memory/lruv_cache.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_physical_memory_base_address =		0x40000000,

			// Original allocation size
			k_physical_memory_map_allocation_size = 0x01B40000,
		};
	};

	namespace GameState
	{
		// Made up structure for Halo1
		template<typename DatumT, uint32 DatumCount>
		struct s_resource_cache
		{
			Memory::DataArray<DatumT, DatumCount>* data;
			void* base_address;
			Memory::s_lruv_cache* lruv_cache;
			bool initialized;
			PAD24;
		};

		struct s_physical_memory_map_globals
		{
#if !defined(PLATFORM_IS_DEDI) || !PLATFORM_IS_DEDI
			s_resource_cache<Cache::s_sound_cache_datum, Enums::k_maximum_number_of_cached_sounds> 
				pc_sound_cache;
			s_resource_cache<Cache::s_texture_cache_datum, Enums::k_maximum_number_of_cached_textures> 
				pc_texture_cache;
#endif

			void* game_state_base_address;
			void* tag_cache_base_address;
			void* texture_cache_base_address;
			void* sound_cache_base_address;
		};
	};
};