/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/DX9/rasterizer_dx9_shaders_vshader9.hpp"

#include <Memory/MemoryInterface.hpp>

namespace Yelo
{
	namespace DX9
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER_DX9
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_DX9_VSHADERS
#include "Memory/_EngineLayout.inl"

		s_rasterizer_dx9_effect_collection*	EffectCollections()	PTR_IMP_GET2(rasterizer_dx9_effect_collection);
		s_effect_shader_entry*	EffectShaders()					PTR_IMP_GET2(rasterizer_effect_shaders);
		s_vsf_entry*	VertexShaders()							PTR_IMP_GET2(vsf_table);
	};
};