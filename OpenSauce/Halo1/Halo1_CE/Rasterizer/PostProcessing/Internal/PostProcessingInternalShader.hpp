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

/*
	User Add-on
		Component: Post-processing
		Original Author: FireScythe
		Integration: kornman00
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/PostProcessingShader.hpp"
#include "Rasterizer/PostProcessing/PostProcessingShaderGenericBase.hpp"

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem { namespace Internal {
		/////////////////////////////////////////////////////////////////////
		// Shader interface for tag based shaders
		/////////////////////////////////////////////////////////////////////
		class c_internal_shader : public c_generic_shader_base
		{
		public:
			HRESULT				LoadBitmaps(IDirect3DDevice9* pDevice);
		};	
	};};};
};
#endif