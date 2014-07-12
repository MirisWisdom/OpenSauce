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
		const uint32	shader_pointer_offset;
	};

	/// <summary>	The list of feature combinations available. </summary>
	const uint32 g_shader_offsets[] = {
		sizeof(DX9::s_rasterizer_dx9_pixel_shader) * 0, // Standard
		sizeof(DX9::s_rasterizer_dx9_pixel_shader) * 1, // Directional
		sizeof(DX9::s_rasterizer_dx9_pixel_shader) * 2  // Directional with Normal map
	};

	static uint32			g_current_shader_offset = 0;
	static _enum			g_extension_usage_mask;

	/// <summary>	Hook to change the used pixel shader to a custom one. </summary>
	API_FUNC_NAKED void Hook_ShaderEnvironmentLightmapPS()
	{
		static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_ENVIRONMENT_PS_INDEX_ENVIRONMENT_LIGHTMAP_RETN);

		_asm
		{
			mov		edx, [edx+84h]

			push	ebx
			add		edx, g_current_shader_offset					// Add g_current_shader_offset to edx to point to the custom shader
			pop		ebx

			jmp		RETN_ADDRESS
		};
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Calculates the z coordinate coefficient. </summary>
	///
	/// <param name="bump_amount">	The bump amount. </param>
	///
	/// <returns>	The calculated z coordinate coefficient. </returns>
	static real CalculateZCoefficient(const real bump_amount)
	{
		real z_multiplier = 0;
		if (bump_amount > 0)
		{
			z_multiplier = __max(1.0f / bump_amount, 0.0f);
		}
		return __min(z_multiplier, 1.0f);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets up the custom shader variables for a shader environment. </summary>
	///
	/// <param name="shader">   	The shader. </param>
	/// <param name="variables">	[out] The pixel variables. </param>
	///
	/// <returns>	Returns the shader offset to use. </returns>
	static const uint32 SetupShaderEnvironment(const TagGroups::s_shader_environment_definition& shader, s_pixel_shader_variables& variables)
	{		
		if(TEST_FLAG(shader.shader.extension_usage, Flags::_shader_extension_usage_bit_normal_map))
		{
			if(shader.environment.shader_extension.Count == 1)
			{
				// If the shader has a shader extension block and a normal map, set the multiplier from the extension block
				auto& extension = shader.environment.shader_extension[0];
				float coefficient = extension.bump_amount;

				variables.base_normal_map_coefficient = coefficient;
				variables.base_normal_map_z_coefficient = CalculateZCoefficient(coefficient);
			}
			
			return g_shader_offsets[2];
		}
		
		return g_shader_offsets[1];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets up the custom shader variables for a shader model. </summary>
	///
	/// <param name="shader">   	The shader. </param>
	/// <param name="variables">	[out] The pixel variables. </param>
	///
	/// <returns>	Returns the shader offset to use. </returns>
	static const uint32 SetupShaderModel(const TagGroups::s_shader_model_definition& shader, s_pixel_shader_variables& variables)
	{
		if(TEST_FLAG(shader.shader.extension_usage, Flags::_shader_extension_usage_bit_normal_map))
		{
			if(shader.model.maps.shader_extension.Count == 1)
			{
				// If the shader has a shader extension block and a normal map set the multiplier from the extension block
				auto& extension = shader.model.maps.shader_extension[0];
				float coefficient = extension.base_normal.modifiers.coefficient;

				variables.base_normal_map_coefficient = coefficient;
				variables.base_normal_map_z_coefficient = CalculateZCoefficient(coefficient);

				return g_shader_offsets[2];
			}
		}

		return g_shader_offsets[1];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets directional lightmap. </summary>
	///
	/// <param name="shader_pointer">	[in,out] If non-null, the shader pointer. </param>
	void SetEnvironmentLightmapVariables(const TagGroups::s_shader_definition* shader)
	{
		if((g_ps_support <= _ps_support_2_0) || !g_extensions_enabled)
			return;

		if(!Render::Lightmaps::UsingDirectionalLightmaps()
			|| !shader || !TEST_FLAG(shader->shader.extension_usage, Flags::_shader_extension_usage_bit_directional_lightmaps))
		{
			g_current_shader_offset = 0;
			return;
		}

		s_pixel_shader_variables pixel_shader_variables = { 1.0f, 1.0f };

		// Default to the stock shader
		switch(shader->shader.shader_type)
		{
		case Enums::_shader_type_environment:
			g_current_shader_offset = SetupShaderEnvironment(*CAST_PTR(const TagGroups::s_shader_environment_definition*, shader), pixel_shader_variables);
			break;
		case Enums::_shader_type_model:
			g_current_shader_offset = SetupShaderModel(*CAST_PTR(const TagGroups::s_shader_model_definition*, shader), pixel_shader_variables);
			break;
		YELO_ASSERT_CASE_UNREACHABLE();
		};

		// Set the custom pixel shader values
		DX9::Direct3DDevice()->SetPixelShaderConstantF(6, (float*)&pixel_shader_variables, 1);
	}

	/// <summary>	Applies the hooks for custom shader code. </summary>
	void		ApplyHooks()
	{
		Memory::WriteRelativeJmp(&Hook_ShaderEnvironmentLightmapPS, GET_FUNC_VPTR(RASTERIZER_ENVIRONMENT_PS_INDEX_ENVIRONMENT_LIGHTMAP_HOOK), true);
	}
};