/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

#include <blamlib/Halo2/memory/data.hpp>
#include <blamlib/Halo2/memory/lruv_cache.hpp>

namespace Yelo
{
	namespace Cache
	{
		struct s_sound_cache
		{
			Memory::s_data_array* Blocks;
			void* BaseAddress;
			Memory::s_lruv_cache* Cache;
		};

		struct s_texture_cache
		{
			Memory::s_data_array* Blocks;
#if PLATFORM_ID != PLATFORM_H2_ALPHA
			Memory::s_data_array* Predicted;
#endif
			void* Header; // haven't had time to map this one out. used for stealing from the texture cache
			void* BaseAddress;
			Memory::s_lruv_cache* Cache;
		};

		struct s_geometry_cache
		{
			Memory::s_data_array* Blocks;
			Memory::s_data_array* Predicted;
			void* BaseAddress;
#if PLATFORM_ID != PLATFORM_H2_ALPHA
			uint32 Size; // size of the block we have at BaseAddress (without page padding)
#endif
			Memory::s_lruv_cache* Cache;
		};
	};

	namespace GameState
	{
		Cache::s_sound_cache* _XboxSounds();

		Cache::s_texture_cache* _XboxTextures();

		Cache::s_geometry_cache* _XboxGeometry();
	};
};