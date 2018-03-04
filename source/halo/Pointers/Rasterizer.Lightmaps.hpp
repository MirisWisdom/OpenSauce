/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Pointers/1.10/HaloCE_110_Runtime.Rasterizer.inl"
#include "Pointers/1.10/HaloCE_110_Runtime_Manual.Rasterizer.inl"
#endif

#include <Memory/MemoryInterface.hpp>
#include <yelolib/memory/memory_interface_base.hpp>

namespace Yelo
{
	namespace Render
	{
		namespace Lightmaps
		{
			FUNC_PTR(BSP_LIGHTMAP_INDEX_HOOK, K_BSP_LIGHTMAP_INDEX_HOOK, FUNC_PTR_NULL);
			FUNC_PTR(BSP_LIGHTMAP_INDEX_RETN, K_BSP_LIGHTMAP_INDEX_RETN, FUNC_PTR_NULL);
			FUNC_PTR(SET_LIGHTMAP_SAMPLER_CALL, K_SET_LIGHTMAP_SAMPLER_CALL, FUNC_PTR_NULL);
			FUNC_PTR(OBJECT_RENDER_CACHE_LIGHTMAP_TAG_INDEX_HOOK, K_OBJECT_RENDER_CACHE_LIGHTMAP_TAG_INDEX_HOOK, FUNC_PTR_NULL);
			FUNC_PTR(OBJECT_RENDER_CACHE_LIGHTMAP_TAG_INDEX_RETN, K_OBJECT_RENDER_CACHE_LIGHTMAP_TAG_INDEX_RETN, FUNC_PTR_NULL);
		}
	}
}
