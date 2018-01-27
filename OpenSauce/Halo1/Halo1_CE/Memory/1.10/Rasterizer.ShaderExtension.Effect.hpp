/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Memory/1.10/Pointers/HaloCE_110_Runtime.Rasterizer.ShaderExtension.inl"
#include "Memory/1.10/Pointers/HaloCE_110_Runtime_Manual.Rasterizer.ShaderExtension.inl"
#endif

#include <YeloLib/memory/memory_interface_base.hpp>
#include <Memory/MemoryInterface.hpp>

namespace Yelo
{
	namespace Rasterizer
	{
		namespace ShaderExtension
		{
			namespace Effect
			{
				FUNC_PTR(RASTERIZER_EFFECT_PARTICLE_SYSTEM_HOOK, K_RASTERIZER_EFFECT_PARTICLE_SYSTEM_HOOK, FUNC_PTR);
				FUNC_PTR(RASTERIZER_EFFECT_PARTICLE_SYSTEM_RETN, K_RASTERIZER_EFFECT_PARTICLE_SYSTEM_RETN, FUNC_PTR);
				FUNC_PTR(RASTERIZER_EFFECT_PARTICLE_HOOK, K_RASTERIZER_EFFECT_PARTICLE_HOOK, FUNC_PTR);
				FUNC_PTR(RASTERIZER_EFFECT_PARTICLE_RETN, K_RASTERIZER_EFFECT_PARTICLE_RETN, FUNC_PTR);
				FUNC_PTR(RASTERIZER_EFFECT_CONTRAIL_HOOK, K_RASTERIZER_EFFECT_CONTRAIL_HOOK, FUNC_PTR);
				FUNC_PTR(RASTERIZER_EFFECT_CONTRAIL_RETN, K_RASTERIZER_EFFECT_CONTRAIL_RETN, FUNC_PTR);

				FUNC_PTR(RASTERIZER_EFFECT_PS_INDEX_NO_HOOK, K_RASTERIZER_EFFECT_PS_INDEX_NO_HOOK, FUNC_PTR_NULL);
				FUNC_PTR(RASTERIZER_EFFECT_PS_INDEX_NO_RETN, K_RASTERIZER_EFFECT_PS_INDEX_NO_RETN, FUNC_PTR_NULL);
			}
		}
	}
}
