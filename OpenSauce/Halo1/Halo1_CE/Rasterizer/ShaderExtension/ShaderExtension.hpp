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
		Component: ShaderExtension
		Original Author: FireScythe

	This component adds a normal maps and a specular map to Halo's
	rendering engine, allowing for per pixel lighting.
	shader_model tags can now have a base normal map, two detail
	normal maps and a specular colour map.
	shader_environment tags that have a normal map assigned will
	also have per pixel lighting applied when used on a model.
*/
#pragma once

#if !PLATFORM_IS_DEDI
namespace Yelo
{
	namespace Rasterizer
	{
		namespace ShaderExtension
		{
			enum {
				k_vector_4_count = 3,
				k_shader_constant_offset = 5
			};

			void		Initialize();
			void		Dispose();

			void		Initialize3D(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params);
			void		OnLostDevice();
			void		OnResetDevice(D3DPRESENT_PARAMETERS* params);
			void		Render();
			void		Release();

			void		ApplyHooks();
			HRESULT		SetVertexShaderConstantF(IDirect3DDevice9* pDevice, UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount);
		};
	};
};
#endif