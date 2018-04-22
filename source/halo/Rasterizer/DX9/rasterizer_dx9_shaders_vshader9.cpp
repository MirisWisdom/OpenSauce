/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include <halo/halo.h>
#include "Rasterizer/DX9/rasterizer_dx9_shaders_vshader9.hpp"

#include <Pointers/Rasterizer.DX9.VShaders.hpp>

namespace Yelo
{
	namespace DX9
	{
		s_rasterizer_dx9_effect_collection* EffectCollections() PTR_IMP_GET2(rasterizer_dx9_effect_collection);
		s_effect_shader_entry* EffectShaders() PTR_IMP_GET2(rasterizer_effect_shaders);
		s_vsf_entry* VertexShaders() PTR_IMP_GET2(vsf_table);
	};
};
