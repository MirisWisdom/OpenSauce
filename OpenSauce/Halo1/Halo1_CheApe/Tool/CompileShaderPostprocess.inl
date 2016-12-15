/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

void		WriteD3DXErrors(LPD3DXBUFFER error_buffer, int32 error_count)
{
	if(!error_buffer)
	{
		YELO_WARN("\tNO D3DX ERRORS?");
		return;
	}
	std::string error_string(CAST_PTR(char*, error_buffer->GetBufferPointer()));
	std::string error_line;

	// iterate through the error string, printing each line
	int count = 0;
	while(StringEditing::GetStringSegment(error_string, error_line, nullptr, "\n")
		&& count < error_count )
	{
		auto& profile = Settings::c_settings_cheape::Profile();
		// if the line contains the data path, remove it
		std::string::size_type data_string_start = std::string::npos;
		if ((data_string_start = error_line.find(profile.GetDataPath())) != std::string::npos)
		{
			error_line.replace(0, strlen(profile.GetDataPath()), "");
		}

		YELO_WARN("\t%s",
			error_line.c_str());

		// remove the line to move
		StringEditing::RemoveStringSegment(error_string, nullptr, "\n");
		count++;
	}
}

HRESULT		SetShaderData(
	const LPD3DXBUFFER& effect_buffer,
	TagGroups::s_shader_postprocess_definition* shader_tag)
{
	HRESULT hr = S_OK;

	// allocate memory for the new binary data
	int32 shader_code_binary_size = CAST(int32, effect_buffer->GetBufferSize());
	if (!blam::tag_data_resize(&shader_tag->shader_code_binary, shader_code_binary_size))
	{				
		YELO_WARN("OS_tool: failed to allocate memory for binary data");
		hr = E_FAIL;
	}

	if(SUCCEEDED(hr))
	{
		// copy the shader to the binary data field
		errno_t error_num = memcpy_s(shader_tag->shader_code_binary.address, 
			effect_buffer->GetBufferSize(),
			effect_buffer->GetBufferPointer(), 
			effect_buffer->GetBufferSize());
		if(error_num != 0)
		{
			YELO_WARN("OS_tool: failed to copy shader data to tag binary data field");
			hr = E_FAIL;
		}		
	}
	shader_tag->flags.shader_is_binary_bit = true;

	return hr;
}

HRESULT		AddPassBlock(LPD3DXEFFECTCOMPILER& compiler, D3DXHANDLE pass, 
	TagBlock<TagGroups::s_pass_definition>& block)
{
	HRESULT success = S_OK;

	// get the pass information
	D3DXPASS_DESC pass_desc;
	if(FAILED(success = compiler->GetPassDesc(pass, &pass_desc)))
		return success;

	TagGroups::s_pass_definition& pass_block = *blam::tag_block_add_and_get_element(block);

	// set the blocks fields
	strncpy_s(pass_block.name, pass_desc.Name, Enums::k_tag_string_length);

	D3DXHANDLE handle;
	// should the target be cleared prior to this pass
	BOOL clear_target;
	if(handle = compiler->GetAnnotationByName(pass, "clear_target"))
		compiler->GetBool(handle, &clear_target);
	else
		clear_target = false;

	pass_block.flags.clear_target_bit = clear_target;

	// should the scene be copied to the target on this pass
	BOOL copy_scene_to_target;
	if(handle = compiler->GetAnnotationByName(pass, "copy_scene_to_target"))
		compiler->GetBool(handle, &copy_scene_to_target);
	else
		copy_scene_to_target = false;

	pass_block.flags.copy_scene_to_target_bit = copy_scene_to_target;

	// which chain should this pass render to
	INT render_chain;
	if(handle = compiler->GetAnnotationByName(pass, "render_chain"))
		compiler->GetInt(handle, &render_chain);
	else
		render_chain = 0;

	pass_block.render_chain = (Yelo::_enum)render_chain;

	// should the buffer be cleared on this pass
	BOOL clear_buffer_texture;
	if(handle = compiler->GetAnnotationByName(pass, "clear_buffer_texture"))
		compiler->GetBool(handle, &clear_buffer_texture);
	else
		clear_buffer_texture = false;

	pass_block.flags.clear_buffer_texture_bit = clear_buffer_texture;

	return success;
}

HRESULT		AddTechniqueBlock(LPD3DXEFFECTCOMPILER& compiler, D3DXHANDLE technique, 
	TagBlock<TagGroups::s_technique_definition>& block)
{
	HRESULT success = S_OK;

	// get the technique information
	D3DXTECHNIQUE_DESC technique_desc;
	if(FAILED(success = compiler->GetTechniqueDesc(technique, &technique_desc)))
		return success;

	TagGroups::s_technique_definition& technique_block = *blam::tag_block_add_and_get_element(block);

	// set the blocks fields
	strncpy_s(technique_block.name, technique_desc.Name, Yelo::Enums::k_tag_string_length);

	D3DXHANDLE handle;

	INT shader_model_mask;
	if(handle = compiler->GetAnnotationByName(technique, "shader_model_mask"))
		compiler->GetInt(handle, &shader_model_mask);
	else
		shader_model_mask = Flags::_shader_model_mask_1_0 | Flags::_shader_model_mask_2_0 | Flags::_shader_model_mask_3_0;

	technique_block.shader_model.sm_1_0_bit = (shader_model_mask & Flags::_shader_model_mask_1_0) == Flags::_shader_model_mask_1_0;
	technique_block.shader_model.sm_2_0_bit = (shader_model_mask & Flags::_shader_model_mask_2_0) == Flags::_shader_model_mask_2_0;
	technique_block.shader_model.sm_3_0_bit = (shader_model_mask & Flags::_shader_model_mask_3_0) == Flags::_shader_model_mask_3_0;

	// add passes to the technique block
	for(UINT i = 0; i < technique_desc.Passes; i++)
	{
		D3DXHANDLE pass = compiler->GetPass(technique, i);

		if(FAILED(success = AddPassBlock(compiler, pass, technique_block.passes)))
			break;
	}

	return success;
}
HRESULT		SetTechniqueBlocks(
	LPD3DXEFFECTCOMPILER& compiler,
	TagGroups::s_shader_postprocess_definition* shader_tag)
{
	HRESULT success = E_FAIL;

	// get technique information
	D3DXEFFECT_DESC effect_desc;
	success = compiler->GetDesc(&effect_desc);

	// add technique blocks
	for(uint32 i = 0; (i < effect_desc.Techniques) && (i < 3) && (SUCCEEDED(success)); i++)
	{
		D3DXHANDLE technique = compiler->GetTechnique(i);

		success = AddTechniqueBlock(compiler, technique, shader_tag->techniques);
	}
	return success;
}

void		ClearTagData(TagGroups::s_shader_postprocess_definition* shader_tag)
{
	// remove any existing binary data
	TagGroups::tag_data_delete(&shader_tag->shader_code_binary);

	// remove any existing technique blocks
	tag_block_delete_all_elements(shader_tag->techniques);
}

HRESULT		CompileEffect(
	const LPD3DXEFFECTCOMPILER& effect_compiler,
	LPD3DXBUFFER& effect_buffer)
{
	printf_s("compiling postprocess effect...");

	// compile postprocess shader into local variable
	LPD3DXBUFFER error_buffer = nullptr;

	HRESULT hr = effect_compiler->CompileEffect(
		D3DXSHADER_OPTIMIZATION_LEVEL3,
		&effect_buffer,
		&error_buffer);

	// copy value to effect_buffer argument if succeeded
	if(SUCCEEDED(hr))
		puts("done");
	else
	{
		puts("failed");
		// inform the user that an error occurred and print the first two D3DX errors
		YELO_WARN("OS_tool: failed to compile effect (%X)", hr);
		WriteD3DXErrors(error_buffer, 3);
		safe_release(effect_buffer);
	}
	safe_release(error_buffer);
	return hr;
}

HRESULT		SetPostprocessTag(
	LPD3DXEFFECTCOMPILER& compiler,
	TagGroups::s_shader_postprocess_definition* shader_tag)
{
	HRESULT success;

	LPD3DXBUFFER effect_buffer = nullptr;
	do
	{
		// compile the effect
		if(FAILED(CompileEffect(compiler, effect_buffer))) break;

		ClearTagData(shader_tag);

		if(FAILED(success = SetShaderData(effect_buffer, shader_tag))) break;
		if(FAILED(success = SetTechniqueBlocks(compiler, shader_tag))) break;
	}while(false);

	safe_release(effect_buffer);

	return success;
}
HRESULT		CreateEffectCompiler(
	LPD3DXEFFECTCOMPILER& effect_compiler, 
	const TagGroups::s_shader_postprocess_definition* shader_tag,
	const char* fx_file)
{
	printf_s("creating effect compiler...");

	// create effect compiler
	LPD3DXBUFFER error_buffer = nullptr;
	
	HRESULT hr = D3DXCreateEffectCompilerFromFile(
		fx_file,
		nullptr,
		nullptr,
		D3DXSHADER_OPTIMIZATION_LEVEL3,
		&effect_compiler,
		&error_buffer);

	// copy value to effect_compiler argument if succeeded
	if(SUCCEEDED(hr))
		puts("done");
	else
	{
		puts("failed");
		// inform the user that an error occurred and print the first three D3DX errors
		YELO_WARN("OS_tool: failed to create effect compiler (%X)", hr);
		YELO_WARN("OS_tool: %s", fx_file);
		WriteD3DXErrors(error_buffer, 3);
		safe_release(effect_compiler);
	}
	safe_release(error_buffer);
	return hr;
}

HRESULT		LoadShader(
	const char* tag_path,
	datum_index& tag_index, 
	TagGroups::s_shader_postprocess_definition*& shader_tag)
{
	// attempt to load the post process shader tag using each type until
	// we find a shoe that fits
	do
	{
		// add any new formats based upon shpp here
		tag_index = blam::tag_load<TagGroups::s_shader_postprocess_generic>(
			tag_path, FLAG(Flags::_tag_load_from_file_system_bit));
		if(!tag_index.IsNull()) break;
	}
	while(false);
	
	// if no tag is found, create a new one
	//TODO: add an argument to the command to state the tag type to create, defaulting to generic
	if(tag_index.IsNull())
	{
		printf_s("creating tag \"%s.shader_postprocess_generic\"\n", tag_path);
		tag_index = blam::tag_new<TagGroups::s_shader_postprocess_generic>(tag_path);
	}
	else
		printf_s("updating tag \"%s\"\n", tag_path);

	HRESULT hr = (tag_index.IsNull() ? E_FAIL : S_OK);

	if(SUCCEEDED(hr))
	{
		shader_tag = blam::tag_get<TagGroups::s_shader_postprocess_definition>(tag_index);
		hr = (!shader_tag ? E_FAIL : S_OK);
	}
	else
	{		
		YELO_WARN("OS_tool: failed to open/create postprocess tag (%X)", hr);
	}
	return hr;
}

HRESULT		SaveShader(
	const datum_index& tag_index)
{
	printf_s("saving shader...");

	bool save_succeeded = blam::tag_save(tag_index);
	if(save_succeeded)
		puts("done");
	else
	{
		puts("failed");
		YELO_WARN("OS_tool: failed to save postprocess tag");
	}
	return (save_succeeded ? S_OK : E_FAIL);
}