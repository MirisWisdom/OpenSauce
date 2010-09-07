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
	char* string_pointer = CAST_PTR(char*, error_buffer->GetBufferPointer());

	bool remove_path = false;
	size_t work_dir_path_len = strlen(Settings::WorkingDirectoryPath());

	size_t string_length = strlen(string_pointer);
	int32 count = 0;
	while(strlen(string_pointer) > 0 && count < error_count)
	{
		if(strlen(string_pointer) < work_dir_path_len)
			remove_path = false;
		else
		{			
			// determine whether the line starts with the working directory
			// if it does, chop it out
			char* line_start = new char[work_dir_path_len + 1];
			memset(line_start, 0, work_dir_path_len);
			memcpy_s(line_start, work_dir_path_len, string_pointer, work_dir_path_len);
			line_start[work_dir_path_len] = '\0'; 
			if(strcmp(line_start, Settings::WorkingDirectoryPath()) == 0)
				remove_path = true;
			delete [] line_start;
		}

		//Get the line
		string_pointer += (remove_path ? work_dir_path_len + 7 : 0);
		size_t line_length = strcspn(string_pointer, "\n") + 1;
		char* buffer = new char[line_length];
		memcpy_s(buffer, line_length, string_pointer, line_length);
		buffer[line_length - 1] = '\0';
	
		
		YELO_ERROR(_error_message_priority_warning, 
			"\t%s",
			buffer);

		delete[] buffer;
		buffer = NULL;
		//probably not safe, but should be ok
		string_pointer += line_length;

		count++;
	}
}

HRESULT		ReplaceShaderCode(
	const LPD3DXBUFFER& effect_buffer, 
	TagGroups::s_shader_postprocess_definition* shader_tag,
	const bool remove_text_data)
{
	// remove any existing binary data
	if(shader_tag->shader_code_binary.address)
		tag_data_delete(shader_tag->shader_code_binary);

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
		else				
			shader_tag->flags.shader_is_binary_bit = true;
	}
	
	// remove the text data if necessary
	// inform user of destructive action and confirm intention
	if(SUCCEEDED(hr) && remove_text_data)
	{
		bool remove_text = false;
		fputs(
			"\n!WARNING!\n" 
			"the text shader data is about to be removed\n"
			"that data will be lost\n"					
			"are you sure you want it removed? (YES/NO)? ",
			stdout);
		fflush(stdout);
		
		char input[4];
		if(fgets(input, sizeof(input), stdin) != NULL)
		{
			// we only need to compare the first three letters
			input[3] = 0;
			if(strcmp("YES", input) == 0)
				remove_text = true;
		}
		fputs("\n", stdout);

		if(remove_text)
		{
			tag_data_delete(shader_tag->shader_code_text, 1);
			printf_s("text shader data removed\n");
		}
	}

	return hr;
}

HRESULT		CompileEffect(
	const LPD3DXEFFECTCOMPILER& effect_compiler, 
	LPD3DXBUFFER& effect_buffer,
	const DWORD flags)
{
	printf_s("compiling postprocess effect...");

	// compile postprocess shader into local variable
	LPD3DXBUFFER error_buffer = NULL;	
	LPD3DXBUFFER _effect_buffer = NULL;	

	HRESULT hr = effect_compiler->CompileEffect(
		flags,
		&_effect_buffer,
		&error_buffer);	

	// copy value to effect_buffer argument if succeeded
	if(SUCCEEDED(hr))
	{
		printf_s("done\n");
		effect_buffer = _effect_buffer;
	}
	else
	{
		printf_s("failed\n");
		// inform the user that an error occurred and print the first two D3DX errors
		YELO_ERROR(_error_message_priority_warning, 
			"OS_tool: failed to compile effect");
		WriteD3DXErrors(error_buffer, 2);
		safe_release(_effect_buffer);
	}
	safe_release(error_buffer);
	return S_OK;
}

HRESULT		CreateEffectCompiler(
	LPD3DXEFFECTCOMPILER& effect_compiler, 
	const TagGroups::s_shader_postprocess_definition* shader_tag,
	const DWORD flags)
{
	printf_s("creating effect compiler...");

	// check shader code is present
	if(shader_tag->shader_code_text.size <= 1)
	{
		printf_s("failed\n");
		YELO_ERROR(_error_message_priority_warning, 
			"OS_tool: tag contains no effect text data");
		return E_FAIL;
	}

	// create effect compiler in local variable
	LPD3DXBUFFER error_buffer = NULL;	
	LPD3DXEFFECTCOMPILER _effect_compiler = NULL;

	HRESULT hr = D3DXCreateEffectCompiler(
		CAST_PTR(char*, shader_tag->shader_code_text.address),
		shader_tag->shader_code_text.size,
		NULL,
		NULL,
		flags,
		&_effect_compiler,
		&error_buffer);	

	// copy value to effect_compiler argument if succeeded
	if(SUCCEEDED(hr))
	{
		printf_s("done\n");
		effect_compiler = _effect_compiler;
	}
	else
	{
		printf_s("failed\n");
		// inform the user that an error occurred and print the first two D3DX errors
		YELO_ERROR(_error_message_priority_warning, 
			"OS_tool: failed to create effect compiler");
		WriteD3DXErrors(error_buffer, 2);
		safe_release(_effect_compiler);
	}
	safe_release(error_buffer);
	return hr;
}

HRESULT		LoadShader(
	const char* tag_location,
	datum_index& tag_index, 
	TagGroups::s_shader_postprocess_definition*& shader_tag)
{
	printf_s("loading postprocess shader...");

	// attempt to load the post process shader using each type until
	// we find a shoe that fits
	do
	{
		tag_index = tag_load<TagGroups::s_shader_postprocess_definition>(
			tag_location, Flags::_tag_load_verify_exist_first);
		if(!tag_index.IsNull()) break;

		tag_index = tag_load<TagGroups::s_shader_postprocess_generic>(
			tag_location, Flags::_tag_load_verify_exist_first);
		if(!tag_index.IsNull()) break;
	}
	while(false);
	
	HRESULT hr = (tag_index.IsNull() ? E_FAIL : S_OK);

	if(SUCCEEDED(hr))
	{
		shader_tag = tag_get<TagGroups::s_shader_postprocess_definition>(tag_index);
		hr = (!shader_tag ? E_FAIL : S_OK);
	}

	if(SUCCEEDED(hr))
		printf_s("done\n");
	else
	{
		printf_s("failed\n");
		YELO_ERROR(_error_message_priority_warning, 
			"OS_tool: failed to load postprocess tag: %s",
			tag_location);
	}
	return hr;
}

HRESULT		SaveShader(
	const datum_index& tag_index)
{
	printf_s("saving shader...");

	bool save_succeeded = tag_save(tag_index);
	if(save_succeeded)
		printf_s("done\n");
	else
	{
		printf_s("failed\n");
		YELO_ERROR(_error_message_priority_warning, 
			"OS_tool: failed to save postprocess tag");
	}
	return (save_succeeded ? S_OK : E_FAIL);
}