/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
namespace Model
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER_SHADEREXTENSION
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_SHADEREXTENSION_MODEL
#include "Memory/_EngineLayout.inl"

	struct s_shader_feature_mix {
		const _enum		feature_mask;
		PAD16;
		const uint32	shader_index_offset;
		const uint32	shader_index_half_offset;
		const char		feature_mix_id[64];
	};

	struct s_vertex_shader_variables {
		real_vector2d	detail_normal_1_scale;
		real_vector2d	detail_normal_2_scale;
	};

	struct s_pixel_shader_variables {
		real base_normal_map_coefficient;
		real base_normal_map_z_coefficient;
		real specular_color_map_exponent;
		real specular_color_map_coefficient;

		real detail_normal_map_1_interpolation;
		real detail_normal_map_2_interpolation;
		real detail_normal_map_1_coefficient;
		real detail_normal_map_2_coefficient;

		PAD32;
		PAD32;
		//real specular_reflection_exponent;
		//real specular_reflection_coefficient;
		real specular_lighting_exponent;
		real specular_lighting_coefficient;
	};

	static const t_shader_usage_id g_base_shader_list[] =
	{
		"MaskDetailBeforeReflectionBiasedMultiply",
		"MaskDetailBeforeReflectionMultiply",
		"MaskDetailBeforeReflectionBiasedAdd",
		"MaskDetailAfterReflectionBiasedMultiply",
		"MaskDetailAfterReflectionMultiply",
		"MaskDetailAfterReflectionBiasedAdd",
		"MaskInverseDetailBeforeReflectionBiasedMultiply",
		"MaskInverseDetailBeforeReflectionMultiply",
		"MaskInverseDetailBeforeReflectionBiasedAdd",
		"MaskInverseDetailAfterReflectionBiasedMultiply",
		"MaskInverseDetailAfterReflectionMultiply",
		"MaskInverseDetailAfterReflectionBiasedAdd",

		"MaskDetailBeforeReflectionBiasedMultiplyComplexFog",
		"MaskDetailBeforeReflectionMultiplyComplexFog",
		"MaskDetailBeforeReflectionBiasedAddComplexFog",
		"MaskDetailAfterReflectionBiasedMultiplyComplexFog",
		"MaskDetailAfterReflectionMultiplyComplexFog",
		"MaskDetailAfterReflectionBiasedAddComplexFog",
		"MaskInverseDetailBeforeReflectionBiasedMultiplyComplexFog",
		"MaskInverseDetailBeforeReflectionMultiplyComplexFog",
		"MaskInverseDetailBeforeReflectionBiasedAddComplexFog",
		"MaskInverseDetailAfterReflectionBiasedMultiplyComplexFog",
		"MaskInverseDetailAfterReflectionMultiplyComplexFog",
		"MaskInverseDetailAfterReflectionBiasedAddComplexFog",
	};

	s_shader_feature_mix g_feature_mix_list[] = {
		{
			Enums::_model_extension_usage_none,
			NUMBEROF(g_base_shader_list) * 0,
			(NUMBEROF(g_base_shader_list) / 2) * 0,
			""
		},
		{
			Enums::_model_extension_usage_normal_map,
			NUMBEROF(g_base_shader_list) * 1,
			(NUMBEROF(g_base_shader_list) / 2) * 1,
			"BaseNorm"
		},
		{
			Enums::_model_extension_usage_normal_map | Enums::_model_extension_usage_specular_map,
			NUMBEROF(g_base_shader_list) * 2,
			(NUMBEROF(g_base_shader_list) / 2) * 2,
			"BaseNormSpecMap"
		},
		{
			Enums::_model_extension_usage_normal_map | Enums::_model_extension_usage_specular_lighting,
			NUMBEROF(g_base_shader_list) * 3,
			(NUMBEROF(g_base_shader_list) / 2) * 3,
			"BaseNormSpecLighting"
		},
		{
			Enums::_model_extension_usage_normal_map | Enums::_model_extension_usage_specular_map | Enums::_model_extension_usage_specular_lighting,
			NUMBEROF(g_base_shader_list) * 4,
			(NUMBEROF(g_base_shader_list) / 2) * 4,
			"BaseNormSpecMapSpecLighting"
		},
		{
			Enums::_model_extension_usage_normal_map | Enums::_model_extension_usage_detail_normal,
			NUMBEROF(g_base_shader_list) * 5,
			(NUMBEROF(g_base_shader_list) / 2) * 5,
			"BaseNormDetailNorm"
		},
		{
			Enums::_model_extension_usage_normal_map | Enums::_model_extension_usage_detail_normal | Enums::_model_extension_usage_specular_map,
			NUMBEROF(g_base_shader_list) * 6,
			(NUMBEROF(g_base_shader_list) / 2) * 6,
			"BaseNormDetailNormSpecMap"
		},
		{
			Enums::_model_extension_usage_normal_map | Enums::_model_extension_usage_detail_normal | Enums::_model_extension_usage_specular_lighting,
			NUMBEROF(g_base_shader_list) * 7,
			(NUMBEROF(g_base_shader_list) / 2) * 7,
			"BaseNormDetailNormSpecLighting"
		},
		{
			Enums::_model_extension_usage_normal_map | Enums::_model_extension_usage_detail_normal | Enums::_model_extension_usage_specular_map | Enums::_model_extension_usage_specular_lighting,
			NUMBEROF(g_base_shader_list) * 8,
			(NUMBEROF(g_base_shader_list) / 2) * 8,
			"BaseNormDetailNormSpecMapSpecLighting"
		},
		{
			Enums::_model_extension_usage_detail_normal,
			NUMBEROF(g_base_shader_list) * 9,
			(NUMBEROF(g_base_shader_list) / 2) * 9,
			"DetailNorm"
		},
		{
			Enums::_model_extension_usage_detail_normal | Enums::_model_extension_usage_specular_map,
			NUMBEROF(g_base_shader_list) * 10,
			(NUMBEROF(g_base_shader_list) / 2) * 10,
			"DetailNormSpecMap"
		},
		{
			Enums::_model_extension_usage_detail_normal | Enums::_model_extension_usage_specular_lighting,
			NUMBEROF(g_base_shader_list) * 11,
			(NUMBEROF(g_base_shader_list) / 2) * 11,
			"DetailNormSpecLighting"
		},
		{
			Enums::_model_extension_usage_detail_normal | Enums::_model_extension_usage_specular_map | Enums::_model_extension_usage_specular_lighting,
			NUMBEROF(g_base_shader_list) * 12,
			(NUMBEROF(g_base_shader_list) / 2) * 12,
			"DetailNormSpecMapSpecLighting"
		},
		{
			Enums::_model_extension_usage_specular_map,
			NUMBEROF(g_base_shader_list) * 13,
			(NUMBEROF(g_base_shader_list) / 2) * 13,
			"SpecMap"
		},
		{
			Enums::_model_extension_usage_specular_map | Enums::_model_extension_usage_specular_lighting,
			NUMBEROF(g_base_shader_list) * 14,
			(NUMBEROF(g_base_shader_list) / 2) * 14,
			"SpecMapSpecLighting"
		},
		{
			Enums::_model_extension_usage_specular_lighting,
			NUMBEROF(g_base_shader_list) * 15,
			(NUMBEROF(g_base_shader_list) / 2) * 15,
			"SpecLighting"
		},
	};

	static t_shader_usage_id g_shader_usage_id_list[NUMBEROF(g_base_shader_list) * NUMBEROF(g_feature_mix_list)];

	static IDirect3DPixelShader9* g_environment_no_pixel_shaders[NUMBEROF(g_shader_usage_id_list) / 2];
	static IDirect3DPixelShader9* g_self_illumination_pixel_shaders[NUMBEROF(g_shader_usage_id_list)];
	static IDirect3DPixelShader9* g_change_color_pixel_shaders[NUMBEROF(g_shader_usage_id_list)];
	static IDirect3DPixelShader9* g_multipurpose_pixel_shaders[NUMBEROF(g_shader_usage_id_list)];
	static IDirect3DPixelShader9* g_reflection_pixel_shaders[NUMBEROF(g_shader_usage_id_list)];
	static IDirect3DPixelShader9* g_no_pixel_shaders[NUMBEROF(g_shader_usage_id_list) / 2];

	static s_shader_feature_mix*		g_current_feature_mix = &g_feature_mix_list[0];
	static s_vertex_shader_variables	g_vertex_shader_variables;
	static s_pixel_shader_variables		g_pixel_shader_variables;
	static _enum						g_extension_usage_mask;

	// hooks to correctly skip past inverse shaders in the usage id list
	API_FUNC_NAKED static void Hook_EnvironmentNoPixelShaderIDOffset()
	{
		static uint32 RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_ENVIRONMENT_NO_USAGE_ID_OFFSET_RETN);

		_asm{
			// preserve ecx and edx
			push	ecx
			push	edx

			// move  the shader index to eax
			mov		eax, esi

			// divide eax by 6 to get the offset multiplier
			xor		edx, edx
			mov		ecx, 6
			div		ecx

			// multiply eax by 300h to get the shader id byte offset in ecx
			imul	ecx, eax, 300h
			lea		eax, [edi+ecx]

			pop		edx
			pop		ecx
			jmp		RETN_ADDRESS
		};
	}
	API_FUNC_NAKED static void Hook_NoPixelShaderIDOffset()
	{
		static uint32 RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_NO_USAGE_ID_OFFSET_RETN);

		_asm{
			// preserve ecx and edx
			push	ecx
			push	edx

			// move  the shader index to eax
			mov		eax, esi

			// divide eax by 6 to get the offset multiplier
			xor		edx, edx
			mov		ecx, 6
			div		ecx

			// multiply eax by 300h to get the shader id byte offset in ecx
			imul	ecx, eax, 300h
			lea		eax, [edi+ecx]

			pop		edx
			pop		ecx
			jmp		RETN_ADDRESS
		};
	}
	// hooks to return the new pixel shaders when applicable by offseting the shader index
	API_FUNC_NAKED static void Hook_EnvironmentNoPixelShader()
	{
		static uint32 RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_PS_INDEX_ENVIRONMENT_NO_RETN);

		_asm {
			push	ebx
			mov		ebx, g_current_feature_mix
			test	ebx, ebx
			jz		no_extension
			add		ecx, [ebx + 8]
no_extension:
			pop		ebx
			mov		edi, g_environment_no_pixel_shaders[ecx * 4]
			jmp		RETN_ADDRESS
		};
	}
	API_FUNC_NAKED static void Hook_EnvironmentNoInversePixelShader()
	{
		static uint32 RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_PS_INDEX_ENVIRONMENT_NO_INV_RETN);

		_asm {
			push	ebx
			mov		ebx, g_current_feature_mix
			test	ebx, ebx
			jz		no_extension
			add		eax, [ebx + 8]
no_extension:
			pop		ebx
			mov		edi, g_environment_no_pixel_shaders[18h + eax * 4]
			jmp		RETN_ADDRESS
		};
	}
	API_FUNC_NAKED static void Hook_ChangeColorPixelShader()
	{
		static uint32 RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_PS_INDEX_CHANGE_COLOR_RETN);

		_asm {
			push	ebx
			mov		ebx, g_current_feature_mix
			test	ebx, ebx
			jz		no_extension
			add		esi, [ebx + 4]
no_extension:
			pop		ebx
			mov		eax, g_change_color_pixel_shaders[esi * 4]
			jmp		RETN_ADDRESS
		};
	}	
	API_FUNC_NAKED static void Hook_ChangeColorInversePixelShader()
	{
		static uint32 RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_PS_INDEX_CHANGE_COLOR_INV_RETN);

		_asm {
			push	ebx
			mov		ebx, g_current_feature_mix
			test	ebx, ebx
			jz		no_extension
			add		esi, [ebx + 4]
no_extension:
			pop		ebx
			mov		eax, g_change_color_pixel_shaders[18h + esi * 4]
			jmp		RETN_ADDRESS
		};
	}	
	API_FUNC_NAKED static void Hook_MultipurposePixelShader()
	{
		static uint32 RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_PS_INDEX_MULTIPURPOSE_RETN);

		_asm {
			push	ebx
			mov		ebx, g_current_feature_mix
			test	ebx, ebx
			jz		no_extension
			add		esi, [ebx + 4]
no_extension:
			pop		ebx
			mov		eax, g_multipurpose_pixel_shaders[esi * 4]
			jmp		RETN_ADDRESS
		};
	}	
	API_FUNC_NAKED static void Hook_MultipurposeInversePixelShader()
	{
		static uint32 RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_PS_INDEX_MULTIPURPOSE_INV_RETN);

		_asm {
			push	ebx
			mov		ebx, g_current_feature_mix
			test	ebx, ebx
			jz		no_extension
			add		esi, [ebx + 4]
no_extension:
			pop		ebx
			mov		eax, g_multipurpose_pixel_shaders[18h + esi * 4]
			jmp		RETN_ADDRESS
		};
	}
	API_FUNC_NAKED static void Hook_NoPixelShader()
	{
		static uint32 RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_PS_INDEX_NO_RETN);

		_asm {
			push	ebx
			mov		ebx, g_current_feature_mix
			test	ebx, ebx
			jz		no_extension
			add		esi, [ebx + 8]
no_extension:
			pop		ebx
			mov		eax, g_no_pixel_shaders[esi * 4]
			jmp		RETN_ADDRESS
		};
	}	
	API_FUNC_NAKED static void Hook_ReflectionPixelShader()
	{
		static uint32 RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_PS_INDEX_REFLECTION_RETN);

		_asm {
			push	ebx
			mov		ebx, g_current_feature_mix
			test	ebx, ebx
			jz		no_extension
			add		esi, [ebx + 4]
no_extension:
			pop		ebx
			mov		eax, g_reflection_pixel_shaders[esi * 4]
			jmp		RETN_ADDRESS
		};
	}
	API_FUNC_NAKED static void Hook_ReflectionInversePixelShader()
	{
		static uint32 RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_PS_INDEX_REFLECTION_INV_RETN);

		_asm {
			push	ebx
			mov		ebx, g_current_feature_mix
			test	ebx, ebx
			jz		no_extension
			add		esi, [ebx + 4]
no_extension:
			pop		ebx
			mov		eax, g_reflection_pixel_shaders[18h + esi * 4]
			jmp		RETN_ADDRESS
		};
	}
	API_FUNC_NAKED static void Hook_SelfIlluminationPixelShader()
	{
		static uint32 RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_PS_INDEX_SELF_ILLUMINATION_RETN);

		_asm {
			push	ebx
			mov		ebx, g_current_feature_mix
			test	ebx, ebx
			jz		no_extension
			add		esi, [ebx + 4]
no_extension:
			pop		ebx
			mov		eax, g_self_illumination_pixel_shaders[esi * 4]
			jmp		RETN_ADDRESS
		};
	}	
	API_FUNC_NAKED static void Hook_SelfIlluminationInversePixelShader()
	{
		static uint32 RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_PS_INDEX_SELF_ILLUMINATION_INV_RETN);

		_asm {
			push	ebx
			mov		ebx, g_current_feature_mix
			test	ebx, ebx
			jz		no_extension
			add		esi, [ebx + 4]
no_extension:
			pop		ebx
			mov		eax, g_self_illumination_pixel_shaders[18h + esi * 4]
			jmp		RETN_ADDRESS
		};
	}	
	API_FUNC_NAKED static void Hook_RenderObject_ForceInvertBackfaceNormals()
	{
		static uint32 RETN_ADDRESS = GET_FUNC_PTR(RASTERIZER_MODEL_DRAW_INVERT_BACKFACE_NORMALS_CHECK_RETN);

		_asm{
			mov     al, 1
			test    al, al

			jmp		RETN_ADDRESS
		};
	}

	void PLATFORM_API SetModelNormSpec(void* shader_pointer)
	{
		// reset to defaults
		g_current_feature_mix = NULL;

		memset(&g_vertex_shader_variables, 0, sizeof(g_vertex_shader_variables));

		g_pixel_shader_variables.base_normal_map_coefficient = 1.0f;
		g_pixel_shader_variables.base_normal_map_z_coefficient = 1.0f;
		g_pixel_shader_variables.specular_color_map_exponent = 1.0f;
		g_pixel_shader_variables.specular_color_map_coefficient = 1.0f;

		g_pixel_shader_variables.detail_normal_map_1_interpolation = 0.0f;
		g_pixel_shader_variables.detail_normal_map_2_interpolation = 0.0f;
		g_pixel_shader_variables.detail_normal_map_1_coefficient = 1.0f;
		g_pixel_shader_variables.detail_normal_map_2_coefficient = 1.0f;

		//g_pixel_shader_variables.specular_reflection_exponent = 1.0f;
		//g_pixel_shader_variables.specular_reflection_coefficient = 1.0f;
		g_pixel_shader_variables.specular_lighting_exponent = 1.0f;
		g_pixel_shader_variables.specular_lighting_coefficient = 0.0f;

		TagGroups::s_shader_definition* shader_base = 
			CAST_PTR(TagGroups::s_shader_definition*, shader_pointer);

		DX9::c_gbuffer_system::OutputObjectTBN() = false;

		_enum feature_usage = shader_base->shader.extension_usage;

		// disable feature as per the users settings
		feature_usage &= g_extension_usage_mask;

		// disable specular effects when they are disabled on the environment
		if(!DebugOptions()->environment_specular_lights)
			feature_usage &= Enums::_model_extension_usage_normal_map | Enums::_model_extension_usage_detail_normal;

		for(int i = 0; i < NUMBEROF(g_feature_mix_list); i++)
			if(g_feature_mix_list[i].feature_mask == feature_usage)
				g_current_feature_mix = &g_feature_mix_list[i];

		if(shader_base->shader.shader_type == Enums::_shader_type_model)
		{
			TagGroups::s_shader_model_definition* shader_model = CAST_PTR(TagGroups::s_shader_model_definition*, shader_base);


			if(shader_model->model.maps.shader_extension.Count == 1)
			{
				const TagGroups::s_shader_model_extension& extension = shader_model->model.maps.shader_extension[0];

				// maps must have been built with build-cache-file-ex for the usage enum to be set

				// base normal map is present
				if(feature_usage & Enums::_model_extension_usage_normal_map)
				{
					DX9::c_gbuffer_system::OutputObjectTBN() = true;
					SetTexture(Yelo::DX9::Direct3DDevice(), 4, extension.base_normal.map.tag_index);

					g_pixel_shader_variables.base_normal_map_coefficient = extension.base_normal.modifiers.coefficient;

					real z_multiplier = max(1.0f / extension.base_normal.modifiers.coefficient, 0.0f);
					g_pixel_shader_variables.base_normal_map_z_coefficient = min(z_multiplier, 1.0f);
				}

				// either of the detail normal maps are present
				if(feature_usage & Enums::_model_extension_usage_detail_normal)
				{
					// if there is no base normal map then detail normal 1 is the only one present
					// however it might not be present if there is a base normal map so check whether its null
					if(!extension.detail_normals[0].map.tag_index.IsNull())
					{
						SetTexture(Yelo::DX9::Direct3DDevice(), 5, extension.detail_normals[0].map.tag_index);
						g_pixel_shader_variables.detail_normal_map_1_coefficient = extension.detail_normals[0].modifiers.coefficient;

						g_vertex_shader_variables.detail_normal_1_scale.Set(extension.detail_normals[0].modifiers.scale, 
							extension.detail_normals[0].modifiers.scale * extension.detail_normals[0].modifiers.v_scale);

						g_pixel_shader_variables.detail_normal_map_1_interpolation = 1.0f;
					}
					else
						g_pixel_shader_variables.detail_normal_map_1_interpolation = 0.0f;

					// if a base normal map is present either detail normal map may be present so check for the second map
					if(feature_usage & Enums::_model_extension_usage_normal_map)
					{
						if(!extension.detail_normals[1].map.tag_index.IsNull())
						{
							SetTexture(Yelo::DX9::Direct3DDevice(), 6, extension.detail_normals[1].map.tag_index);
							g_pixel_shader_variables.detail_normal_map_2_coefficient = extension.detail_normals[1].modifiers.coefficient;

							g_vertex_shader_variables.detail_normal_2_scale.Set(extension.detail_normals[1].modifiers.scale,
								extension.detail_normals[1].modifiers.scale * extension.detail_normals[1].modifiers.v_scale);

							g_pixel_shader_variables.detail_normal_map_2_interpolation = 1.0f;
						}
						else
							g_pixel_shader_variables.detail_normal_map_2_interpolation = 0.0f;
					}
				}

				// setup specular map
				if(shader_model->shader.extension_usage & Enums::_model_extension_usage_specular_map)
				{
					SetTexture(Yelo::DX9::Direct3DDevice(), 7, extension.specular_color.map.tag_index);

					g_pixel_shader_variables.specular_color_map_coefficient = extension.specular_color.modifiers.coefficient;
					g_pixel_shader_variables.specular_color_map_exponent = extension.specular_color.modifiers.exponent;
				}

				// setup specular lighting
				if(shader_model->shader.extension_usage & Enums::_model_extension_usage_specular_lighting)
				{
					g_pixel_shader_variables.specular_lighting_exponent = extension.specular_lighting_exponent;
					g_pixel_shader_variables.specular_lighting_coefficient = extension.specular_lighting_coefficient;
				}

				// copy the specular colour values to the main colour set to override them
				shader_model->model.reflection_properties.parallel_brightness = extension.parallel_brightness;
				shader_model->model.reflection_properties.parallel_tint_color = extension.parallel_tint_color;
				shader_model->model.reflection_properties.perpendicular_brightness = extension.perpendicular_brightness;
				shader_model->model.reflection_properties.perpendicular_tint_color = extension.perpendicular_tint_color;

				// currently removed as this broke the ambient factor of the specular reflection
				// causing the reflection to disappear at high exponents
				//g_pixel_shader_variables.specular_reflection_exponent = extension.specular_reflection_exponent;
				//g_pixel_shader_variables.specular_reflection_coefficient = extension.specular_reflection_coefficient;

			}
		}
		else if(shader_base->shader.shader_type == Enums::_shader_type_environment)
		{
			TagGroups::s_shader_environment_definition* shader_environment = 
				CAST_PTR(TagGroups::s_shader_environment_definition*, shader_base);

			g_current_feature_mix = NULL;

			// setup base normal map
			if(!shader_environment->environment.bump_map.map.tag_index.IsNull())
			{
				g_current_feature_mix = &g_feature_mix_list[1];

				DX9::c_gbuffer_system::OutputObjectTBN() = true;

				SetTexture(Yelo::DX9::Direct3DDevice(), 4, shader_environment->environment.bump_map.map.tag_index);

				g_pixel_shader_variables.base_normal_map_coefficient = shader_environment->environment.bump_map.scale;

				real z_multiplier = max(1.0f / shader_environment->environment.bump_map.scale, 0.0f);
				g_pixel_shader_variables.base_normal_map_z_coefficient = min(z_multiplier, 1.0f);
			}
			else
				Yelo::DX9::Direct3DDevice()->SetTexture(4, NULL);
		}
		else
		{
			Yelo::DX9::Direct3DDevice()->SetTexture(4, NULL);
			Yelo::DX9::Direct3DDevice()->SetTexture(5, NULL);
			Yelo::DX9::Direct3DDevice()->SetTexture(6, NULL);
			Yelo::DX9::Direct3DDevice()->SetTexture(7, NULL);
		}
		Yelo::DX9::Direct3DDevice()->SetPixelShaderConstantF(0 + k_shader_constant_offset, (float*)&g_pixel_shader_variables, k_vector_4_count);
		Yelo::DX9::Direct3DDevice()->SetVertexShaderConstantF(13, (float*)&g_vertex_shader_variables, 1);
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

	void		BuildShaderIDs()
	{
		for(int i = 0; i < NUMBEROF(g_feature_mix_list); i++)
		{
			for(int j = 0; j < NUMBEROF(g_base_shader_list); j++)
			{
				uint32 id_index = (i * NUMBEROF(g_base_shader_list)) + j;
				strcat_s(g_shader_usage_id_list[id_index], 128, g_base_shader_list[j]);
				strcat_s(g_shader_usage_id_list[id_index], 128, g_feature_mix_list[i].feature_mix_id);
			}
		}
	}

	void		LoadSettings(TiXmlElement* parent_element)
	{
		g_extension_usage_mask = Enums::_model_extension_usage_normal_map | Enums::_model_extension_usage_detail_normal |
			Enums::_model_extension_usage_specular_map | Enums::_model_extension_usage_specular_lighting;

		if(parent_element != NULL)
		{
			int32 usage_mask = Enums::_model_extension_usage_none;
			TiXmlElement* model_element = parent_element->FirstChildElement("Model");

			if(model_element != NULL)
			{
				TiXmlElement* feature_element = NULL;
				char* feature_attribute = NULL;

				if(feature_element = model_element->FirstChildElement("NormalMap"))
					usage_mask |= (Settings::ParseBoolean(feature_element->Attribute("enabled")) ? Enums::_model_extension_usage_normal_map : Enums::_model_extension_usage_none);

				if(feature_element = model_element->FirstChildElement("DetailNormalMaps"))
					usage_mask |= (Settings::ParseBoolean(feature_element->Attribute("enabled")) ? Enums::_model_extension_usage_detail_normal : Enums::_model_extension_usage_none);

				if(feature_element = model_element->FirstChildElement("SpecularMap"))
					usage_mask |= (Settings::ParseBoolean(feature_element->Attribute("enabled")) ? Enums::_model_extension_usage_specular_map : Enums::_model_extension_usage_none);

				if(feature_element = model_element->FirstChildElement("SpecularLighting"))
					usage_mask |= (Settings::ParseBoolean(feature_element->Attribute("enabled")) ? Enums::_model_extension_usage_specular_lighting : Enums::_model_extension_usage_none);

				g_extension_usage_mask &= usage_mask;
			}
		}
	}

	void		SaveSettings(TiXmlElement* parent_element)
	{
		if(parent_element != NULL)
		{
			TiXmlElement* model_element = new TiXmlElement("Model");
			parent_element->LinkEndChild(model_element);

			TiXmlElement* feature_element = NULL;

			feature_element = new TiXmlElement("NormalMap");
			feature_element->SetAttribute("enabled",
				Settings::BooleanToString((g_extension_usage_mask & Enums::_model_extension_usage_normal_map) == Enums::_model_extension_usage_normal_map));
			model_element->LinkEndChild(feature_element);

			feature_element = new TiXmlElement("DetailNormalMaps");
			feature_element->SetAttribute("enabled",
				Settings::BooleanToString((g_extension_usage_mask & Enums::_model_extension_usage_detail_normal) == Enums::_model_extension_usage_detail_normal));
			model_element->LinkEndChild(feature_element);

			feature_element = new TiXmlElement("SpecularMap");
			feature_element->SetAttribute("enabled",
				Settings::BooleanToString((g_extension_usage_mask & Enums::_model_extension_usage_specular_map) == Enums::_model_extension_usage_specular_map));
			model_element->LinkEndChild(feature_element);

			feature_element = new TiXmlElement("SpecularLighting");
			feature_element->SetAttribute("enabled",
				Settings::BooleanToString((g_extension_usage_mask & Enums::_model_extension_usage_specular_lighting) == Enums::_model_extension_usage_specular_lighting));
			model_element->LinkEndChild(feature_element);
		}
	}

	void		ApplyHooks()
	{
		BuildShaderIDs();

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

		// change the shader usage id offsets on effects that only have 12 shaders
		Memory::WriteRelativeJmp(&Hook_EnvironmentNoPixelShaderIDOffset,	GET_FUNC_VPTR(RASTERIZER_MODEL_ENVIRONMENT_NO_USAGE_ID_OFFSET_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_NoPixelShaderIDOffset,				GET_FUNC_VPTR(RASTERIZER_MODEL_NO_USAGE_ID_OFFSET_HOOK), true);
		
		int32 i;
		// change shader usage id pointers to the extended array
		for(i = 0; i < NUMBEROF(K_SHADER_USAGE_ID_ARRAY_REFERENCES); i++)
			*K_SHADER_USAGE_ID_ARRAY_REFERENCES[i] = CAST_PTR(void*, &g_shader_usage_id_list[0][0]);

		// update with the extended shader count
		GET_PTR(RASTERIZER_MODEL_SHADER_LOAD_COUNT) = NUMBEROF(g_shader_usage_id_list);
		GET_PTR(RASTERIZER_MODEL_SHADER_LOAD_NO_INVERSE_COUNT) = NUMBEROF(g_shader_usage_id_list) / 2;

		// redirect references to the original pixel shader arrays to the custom arrays
		for(i = 0; i < NUMBEROF(K_PIXEL_SHADER_REFERENCES_ENVIRONMENT_NO); i++)
			*K_PIXEL_SHADER_REFERENCES_ENVIRONMENT_NO[i] = &g_environment_no_pixel_shaders;
		for(i = 0; i < NUMBEROF(K_PIXEL_SHADER_REFERENCES_CHANGE_COLOR); i++)
			*K_PIXEL_SHADER_REFERENCES_CHANGE_COLOR[i] = &g_change_color_pixel_shaders;
		for(i = 0; i < NUMBEROF(K_PIXEL_SHADER_REFERENCES_MULTIPURPOSE); i++)
			*K_PIXEL_SHADER_REFERENCES_MULTIPURPOSE[i] = &g_multipurpose_pixel_shaders;
		for(i = 0; i < NUMBEROF(K_PIXEL_SHADER_REFERENCES_NO); i++)
			*K_PIXEL_SHADER_REFERENCES_NO[i] = &g_no_pixel_shaders;
		for(i = 0; i < NUMBEROF(K_PIXEL_SHADER_REFERENCES_REFLECTION); i++)
			*K_PIXEL_SHADER_REFERENCES_REFLECTION[i] = &g_reflection_pixel_shaders;
		for(i = 0; i < NUMBEROF(K_PIXEL_SHADER_REFERENCES_SELF_ILLUMINATION); i++)
			*K_PIXEL_SHADER_REFERENCES_SELF_ILLUMINATION[i] = &g_self_illumination_pixel_shaders;

		// change the pixel shader index to return the normal map shaders when appropriate
		Memory::WriteRelativeJmp(&Hook_EnvironmentNoPixelShader, GET_FUNC_VPTR(RASTERIZER_MODEL_PS_INDEX_ENVIRONMENT_NO_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_EnvironmentNoInversePixelShader, GET_FUNC_VPTR(RASTERIZER_MODEL_PS_INDEX_ENVIRONMENT_NO_INV_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_ChangeColorPixelShader, GET_FUNC_VPTR(RASTERIZER_MODEL_PS_INDEX_CHANGE_COLOR_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_ChangeColorInversePixelShader, GET_FUNC_VPTR(RASTERIZER_MODEL_PS_INDEX_CHANGE_COLOR_INV_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_MultipurposePixelShader, GET_FUNC_VPTR(RASTERIZER_MODEL_PS_INDEX_MULTIPURPOSE_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_MultipurposeInversePixelShader, GET_FUNC_VPTR(RASTERIZER_MODEL_PS_INDEX_MULTIPURPOSE_INV_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_NoPixelShader, GET_FUNC_VPTR(RASTERIZER_MODEL_PS_INDEX_NO_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_ReflectionPixelShader, GET_FUNC_VPTR(RASTERIZER_MODEL_PS_INDEX_REFLECTION_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_ReflectionInversePixelShader, GET_FUNC_VPTR(RASTERIZER_MODEL_PS_INDEX_REFLECTION_INV_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_SelfIlluminationPixelShader, GET_FUNC_VPTR(RASTERIZER_MODEL_PS_INDEX_SELF_ILLUMINATION_HOOK), true);
		Memory::WriteRelativeJmp(&Hook_SelfIlluminationInversePixelShader, GET_FUNC_VPTR(RASTERIZER_MODEL_PS_INDEX_SELF_ILLUMINATION_INV_HOOK), true);
	}

	HRESULT		SetVertexShaderConstantF(IDirect3DDevice9* pDevice, UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
	{
		HRESULT hr = S_OK;
		if(g_extensions_enabled)
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
				hr |= pDevice->SetPixelShaderConstantF(4 + k_shader_constant_offset, &pConstantData[2 * 4], 1);
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
};