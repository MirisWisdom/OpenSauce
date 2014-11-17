/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
namespace Effect
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER_SHADEREXTENSION
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_SHADEREXTENSION_EFFECT
#include "Memory/_EngineLayout.inl"

	struct s_pixel_shader_variables {
		real m_far_clip_distance;
		real m_near_clip_distance;
		real m_depth_fade_distance;
		real m_camera_fade_distance;
	};
	
	const uint32 g_feature_mix_list[] = {
		sizeof(DX9::s_rasterizer_dx9_pixel_shader) * 0,
		sizeof(DX9::s_rasterizer_dx9_pixel_shader) * 1
	};

	static uint32			g_current_shader_offset = 0;
	static _enum			g_extension_usage_mask;

	void SetShaderEffectVariables(const TagGroups::s_shader_effect* shader);

	API_FUNC_NAKED void Hook_ShaderEffect_DrawEffect()
	{
		static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_EFFECT_DRAW_EFFECT_RETN);

		_asm{
			push	ecx
			push	edx

			mov		eax, [ebx + 0Ch]
			push	eax
			call	SetShaderEffectVariables;
			
			pop		edx
			pop		ecx

			jmp		RETN_ADDRESS;
		};
	}

	API_FUNC_NAKED void Hook_ShaderEffectPixelShaderOffset()
	{
		static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_EFFECT_PS_INDEX_NO_RETN);

		_asm{
			push	ebx
			add		eax, g_current_shader_offset
			pop		ebx

			mov		eax, [eax + 80h]
			jmp		RETN_ADDRESS
		};
	}

	static const uint32 SetupShaderEffect(const TagGroups::s_shader_effect& shader, s_pixel_shader_variables& variables)
	{
		_enum feature_usage = shader.shader.extension_usage & g_extension_usage_mask;

		if(TEST_FLAG(feature_usage, Flags::_shader_extension_usage_bit_depth_fade))
		{
			if(shader.effect.shader_extension.Count == 1)
			{
				// If the shader has a shader extension block set the effect softness from the extension block
				auto& extension = shader.effect.shader_extension[0];
		
				variables.m_far_clip_distance = Render::RenderGlobals()->frustum.z_far;
				variables.m_near_clip_distance = Render::RenderGlobals()->frustum.z_near;
				variables.m_depth_fade_distance = extension.depth_fade_distance;
				variables.m_camera_fade_distance = extension.camera_fade_distance;

				DX9::Direct3DDevice()->SetTexture(4, DX9::c_gbuffer_system::GBuffer().m_rt_depth.texture);

				return g_feature_mix_list[1];
			}
		}
		
		return g_feature_mix_list[0];
	}
	
	void SetShaderEffectVariables(const TagGroups::s_shader_effect* shader)
	{
		g_current_shader_offset = 0;

		if((g_ps_support <= _ps_support_2_0) || !g_extensions_enabled)
		{
			return;
		}

		if(!DX9::c_gbuffer_system::g_system_enabled || !DX9::c_gbuffer_system::GBuffer().m_rt_depth.IsEnabled())
		{
			return;
		}

		s_pixel_shader_variables pixel_shader_variables = { 0.0f, 0.0f };

		g_current_shader_offset = SetupShaderEffect(*shader, pixel_shader_variables);

		// Set the custom pixel shader values
		DX9::Direct3DDevice()->SetPixelShaderConstantF(8, (float*)&pixel_shader_variables, 1);
	}

	void ApplyHooks()
	{
		Memory::WriteRelativeJmp(&Hook_ShaderEffectPixelShaderOffset, GET_FUNC_VPTR(RASTERIZER_EFFECT_PS_INDEX_NO_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_ShaderEffect_DrawEffect, GET_FUNC_VPTR(RASTERIZER_EFFECT_DRAW_EFFECT_HOOK), true);
	}
};