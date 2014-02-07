/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
namespace Environment
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER_SHADEREXTENSION
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_SHADEREXTENSION_ENVIRONMENT
#include "Memory/_EngineLayout.inl"

	/// <summary>	The pixel shader variables used by the custom lightmap shader. </summary>
	struct s_pixel_shader_variables {
		real base_normal_map_coefficient;
		real base_normal_map_z_coefficient;
		PAD32;
		PAD32;
	};

	/// <summary>	Defines an environment shader feature mix. </summary>
	struct s_shader_feature_mix {
		const _enum		feature_mask;
		PAD16;
		const uint32	shader_pointer_offset;
		const char*		extension_id;
	};

	/// <summary>	The list of feature combinations available. </summary>
	s_shader_feature_mix g_feature_mix_list[] = {
		{
			Enums::_environment_extension_usage_none,
			sizeof(DX9::s_rasterizer_dx9_pixel_shader) * 0,
			""
		},
		{
			Enums::_environment_extension_usage_diffuse_dlm_lighting,
			sizeof(DX9::s_rasterizer_dx9_pixel_shader) * 1,
			"Dir"
		},
		{
			Enums::_environment_extension_usage_diffuse_dlm_lighting,
			sizeof(DX9::s_rasterizer_dx9_pixel_shader) * 1,
			"DirBaseNorm"
		},
	};

	static s_shader_feature_mix*		g_current_feature_mix = &g_feature_mix_list[0];
	static _enum						g_extension_usage_mask;
	static s_pixel_shader_variables		g_pixel_shader_variables;

	/// <summary>	Hook to change the used pixel shader to a custom one. </summary>
	API_FUNC_NAKED void Hook_ShaderEnvironmentLightmapPS()
	{
		static const uint32 RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_ENVIRONMENT_PS_INDEX_ENVIRONMENT_LIGHTMAP_RETN);

		_asm
		{
			mov		edx, [edx+84h]

			push	ebx
			mov		ebx, g_current_feature_mix
			test	ebx, ebx
			jz		no_extension					// If g_current_feature_mix is null use the stock shader pointer
			add		edx, [ebx + 4]					// Otherwise add g_current_feature_mix.shader_pointer_offset to edx to point to the custom shader
no_extension:
			pop		ebx

			jmp		RETN_ADDRESS
		};
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets up the custom shader variables for a shader environment. </summary>
	///
	/// <param name="shader">	The shader. </param>
	///
	/// <returns>	Returns the shader index to use. </returns>
	static uint32 SetupShaderEnvironment(const TagGroups::s_shader_environment_definition* shader)
	{
		int shader_index = 1;

		// Set the default bump multiplier
		g_pixel_shader_variables.base_normal_map_coefficient = 1.0f;
		g_pixel_shader_variables.base_normal_map_z_coefficient = 1.0f;

		if((datum_index::null != shader->environment.bump.bump_map.map.tag_index)
			&& (shader->environment.shader_extension.Count == 1))
		{
			// If the shader has a shader extension block and a normal map set the multiplier from the extension block
			//TODO: put the coefficients in the tag and calculate it at compile time, or do we leave it like this for RTE?
			auto shader_extension = shader->environment.shader_extension[0];

			g_pixel_shader_variables.base_normal_map_coefficient = shader_extension.bump_amount;

			real z_multiplier = 0;
			if (shader_extension.bump_amount > 0)
			{
				z_multiplier = max(1.0f / shader_extension.bump_amount, 0.0f);
			}
			g_pixel_shader_variables.base_normal_map_z_coefficient = min(z_multiplier, 1.0f);

			shader_index = 2;
		}

		// Set the custom pixel shader values
		DX9::Direct3DDevice()->SetPixelShaderConstantF(6, (float*)&g_pixel_shader_variables, 1);

		return shader_index;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets up the custom shader variables for a shader model. </summary>
	///
	/// <param name="shader">	The shader. </param>
	///
	/// <returns>	Returns the shader index to use. </returns>
	static uint32 SetupShaderModel(const TagGroups::s_shader_model_definition* shader)
	{
		int shader_index = 1;

		// Set the default bump multiplier
		g_pixel_shader_variables.base_normal_map_coefficient = 1.0f;
		g_pixel_shader_variables.base_normal_map_z_coefficient = 1.0f;

		if(shader->model.maps.shader_extension.Count == 1)
		{
			if(datum_index::null != shader->model.maps.shader_extension[0].base_normal.map.tag_index)
			{
				// If the shader has a shader extension block and a normal map set the multiplier from the extension block
				float coefficient = shader->model.maps.shader_extension[0].base_normal.modifiers.coefficient;

				g_pixel_shader_variables.base_normal_map_coefficient = coefficient;

				real z_multiplier = 0;
				if (coefficient > 0)
				{
					z_multiplier = max(1.0f / coefficient, 0.0f);
				}
				g_pixel_shader_variables.base_normal_map_z_coefficient = min(z_multiplier, 1.0f);

				shader_index = 2;
			}
		}

		// Set the custom pixel shader values
		DX9::Direct3DDevice()->SetPixelShaderConstantF(6, (float*)&g_pixel_shader_variables, 1);

		return shader_index;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets directional lightmap. </summary>
	///
	/// <param name="shader_pointer">	[in,out] If non-null, the shader pointer. </param>
	void SetEnvironmentLightmapVariables(void* shader_pointer)
	{
		// Default to the stock shader index
		int shader_index = 0;

		if (shader_pointer && Render::Lightmaps::UsingDirectionalLightmaps())
		{
			// If directional lightmaps should be used, set the custom shader variables
			const TagGroups::s_shader_definition* shader_base = CAST_PTR(const TagGroups::s_shader_definition*, shader_pointer);

			switch(shader_base->shader.shader_type)
			{
			case Enums::_shader_type_environment:
				shader_index = SetupShaderEnvironment(CAST_PTR(const TagGroups::s_shader_environment_definition*, shader_pointer));
				break;
			case Enums::_shader_type_model:
				shader_index = SetupShaderModel(CAST_PTR(const TagGroups::s_shader_model_definition*, shader_pointer));
				break;
			default:
				YELO_ASSERT_DISPLAY(false, "Unexpected shader type for directional lightmap setup");
			};
		}

		YELO_ASSERT_DISPLAY((shader_index < NUMBEROF(g_feature_mix_list)) && (shader_index >= 0), "Environment lightmap shader index is out of bounds");

		g_current_feature_mix = &g_feature_mix_list[shader_index];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Loads the environment shader settings . </summary>
	///
	/// <param name="parent_element">	[in,out] If non-null, the parent xml element. </param>
	void		LoadSettings(TiXmlElement* parent_element)
	{
		g_extension_usage_mask = Enums::_environment_extension_usage_diffuse_dlm_lighting;

		if(parent_element != NULL)
		{
			int32 usage_mask = Enums::_environment_extension_usage_none;
			TiXmlElement* environment_element = parent_element->FirstChildElement("Environment");

			if(environment_element != NULL)
			{
				TiXmlElement* feature_element = NULL;
				char* feature_attribute = NULL;

				if(feature_element = environment_element->FirstChildElement("DiffuseDLMLighting"))
					usage_mask |= (Settings::ParseBoolean(feature_element->Attribute("enabled")) ? Enums::_environment_extension_usage_diffuse_dlm_lighting : Enums::_environment_extension_usage_none);

				g_extension_usage_mask &= usage_mask;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Saves the environment shader settings. </summary>
	///
	/// <param name="parent_element">	[in,out] If non-null, the parent xml element. </param>
	void		SaveSettings(TiXmlElement* parent_element)
	{
		if(parent_element != NULL)
		{
			TiXmlElement* environment_element = new TiXmlElement("Environment");
			parent_element->LinkEndChild(environment_element);

			TiXmlElement* feature_element = NULL;

			feature_element = new TiXmlElement("DiffuseDLMLighting");
			feature_element->SetAttribute("enabled",
				BooleanToString((g_extension_usage_mask & Enums::_environment_extension_usage_diffuse_dlm_lighting) == Enums::_environment_extension_usage_diffuse_dlm_lighting));
			environment_element->LinkEndChild(feature_element);
		}
	}

	/// <summary>	Applies the hooks for custom shader code. </summary>
	void		ApplyHooks()
	{
		Memory::WriteRelativeJmp(&Hook_ShaderEnvironmentLightmapPS, GET_FUNC_VPTR(RASTERIZER_ENVIRONMENT_PS_INDEX_ENVIRONMENT_LIGHTMAP_HOOK), true);
	}
};