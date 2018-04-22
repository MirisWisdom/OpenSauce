/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Pointers/1.10/HaloCE_110_Runtime.Rasterizer.ShaderExtension.inl"
#include "Pointers/1.10/HaloCE_110_Runtime_Manual.Rasterizer.ShaderExtension.inl"

#include <yelo/memory/memory_interface_base.hpp>
#include <Memory/MemoryInterface.hpp>

namespace Yelo
{
	namespace Rasterizer
	{
		namespace ShaderExtension
		{
			FUNC_PTR(RASTERIZER_MODEL_DRAW_INVERT_BACKFACE_NORMALS_CHECK_HOOK, K_RASTERIZER_MODEL_DRAW_INVERT_BACKFACE_NORMALS_CHECK_HOOK, FUNC_PTR_NULL);
			FUNC_PTR(RASTERIZER_MODEL_DRAW_INVERT_BACKFACE_NORMALS_CHECK_RETN, K_RASTERIZER_MODEL_DRAW_INVERT_BACKFACE_NORMALS_CHECK_RETN, FUNC_PTR_NULL);

			FUNC_PTR(RASTERIZER_DX9_SHADERS_EFFECT_SHADERS_INITIALIZE__SPRINTF_CALL, K_RASTERIZER_DX9_SHADERS_EFFECT_SHADERS_INITIALIZE__SPRINTF_CALL, PTR_NULL);

			static cstring* K_VSH_COLLECTION_PATH_REFERENCES[] = {
				CAST_PTR(cstring*,K_VSH_COLLECTION_PATH_REFERENCE_0),
				CAST_PTR(cstring*,K_VSH_COLLECTION_PATH_REFERENCE_1),
			};

			static void** K_PS_VERSION_ITERATOR_START[] = {
				CAST_PTR(void**,K_PS_VERSION_ITERATOR_START_0),
				CAST_PTR(void**,K_PS_VERSION_ITERATOR_START_1),
			};
		}
	}
}
#endif
