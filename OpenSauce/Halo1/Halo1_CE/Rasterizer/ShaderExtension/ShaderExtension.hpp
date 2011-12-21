/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
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
			HRESULT		SetVertexShaderConstantF(uint32 render_stage, IDirect3DDevice9* pDevice, UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount);
		};
	};
};
#endif