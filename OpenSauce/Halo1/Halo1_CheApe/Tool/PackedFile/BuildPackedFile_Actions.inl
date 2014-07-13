/*!
 * \brief
 * Packs the file as-is.
 * 
 * \param element
 * The packed file element being created.
 * 
 * \param element_source
 * The source path for the file to pack.
 * 
 * \returns
 * True if the file was processed successfully.
 * 
 * The default action simply copies the file to the packed file element without any extra processing.
 */
bool CustomAction_Default(c_packed_file::s_element_editor& element, const filesystem::path& file_path)
{
	FileIO::s_file_info file;
	if(Enums::_file_io_open_error_none != FileIO::OpenFile(file, 
		file_path.string().c_str(), 
		Enums::_file_io_open_access_type_write, 
		Enums::_file_io_open_create_option_new))
	{
		return false; // failed to open file
	}

	element.element_size = file.file_size;
	if(Enums::_file_io_read_error_none != FileIO::ReadFileToMemory(file, CAST_PTR(void**, &element.source_data), 0, 0))
	{
		FileIO::CloseFile(file);
		element.element_size = 0;
		return false;
	}
	FileIO::CloseFile(file);
	return true;
}
/*!
 * \brief
 * Packs the file after compiling it as a HLSL effect.
 * 
 * \param element
 * The packed file element being created.
 * 
 * \param element_source
 * The source path for the file to pack.
 * 
 * \returns
 * True if the file was processed successfully.
 * 
 * Packs the file after compiling it as a HLSL effect.
 */
bool CustomAction_Shader(c_packed_file::s_element_editor& element, const filesystem::path& file_path)
{
	LPD3DXEFFECTCOMPILER compiler = NULL;
	LPD3DXBUFFER error_buffer = NULL;
	LPD3DXBUFFER compiled_effect = NULL;
	bool success = true;

	do{
		HRESULT hr;

		// attempt to create an effect compiler from the shader file
		hr = D3DXCreateEffectCompilerFromFile(file_path.string().c_str(), 
			NULL, NULL, 
			D3DXSHADER_OPTIMIZATION_LEVEL3, 
			&compiler,
			&error_buffer);

		// if compilation failed, print the debug text to the console
		if(FAILED(hr)) 
		{ 
			if(error_buffer)
				puts((char*)error_buffer->GetBufferPointer());
			success = false; break; 
		} //broken effect
		safe_release(error_buffer);

		// attempt to compile the effect
		hr = compiler->CompileEffect(D3DXSHADER_OPTIMIZATION_LEVEL3,
			&compiled_effect,
			&error_buffer);

		// if compilation failed, print the debug text to the console
		if(FAILED(hr)) 
		{ 
			puts((char*)error_buffer->GetBufferPointer());
			success = false; break; 
		} //broken effect
		safe_release(error_buffer);
		
		// 
		element.element_size = compiled_effect->GetBufferSize();
		element.source_data = new char[element.element_size];
		errno_t error = memcpy_s(element.source_data, 
			element.element_size, 
			compiled_effect->GetBufferPointer(), 
			element.element_size);

		if(error != 0)
			success = false;

	}while(false);

	safe_release(compiler);
	safe_release(error_buffer);
	safe_release(compiled_effect);

	return success;
}