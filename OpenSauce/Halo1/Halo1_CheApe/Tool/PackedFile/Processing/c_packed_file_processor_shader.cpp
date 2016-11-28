/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/PackedFile/Processing/c_packed_file_processor_shader.hpp"

#if PLATFORM_TYPE == PLATFORM_TOOL
#include <YeloLib/files/files.hpp>

namespace Yelo
{
	namespace Tool { namespace PackedFile
	{
		bool c_packed_file_processor_shader::ProcessElement(c_packed_file::s_element_editor& element, const boost::filesystem::path& file_path)
		{
			c_auto_release<ID3DXEffectCompiler> compiler;
			c_auto_release<ID3DXBuffer> error_buffer;
			c_auto_release<ID3DXBuffer> compiled_effect;

			HRESULT hr;

			// Attempt to create an effect compiler from the shader file
			hr = D3DXCreateEffectCompilerFromFile(file_path.string().c_str(), 
				nullptr,
				nullptr, 
				D3DXSHADER_OPTIMIZATION_LEVEL3, 
				&compiler,
				&error_buffer);

			// If compilation failed, print the debug text to the console
			if(FAILED(hr)) 
			{ 
				if(error_buffer)
				{
					puts((char*)error_buffer->GetBufferPointer());
				}
				return false;
			}

			// Attempt to compile the effect
			hr = compiler->CompileEffect(D3DXSHADER_OPTIMIZATION_LEVEL3,
				&compiled_effect,
				&error_buffer);

			// If compilation failed, print the debug text to the console
			if(FAILED(hr)) 
			{ 
				puts((char*)error_buffer->GetBufferPointer());
				return false;
			}

			// Copy the compiled data to the element
			element.element_size = compiled_effect->GetBufferSize();
			element.source_data = new char[element.element_size];
			errno_t error = memcpy_s(element.source_data, 
				element.element_size, 
				compiled_effect->GetBufferPointer(), 
				element.element_size);

			if(error != 0)
			{
				return false;
			}

			return true;
		}
	};};
};
#endif