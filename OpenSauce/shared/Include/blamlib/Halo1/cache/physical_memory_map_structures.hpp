/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/cache/cache_constants.hpp>
#include <blamlib/Halo1/memory/data.hpp>

namespace Yelo
{
	namespace Cache
	{
		struct s_sound_cache_datum;
		struct s_texture_cache_datum;
	};
	namespace Memory
	{
		struct s_lruv_cache;
	};

	namespace GameState
	{
		// Made up structure for Halo1
		template<typename DatumT, size_t MaxDatumCount, size_t MaxDatumCountUpgrade = MaxDatumCount>
		struct s_resource_cache
		{
			Memory::DataArray<DatumT, MaxDatumCount, MaxDatumCountUpgrade>* data;
			void* base_address;
			Memory::s_lruv_cache* lruv_cache;
			bool initialized;
			PAD24;
		};

		struct s_physical_memory_map_globals
		{
#if !defined(PLATFORM_IS_DEDI) || !PLATFORM_IS_DEDI
			s_resource_cache<	Cache::s_sound_cache_datum, 
								Enums::k_maximum_number_of_cached_sounds> 
				pc_sound_cache;
			s_resource_cache<
								Cache::s_texture_cache_datum, 
								Enums::k_maximum_number_of_cached_textures> 
				pc_texture_cache;
#endif

			void* game_state_base_address;
			void* tag_cache_base_address;
			void* texture_cache_base_address;
			void* sound_cache_base_address;
		};
	};
};