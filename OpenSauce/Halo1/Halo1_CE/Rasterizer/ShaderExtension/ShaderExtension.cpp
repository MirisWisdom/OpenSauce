/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/ShaderExtension/ShaderExtension.hpp"

#if !PLATFORM_IS_DEDI
#include <sys/stat.h>

#include <blamlib/Halo1/bitmaps/bitmap_group.hpp>
#include <blamlib/Halo1/shaders/shader_definitions.hpp>
#include <YeloLib/Halo1/shell/shell_windows_command_line.hpp>

#include "Common/YeloSettings.hpp"
#include "Common/FileIO.hpp"
#include "Game/EngineFunctions.hpp"
#include "Game/GameState.hpp"
#include "Memory/MemoryInterface.hpp"
#include "Rasterizer/GBuffer.hpp"
#include "Rasterizer/DX9/DX9.hpp"
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace Rasterizer
	{
		namespace ShaderExtension
		{
#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER_SHADEREXTENSION
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_SHADEREXTENSION
#include "Memory/_EngineLayout.inl"

			enum ps_support{
				_ps_support_legacy,
				_ps_support_2_0,
				_ps_support_2_a,
				_ps_support_2_b,
				_ps_support_3_0,

				_ps_support
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
			static ps_support					g_ps_support = _ps_support_legacy;
			static bool							g_extensions_enabled = true;
			static bool							g_extensions_enabled_user_override = true;
			static DWORD						g_ps_version_iterator_start = 0xFFFF0101;

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
				TagGroups::s_bitmap_group*	group = TagGroups::TagGetForModify<TagGroups::s_bitmap_group>(bitmap_tag_index);
				TagGroups::s_bitmap_data*	bitmap = CAST_PTR(TagGroups::s_bitmap_data*, &group->bitmaps[0]);

				// set the texture to the device
				Engine::SetTextureSamplerStage(bitmap, sampler);

				pDevice->SetSamplerState(sampler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
				pDevice->SetSamplerState(sampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
				pDevice->SetSamplerState(sampler, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

			}

			int PLATFORM_API BuildEffectCollectionPath(char* string_out, const char* format_string, const int major, const int minor)
			{
				const char* format = format_string;

				if((major >= 2) && (minor >= 0) && (g_ps_support > _ps_support_2_0))
				{
					switch(g_ps_support)
					{
					case _ps_support_3_0:
					case _ps_support_2_a:
						format = g_ps_collection_format_a;
						break;
					case _ps_support_2_b:
						format = g_ps_collection_format_b;
						break;
					default:
						break;
					}
				}
				return sprintf(string_out, format, major, minor);
			}

			void		ApplyHooks()
			{
				int32 i = 0;
				// replace the vertex shader collection paths
				for(i = 0; i < NUMBEROF(K_VSH_COLLECTION_PATH_REFERENCES); i++)
					*K_VSH_COLLECTION_PATH_REFERENCES[i] = g_vs_collection_path;

				// replace the sprintf call to intercept the collection path creation and edit it as necessary
				Memory::WriteRelativeCall(&BuildEffectCollectionPath, GET_FUNC_VPTR(RASTERIZER_DX9_SHADERS_EFFECT_SHADERS_INITIALIZE__SPRINTF_CALL), true);
				// Forces the backface normals of a two sided face to be inverted to fix lighting issues on trees
				Memory::WriteRelativeJmp(&Hook_RenderObject_ForceInvertBackfaceNormals, GET_FUNC_VPTR(RASTERIZER_MODEL_DRAW_INVERT_BACKFACE_NORMALS_CHECK_HOOK), true);

				for(i = 0; i < NUMBEROF(K_PS_VERSION_ITERATOR_START); i++)
					*K_PS_VERSION_ITERATOR_START[i] = &g_ps_version_iterator_start;
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

				if(CMDLINE_GET_PARAM(no_os_gfx).ParameterSet())
					return;

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
				case 3:
					g_ps_support = _ps_support_3_0;
					break;
				case 2:
					{
						if(profile_minor_char == 'a')
							g_ps_support = _ps_support_2_a;
						else if(profile_minor_char == 'b')
							g_ps_support = _ps_support_2_b;
						else
							g_ps_support = _ps_support_2_0;
						break;
					}
				default:
					g_ps_support = _ps_support_legacy;
					break;
				}

				// if the required shader version is supported, put the hooks in place to
				// add normal goodness
				if(g_ps_support > _ps_support_2_0)
				{
					// modify the pixel shader version used for finding the correct shader collection and shaders
					// the *ShaderVersion value in D3DCAPS doesn't include 2.a and 2.b, instead it just shows 2.0

					// since this value is used to iterate through the shaders when loading, it would start at ps_2_0 and miss
					// the custom shaders at ps_2_1/ps_2_2 and Halo would close because the new shaders are expected, but not loaded

					// this does not affect shader model 3.0 cards as the iteration starts at ps_3_0 and works down.

					D3DCAPS9* device_caps = DX9::D3DCaps();
					if(g_ps_support == _ps_support_2_a)
						g_ps_version_iterator_start = 0xFFFF0202;
					else if(g_ps_support == _ps_support_2_b)
						g_ps_version_iterator_start = 0xFFFF0201;
					else
						g_ps_version_iterator_start = device_caps->PixelShaderVersion;
					
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
					if(extension_element)
					{
						const char* enabled = extension_element->Attribute("enabled");
						if(enabled)
							g_extensions_enabled_user_override = Settings::ParseBoolean(enabled);
						Model::LoadSettings(extension_element);
					}
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