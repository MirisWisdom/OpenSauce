/*
    Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition
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
void		WriteD3DXErrors(LPD3DXBUFFER error_buffer, int32 error_count)
{
	if(!error_buffer)
		return;
	std::string error_string(CAST_PTR(char*, error_buffer->GetBufferPointer()));
	std::string error_line;

	// iterate through the error string, printing each line
	int count = 0;
	while(StringEditing::GetStringSegment(error_string, error_line, NULL, "\n")
		&& count < error_count )
	{
		// if the line contains the data path, remove it
		std::string::size_type data_string_start = std::string::npos;
		if((data_string_start = error_line.find(Settings::Get().active_profile.GetDataOverridePath())) != std::string::npos)
			error_line.replace(0, strlen(Settings::Get().active_profile.GetDataOverridePath()), "");	

		YELO_ERROR(_error_message_priority_warning, 
			"\t%s",
			error_line.c_str());

		// remove the line to move
		StringEditing::RemoveStringSegment(error_string, NULL, "\n");
		count++;
	}
}

HRESULT		ReplaceShaderCode(
	const LPD3DXBUFFER& effect_buffer, 
	TagGroups::s_shader_postprocess_definition* shader_tag)
{
	// remove any existing binary data
	if(shader_tag->shader_code_binary.address)
		TagGroups::tag_data_delete(shader_tag->shader_code_binary);

	// allocate memory for the new binary data
	shader_tag->shader_code_binary.address = 
		YELO_MALLOC(effect_buffer->GetBufferSize(), false);

	HRESULT hr = S_OK;

	if(!shader_tag->shader_code_binary.address) 
	{				
		YELO_ERROR(_error_message_priority_warning, 
			"OS_tool: failed to allocate memory for binary data");
		hr = E_FAIL;
	}
	else
		shader_tag->shader_code_binary.size = effect_buffer->GetBufferSize();

	if(SUCCEEDED(hr))
	{
		// copy the shader to the binary data field
		errno_t error_num = memcpy_s(shader_tag->shader_code_binary.address, 
			effect_buffer->GetBufferSize(),
			effect_buffer->GetBufferPointer(), 
			effect_buffer->GetBufferSize());
		if(error_num != 0)
		{
			YELO_ERROR(_error_message_priority_warning, 
				"OS_tool: failed to copy shader data to tag binary data field");
			hr = E_FAIL;
		}		
	}
	shader_tag->flags.shader_is_binary_bit = true;

	return hr;
}

HRESULT		CompileEffect(
	const LPD3DXEFFECTCOMPILER& effect_compiler, 
	LPD3DXBUFFER& effect_buffer)
{
	printf_s("compiling postprocess effect...");

	// compile postprocess shader into local variable
	LPD3DXBUFFER error_buffer = NULL;	

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
		YELO_ERROR(_error_message_priority_warning, 
			"OS_tool: failed to compile effect");
		WriteD3DXErrors(error_buffer, 3);
		safe_release(effect_buffer);
	}
	safe_release(error_buffer);
	return hr;
}

HRESULT		CreateEffectCompiler(
	LPD3DXEFFECTCOMPILER& effect_compiler, 
	const TagGroups::s_shader_postprocess_definition* shader_tag,
	const char* fx_file)
{
	printf_s("creating effect compiler...");

	// create effect compiler in local variable
	LPD3DXBUFFER error_buffer = NULL;
	
	HRESULT hr = D3DXCreateEffectCompilerFromFile(
		fx_file,
		NULL,
		NULL,
		D3DXSHADER_OPTIMIZATION_LEVEL3,
		&effect_compiler,
		&error_buffer);	

	// copy value to effect_compiler argument if succeeded
	if(SUCCEEDED(hr))
		puts("done");
	else
	{
		puts("failed");
		// inform the user that an error occurred and print the first two D3DX errors
		YELO_ERROR(_error_message_priority_warning, 
			"OS_tool: failed to create effect compiler");
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
	// attempt to load the post process shader using each type until
	// we find a shoe that fits
	do
	{
		tag_index = tag_load<TagGroups::s_shader_postprocess_generic>(
			tag_path, Flags::_tag_load_verify_exist_first);
		if(!tag_index.IsNull()) break;
	}
	while(false);
	
	// if no tag is found, create a new one
	if(tag_index.IsNull())
	{
		printf_s("created tag \"%s.shader_postprocess_generic\"\n", tag_path);		
		tag_index = tag_new(TagGroups::s_shader_postprocess_generic::k_group_tag, tag_path);
	}
	else
		printf_s("updating tag \"%s\"\n", tag_path);

	HRESULT hr = (tag_index.IsNull() ? E_FAIL : S_OK);

	if(SUCCEEDED(hr))
	{
		shader_tag = tag_get<TagGroups::s_shader_postprocess_definition>(tag_index);
		hr = (!shader_tag ? E_FAIL : S_OK);
	}
	else
	{		
		YELO_ERROR(_error_message_priority_warning, 
			"OS_tool: failed to open/create postprocess tag");
	}
	return hr;
}

HRESULT		SaveShader(
	const datum_index& tag_index)
{
	printf_s("saving shader...");

	bool save_succeeded = tag_save(tag_index);
	if(save_succeeded)
		puts("done");
	else
	{
		puts("failed");
		YELO_ERROR(_error_message_priority_warning, 
			"OS_tool: failed to save postprocess tag");
	}
	return (save_succeeded ? S_OK : E_FAIL);
}