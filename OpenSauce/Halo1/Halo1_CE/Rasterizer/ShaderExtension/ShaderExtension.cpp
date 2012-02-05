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
#include "Common/Precompile.hpp"
#include "Rasterizer/ShaderExtension/ShaderExtension.hpp"

#if !PLATFORM_IS_DEDI
#include "Common/YeloSettings.hpp"
#include "Common/FileIO.hpp"
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
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_SHADEREXTENSION
#include "Memory/_EngineLayout.inl"

			enum ps_2_x_support{
				_ps_2_x_support_none,
				_ps_2_x_support_2_0,
				_ps_2_x_support_2_a,
				_ps_2_x_support_2_b,

				_ps_2_x_support
			};

			typedef char t_shader_usage_id[128];
			typedef void (PLATFORM_API *shadersetup_func)(void*, void*, void*, void*, void*, void*, void*);

			static const char* g_ps_collection_format_a =	"shaders\\EffectCollection_ps_%d_a.enc";
			static const char* g_ps_collection_format_b =	"shaders\\EffectCollection_ps_%d_b.enc";
			static const char* g_vs_collection_path =		"shaders\\vsh_OS.enc";

			static const char* g_shader_collection_ids[] = 
			{
				"SE_EffectCollection_PS2a",
				"SE_EffectCollection_PS2b",
				"SE_EffectCollection_VS"
			};

			static bool							g_shader_files_present = false;
			static ps_2_x_support				g_ps_support = _ps_2_x_support_none;
			static bool							g_extensions_enabled = true;
			static bool							g_extensions_enabled_user_override = true;

			void		SetTexture(IDirect3DDevice9* pDevice, uint16 sampler, datum_index bitmap_tag_index);

#include "Rasterizer/ShaderExtension/ShaderExtension_Model.inl"
#include "Rasterizer/ShaderExtension/ShaderExtension_Environment.inl"

			API_FUNC_NAKED static void Hook_RenderObject_ForceInvertBackfaceNormals()
			{
				static uint32 RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_DRAW_INVERT_BACKFACE_NORMALS_CHECK_RETN);

				_asm{
					mov     al, 1
					test    al, al

					jmp		RETN_ADDRESS
				};
			}

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
				// replace the vertex shader collection paths
				for(int32 i = 0; i < NUMBEROF(K_VSH_COLLECTION_PATH_REFERENCES); i++)
					*K_VSH_COLLECTION_PATH_REFERENCES[i] = g_vs_collection_path;

				// replace the sprintf call to intercept the collection path creation and edit it as necessary
				Memory::WriteRelativeCall(&BuildEffectCollectionPath, GET_FUNC_VPTR(RASTERIZER_DX9_SHADERS_EFFECT_SHADERS_INITIALIZE__SPRINTF_CALL), true);
				// Forces the backface normals of a two sided face to be inverted to fix lighting issues on trees
				Memory::WriteRelativeJmp(&Hook_RenderObject_ForceInvertBackfaceNormals, GET_FUNC_VPTR(RASTERIZER_MODEL_DRAW_INVERT_BACKFACE_NORMALS_CHECK_HOOK), true);
			}

			HRESULT		SetVertexShaderConstantF(uint32 render_stage, IDirect3DDevice9* pDevice, UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
			{
				HRESULT hr = S_OK;
				if(g_ps_support > _ps_2_x_support_2_0)
				{
					switch(render_stage)
					{
					case Enums::_render_progress_objects:
						hr = Model::SetVertexShaderConstantF(pDevice, StartRegister, pConstantData, Vector4fCount); break;
					default:
						hr = pDevice->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount); break;
					}
				}
				else
					hr = pDevice->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
				return hr;
			}

			void		Initialize()
			{
				g_shader_files_present = true;

				// determine if the custom shader collections are present
				for (int i = 0; (i < NUMBEROF(g_shader_collection_ids)) && g_shader_files_present; i++)
				{
					FileIO::s_file_info file_info;
					Enums::file_io_open_error file_open_success = FileIO::OpenFileByID(file_info, g_shader_collection_ids[i]);
					FileIO::CloseFile(file_info);

					g_shader_files_present &= (file_open_success == Enums::_file_io_open_error_none);
				}
			}
			void		Dispose()		{}

			void		Initialize3D(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params)
			{
				g_extensions_enabled = false;

				// user settings override to completely disable the shader extensions
				if(!g_extensions_enabled_user_override)
					return;

				// the shader files are not present, so do not set up the hooks
				if(!g_shader_files_present)
					return;

				// determine the maximum pixel shader profile the graphics device supports

				// get the profile string id
				const char* ps_profile = D3DXGetPixelShaderProfile(device);
				if(!ps_profile)
					return;

				// parse the profile string to get the major version as an integer and the minor version as a char
				int profile_major = 0;
				char profile_minor_char = 0;

				int fields_read = sscanf_s(ps_profile, "ps_%i_%c", &profile_major, &profile_minor_char, 1);
				if(fields_read != 2)
					return;

				// no need to have the minor version as an int as we only need to compare it with a or b when major version is 2
				switch(profile_major)
				{
				case 2:
					{
						if(profile_minor_char == 'a')
							g_ps_support = _ps_2_x_support_2_a;
						else if(profile_minor_char == 'b')
							g_ps_support = _ps_2_x_support_2_b;
						else
							g_ps_support = _ps_2_x_support_2_0;
						break;
					}
				default:
					{
						// if the major version is less than 2 disable the shader extension
						if(profile_major < 2)
						{
							g_ps_support = _ps_2_x_support_none;
							return;
						}
						// if the pixel shader major version is more than 2 use ps_2_a as that has more features
						g_ps_support = _ps_2_x_support_2_a;
						break;
					}
				}

				// if the required shader version is supported, put the hooks in place to
				// add normal goodness
				if(g_ps_support > _ps_2_x_support_2_0)
				{
					g_extensions_enabled = true;
					ApplyHooks();
					Model::ApplyHooks();
					Environment::ApplyHooks();
				}
			}
			void		OnLostDevice(){}
			void		OnResetDevice(D3DPRESENT_PARAMETERS* params){}
			void		Release(){}
			void		Render() {}

			void		LoadSettings(TiXmlElement* parent_element)
			{
				g_extensions_enabled_user_override = true;

				if(parent_element != NULL)
				{
					TiXmlElement* extension_element = parent_element->FirstChildElement("ShaderExtension");

					// read the user override value, default to enabled if the attribute is not set
					const char* enabled = extension_element->Attribute("enabled");
					if(enabled)
						g_extensions_enabled_user_override = Settings::ParseBoolean(enabled);

					Model::LoadSettings(extension_element);
				}
			}

			void		SaveSettings(TiXmlElement* parent_element)
			{
				if(parent_element != NULL)
				{
					TiXmlElement* extension_element = new TiXmlElement("ShaderExtension");
					parent_element->LinkEndChild(extension_element);

					extension_element->SetAttribute("enabled", Settings::BooleanToString(g_extensions_enabled_user_override));

					Model::SaveSettings(extension_element);
				}
			}
		};
	};
};
#endif