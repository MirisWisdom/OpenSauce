/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Pointers/1.10/HaloCE_110_Runtime.Rasterizer.DX9.inl"
#include "Pointers/1.10/HaloCE_110_Runtime_Manual.Rasterizer.DX9.inl"
#endif

#include <Memory/MemoryInterface.hpp>
#include <Rasterizer/DX9/rasterizer_dx9_shaders_vshader9.hpp>
#include <yelo/memory/memory_interface_base.hpp>

namespace Yelo
{
	namespace DX9
	{
		ENGINE_PTR(s_rasterizer_dx9_effect_collection, rasterizer_dx9_effect_collection, K_RASTERIZER_DX9_EFFECT_COLLECTION, PTR_NULL);
		ENGINE_PTR(s_effect_shader_entry, rasterizer_effect_shaders, K_RASTERIZER_EFFECT_SHADERS, PTR_NULL);
		ENGINE_PTR(s_vsf_entry, vsf_table, K_VSF_TABLE, PTR_NULL);
	}
}
