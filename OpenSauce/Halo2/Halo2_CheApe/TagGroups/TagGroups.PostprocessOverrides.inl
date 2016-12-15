/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\Halo2_CheApe for specific license information
*/

namespace TagGroups
{
	//////////////////////////////////////////////////////////////////////////
	// shader_fill
	static API_FUNC_NAKED bool shader_fill_postprocess_definition(datum_index shader_index, tag_block& global_shader_parameters,
		datum_index shader_template_index, datum_index shader_light_response_index, 
		tag_block& shader_properties, tag_block& shader_postprocess_definition)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(SHADER_FILL_POSTPROCESS_DEFINITION);

		API_FUNC_NAKED_START()
			push	shader_postprocess_definition
			push	shader_properties
			push	shader_light_response_index
			push	shader_template_index
			push	global_shader_parameters
			push	shader_index
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(6);
	}
	static bool PLATFORM_API shader_fill_postprocess_definition_override(datum_index shader_index, tag_block& global_shader_parameters,
		datum_index shader_template_index, datum_index shader_light_response_index, 
		tag_block& shader_properties, tag_block& shader_postprocess_definition)
	{
		if(shader_postprocess_definition.count != 1)
			return shader_fill_postprocess_definition(shader_index, global_shader_parameters, 
				shader_template_index, shader_light_response_index, shader_properties, shader_postprocess_definition);

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// shader
	static API_FUNC_NAKED void shader_postprocess(datum_index owner_tag_index, tag_block& global_shader_parameters, 
		datum_index shader_template_index, tag_block& shader_postprocess_definition)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(SHADER_POSTPROCESS);

		API_FUNC_NAKED_START()
			push	shader_postprocess_definition
			push	shader_template_index
			push	global_shader_parameters
			push	owner_tag_index
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(4);
	}
	static void PLATFORM_API shader_postprocess_override(datum_index owner_tag_index, tag_block& global_shader_parameters, 
		datum_index shader_template_index, tag_block& shader_postprocess_definition)
	{
		if(shader_postprocess_definition.count != 1)
			shader_postprocess(owner_tag_index, global_shader_parameters, 
				shader_template_index, shader_postprocess_definition);
	}

	//////////////////////////////////////////////////////////////////////////
	// shader_template
	static API_FUNC_NAKED void shader_template_platform_postprocess(datum_index shader_template_index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(SHADER_TEMPLATE_PLATFORM_POSTPROCESS);

		API_FUNC_NAKED_START()
			push	shader_template_index
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}
	static void PLATFORM_API shader_template_platform_postprocess_override(datum_index shader_template_index)
	{
		s_shader_template_definition* stem = tag_get<s_shader_template_definition>(shader_template_index);

		if(stem->postprocess_definition.count != 1)
			shader_template_platform_postprocess(shader_template_index);
	}

	//////////////////////////////////////////////////////////////////////////
	// shader_pass
	static API_FUNC_NAKED void shader_pass_platform_postprocess(datum_index shader_pass_index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(SHADER_PASS_PLATFORM_POSTPROCESS);

		API_FUNC_NAKED_START()
			push	shader_pass_index
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}
	static void PLATFORM_API shader_pass_platform_postprocess_override(datum_index shader_pass_index)
	{
		s_shader_pass_definition* spas = tag_get<s_shader_pass_definition>(shader_pass_index);

		if(spas->postprocess_definition.count != 1)
			shader_pass_platform_postprocess(shader_pass_index);
	}

	//////////////////////////////////////////////////////////////////////////
	// sound_effect_template
	static proc_tag_group_postprocess sound_effect_template_postprocess;
	bool PLATFORM_API sound_effect_template_postprocess_override(datum_index tag_index, bool is_new)
	{
		s_sound_effect_template_collection* sfx = tag_get<s_sound_effect_template_collection>(tag_index);

		if(sfx->platform_definition.count != 1 && sound_effect_template_postprocess != NULL)
			return sound_effect_template_postprocess(tag_index, is_new);

		return true;
	}


	void PostprocessOverridesInitialize()
	{
		Memory::WriteRelativeCall(shader_fill_postprocess_definition_override,	GET_FUNC_VPTR(SHADER_FILL_POSTPROCESS_DEFINITION_CALL));
		Memory::WriteRelativeCall(shader_postprocess_override,					GET_FUNC_VPTR(SHADER_POSTPROCESS_CALL_PARTICLE_GROUP_POSTPROCESS));
		Memory::WriteRelativeCall(shader_template_platform_postprocess_override,GET_FUNC_VPTR(SHADER_TEMPLATE_PLATFORM_POSTPROCESS_CALL));
		Memory::WriteRelativeCall(shader_pass_platform_postprocess_override,	GET_FUNC_VPTR(SHADER_PASS_PLATFORM_POSTPROCESS_CALL));

		{
			tag_group* sfx_group = tag_group_get<s_sound_effect_template_collection>();
			sound_effect_template_postprocess = sfx_group->postprocess_proc;
			sfx_group->postprocess_proc = sound_effect_template_postprocess_override;
		}
	}
	void PostprocessOverridesDispose()
	{
	}
};