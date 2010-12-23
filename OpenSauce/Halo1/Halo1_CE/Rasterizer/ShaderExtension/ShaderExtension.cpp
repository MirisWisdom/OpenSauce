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
#include "Common/Precompile.hpp"
#include "Rasterizer/ShaderExtension/ShaderExtension.hpp"

#if !PLATFORM_IS_DEDI
#include <sys/stat.h>
#include "Rasterizer/DX9/DX9.hpp"
#include "Memory/MemoryInterface.hpp"
#include "Game/EngineFunctions.hpp"
#include "TagGroups/TagGroups.hpp"
#include <TagGroups/Halo1/shader_definitions.hpp>
#include <TagGroups/Halo1/bitmap_definitions.hpp>
#include "Rasterizer/GBuffer.hpp"

namespace Yelo
{
	namespace Rasterizer
	{
		namespace ShaderExtension
		{		
#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER_SHADEREXTENSION
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_SHADEREXTENSION_MODEL
#include "Memory/_EngineLayout.inl"

			enum ps_2_x_support{
				_ps_2_x_support_none,
				_ps_2_x_support_2_0,
				_ps_2_x_support_2_a,
				_ps_2_x_support_2_b,

				_ps_2_x_support
			};

			struct s_shader_variables {
				real	specular_color_interp;
				real	base_normal_interp;
				real	detail_normal_1_interp;
				real	detail_normal_2_interp;

				real	specular_color_multiplier;
				real	base_normal_multiplier;
				real	detail_normal_1_multiplier;
				real	detail_normal_2_multiplier;

				real	specular_color_power;
				real	detail_normal_1_scale;
				real	detail_normal_2_scale;
				PAD32;
			};

			typedef void (PLATFORM_API *shadersetup_func)(void*, void*, void*, void*, void*, void*, void*);

			static const char* g_ps_collection_format_a =	"shaders\\EffectCollection_ps_%d_a.enc";
			static const char* g_ps_collection_format_b =	"shaders\\EffectCollection_ps_%d_b.enc";
			static const char* g_vs_collection_path =		"shaders\\vsh_OS.enc";

			static bool					g_shader_files_present = false;
			static ps_2_x_support		g_ps_support = _ps_2_x_support_none;
			static s_shader_variables	g_shader_variables;

			void		SetTexture(IDirect3DDevice9* pDevice, uint16 sampler, datum_index bitmap_tag_index)
			{
				// get the bitmap datum pointer
				TagGroups::s_bitmap_definition*	bitmap = TagGroups::Instances()[bitmap_tag_index.index].Definition<TagGroups::s_bitmap_definition>();
				TagGroups::s_bitmap_data*		bitmap_datum = CAST_PTR(TagGroups::s_bitmap_data*, &bitmap->bitmaps[0]);

				// if the bitmap hasn't been created yet, create it
				if(bitmap_datum->hardware_format == NULL)
					Yelo::Engine::TextureCacheRequestTexture(bitmap_datum, true, true);

				// set the texture to the device
				pDevice->SetTexture(sampler, CAST_PTR(IDirect3DBaseTexture9*, bitmap_datum->hardware_format));
				pDevice->SetSamplerState(sampler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
				pDevice->SetSamplerState(sampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
				pDevice->SetSamplerState(sampler, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
			}
			void PLATFORM_API SetModelNormSpec(void* shader_pointer)
			{				
				// reset to defaults
				memset(&g_shader_variables, 0, sizeof(g_shader_variables));
				g_shader_variables.specular_color_multiplier = 1.0f;
				g_shader_variables.base_normal_multiplier = 1.0f;
				g_shader_variables.detail_normal_1_multiplier = 1.0f;
				g_shader_variables.detail_normal_2_multiplier = 1.0f;

				TagGroups::s_shader_definition* shader_base = 
					CAST_PTR(TagGroups::s_shader_definition*, shader_pointer);

				DX9::c_gbuffer_system::OutputObjectTBN() = false;

				if(shader_base->shader.shader_type == Enums::_shader_type_model)
				{
					TagGroups::s_shader_model* shader_model = CAST_PTR(TagGroups::s_shader_model*, shader_base);
					if(shader_model->model.maps.shader_extension.Count == 1)
					{
						const TagGroups::s_shader_model_extension& extension = shader_model->model.maps.shader_extension[0];

						// setup base normal map
						if(!extension.base_normal.map.tag_index.IsNull())
						{
							DX9::c_gbuffer_system::OutputObjectTBN() = true;

							SetTexture(Yelo::DX9::Direct3DDevice(), 4, extension.base_normal.map.tag_index); 
							g_shader_variables.base_normal_interp = 1.0f;
							g_shader_variables.base_normal_multiplier = extension.base_normal.modifiers.multiplier;
						}
						else
							Yelo::DX9::Direct3DDevice()->SetTexture(4, NULL);

						// setup detail normal map 1
						if(!extension.detail_normals[0].map.tag_index.IsNull())
						{
							SetTexture(Yelo::DX9::Direct3DDevice(), 5, extension.detail_normals[0].map.tag_index); 
							g_shader_variables.detail_normal_1_interp = 1.0f;
							g_shader_variables.detail_normal_1_multiplier = extension.detail_normals[0].modifiers.multiplier;
							g_shader_variables.detail_normal_1_scale = extension.detail_normals[0].modifiers.scale;
						}
						else
							Yelo::DX9::Direct3DDevice()->SetTexture(5, NULL);

						// setup detail normal map 2
						if(!extension.detail_normals[1].map.tag_index.IsNull())
						{
							SetTexture(Yelo::DX9::Direct3DDevice(), 6, extension.detail_normals[1].map.tag_index); 
							g_shader_variables.detail_normal_2_interp = 1.0f;
							g_shader_variables.detail_normal_2_multiplier = extension.detail_normals[1].modifiers.multiplier;
							g_shader_variables.detail_normal_2_scale = extension.detail_normals[1].modifiers.scale;
						}
						else
							Yelo::DX9::Direct3DDevice()->SetTexture(6, NULL);

						// setup specular map
						if(!extension.specular_color.map.tag_index.IsNull())
						{
							SetTexture(Yelo::DX9::Direct3DDevice(), 7, extension.specular_color.map.tag_index); 
							g_shader_variables.specular_color_interp = 1.0f;
							g_shader_variables.specular_color_multiplier = extension.specular_color.modifiers.multiplier;
							g_shader_variables.specular_color_power = extension.specular_color.modifiers.power;
						}
						else
							Yelo::DX9::Direct3DDevice()->SetTexture(7, NULL);
					}
				}
				else if(shader_base->shader.shader_type == Enums::_shader_type_environment)
				{
					TagGroups::s_shader_environment_definition* shader_environment = 
						CAST_PTR(TagGroups::s_shader_environment_definition*, shader_base);

					// setup base normal map
					if(!shader_environment->environment.bump_map.tag_index.IsNull())
					{
						DX9::c_gbuffer_system::OutputObjectTBN() = true;

						SetTexture(Yelo::DX9::Direct3DDevice(), 4, shader_environment->environment.bump_map.tag_index); 
						g_shader_variables.base_normal_interp = 1.0f;
						g_shader_variables.base_normal_multiplier = shader_environment->environment.bump_map_scale;
					}
					else
						Yelo::DX9::Direct3DDevice()->SetTexture(4, NULL);
					Yelo::DX9::Direct3DDevice()->SetTexture(5, NULL);
					Yelo::DX9::Direct3DDevice()->SetTexture(6, NULL);
					Yelo::DX9::Direct3DDevice()->SetTexture(7, NULL);
				}
				else
				{
					Yelo::DX9::Direct3DDevice()->SetTexture(4, NULL);
					Yelo::DX9::Direct3DDevice()->SetTexture(5, NULL);
					Yelo::DX9::Direct3DDevice()->SetTexture(6, NULL);
					Yelo::DX9::Direct3DDevice()->SetTexture(7, NULL);
				}
				Yelo::DX9::Direct3DDevice()->SetPixelShaderConstantF(0 + k_shader_constant_offset, (float*)&g_shader_variables, k_vector_4_count);
			}

			void PLATFORM_API ShaderSetupOverride_Model_ShaderEnvironment(void* shader_pointer, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6, void* arg7)
			{
				//call any custom shader setup functions
				SetModelNormSpec(shader_pointer);

				// call the original function
				static shadersetup_func shader_function = 
					CAST_PTR(shadersetup_func, GET_FUNC_VPTR(RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER));
				(*shader_function)(shader_pointer, arg2, arg3, arg4, arg5, arg6, arg7);
			}
			void PLATFORM_API ShaderSetupOverride_Model_ShaderModel(void* shader_pointer, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6, void* arg7)
			{
				//call any custom shader setup functions
				SetModelNormSpec(shader_pointer);

				// call the original function
				static shadersetup_func shader_function = 
					CAST_PTR(shadersetup_func, GET_FUNC_VPTR(RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_MODEL));
				(*shader_function)(shader_pointer, arg2, arg3, arg4, arg5, arg6, arg7);
			}

			int PLATFORM_API BuildEffectCollectionPath(char* string_out, const char* format_string, const int major, const int minor)
			{
				const char* format = format_string;
				if((major == 2) && (minor == 2) && (g_ps_support == _ps_2_x_support_2_a))
					format = g_ps_collection_format_a;
				if((major == 2) && (minor == 1) && (g_ps_support == _ps_2_x_support_2_b))
					format = g_ps_collection_format_b;
				return sprintf(string_out, format, major, minor);
			}

			void		ApplyHooks()
			{
				// Halo uses function pointers to point to the appropriate functions for
				// whatever shader model the device supports
				// Here, we replace the pointer to the highest level shader model functions
				// for shader_model and shader_environment

				// replace the function used for shader_environment when on a model
				GET_PTR(RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_SETUP__SET_SHADER_ENVIRONMENT_PTR) =	&ShaderSetupOverride_Model_ShaderEnvironment;
				// replace the function used for shader_model when on a model
				GET_PTR(RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_SETUP__SET_SHADER_MODEL_PTR) =			&ShaderSetupOverride_Model_ShaderModel;

				// replace the vertex shader collection paths
				for(int32 i = 0; i < NUMBEROF(K_VSH_COLLECTION_PATH_REFERENCES); i++)
					*K_VSH_COLLECTION_PATH_REFERENCES[i] = g_vs_collection_path;

				// replace the sprintf call to intercept the collection path creation and edit it as necessary
				Memory::WriteRelativeCall(&BuildEffectCollectionPath, GET_FUNC_VPTR(RASTERIZER_DX9_SHADERS_EFFECT_SHADERS_INITIALIZE__SPRINTF_CALL), true);
			}

			HRESULT		SetVertexShaderConstantF(IDirect3DDevice9* pDevice, UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
			{
				HRESULT hr = S_OK;
				if(g_ps_support > _ps_2_x_support_2_0)
				{
					//always pass the eye position since it doesn't get updated for each object	
					if(StartRegister == 0)
					{
						// both the pixel shader and vertex shader use constant c0
						hr = pDevice->SetPixelShaderConstantF(3 + k_shader_constant_offset, &pConstantData[4 * 4], 1);
						hr |= pDevice->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
					}
					else if(StartRegister == 10) // c_unknown_c12
					{
						// the vertex shader uses the constants in c10-c12 so they have to be set as well
						hr = pDevice->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
						// but the pixel shader only uses c12 so we only set that to the pixel shader
						hr = pDevice->SetPixelShaderConstantF(4 + k_shader_constant_offset, &pConstantData[2 * 4], 1);
					}
					else if(StartRegister == 13 && Vector4fCount == 2) // parallel and perpendicular colour
						hr = pDevice->SetPixelShaderConstantF(5 + k_shader_constant_offset, pConstantData, 2);
					else if(StartRegister == 15) // light positions rotations and colours (2 dynamic, 2 ambient)
						hr = pDevice->SetPixelShaderConstantF(7 + k_shader_constant_offset, pConstantData, Vector4fCount);
					else						
						hr = pDevice->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
				}
				else
					hr = pDevice->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
				return hr;
			}
			
			void		Initialize()	
			{
				g_shader_files_present = true;

				// determine if the custom shader collections are present
				struct stat file_stat;
				g_shader_files_present &= stat("shaders\\EffectCollection_ps_2_a.enc", &file_stat) == 0;
				g_shader_files_present &= stat("shaders\\EffectCollection_ps_2_b.enc", &file_stat) == 0;
				g_shader_files_present &= stat("shaders\\vsh_OS.enc", &file_stat) == 0;
			}
			void		Dispose()		{}

			void		Initialize3D(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params)
			{
				// the shader files are not present, so do not set up the hooks
				if(!g_shader_files_present)
					return;

				// determine whether the device supports ps_2_a or ps_2_b
				// this affects which effect collection will eventually be loaded
				D3DCAPS9 dev_caps;
				device->GetDeviceCaps(&dev_caps);
				if(dev_caps.PS20Caps.NumInstructionSlots == 96)
					g_ps_support = _ps_2_x_support_2_0;
				else if(dev_caps.PS20Caps.NumInstructionSlots == 512)
				{
					if(dev_caps.PS20Caps.NumTemps == 22)
						g_ps_support = _ps_2_x_support_2_a;
					else if(dev_caps.PS20Caps.NumTemps == 32)
						g_ps_support = _ps_2_x_support_2_b;
				}
				// if the required shader version is supported, put the hooks in place to
				// add normal goodness
				if(g_ps_support > _ps_2_x_support_2_0)
					ApplyHooks();
			}
			void		OnLostDevice(){}
			void		OnResetDevice(D3DPRESENT_PARAMETERS* params){}
			void		Release(){}
			void		Render() {}
		};
	};
};
#endif