/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/Shader/c_shader_processor.hpp"

#if PLATFORM_TYPE == PLATFORM_TOOL

namespace Yelo
{
	namespace Tool { namespace Shader
	{
		void c_shader_processor::Disassemble(const DWORD* byte_code, LPD3DXBUFFER& output_buffer)
		{
			YELO_ASSERT_DISPLAY(SUCCEEDED(D3DXDisassembleShader(byte_code, false, NULL, &output_buffer)), "ERROR: Failed to disassemble shader");
		}

		void c_shader_processor::Assemble(std::string& assembly, LPD3DXBUFFER& output_buffer)
		{
			LPD3DXBUFFER errors;
			HRESULT hr = D3DXAssembleShader(assembly.c_str(), assembly.length(), NULL, NULL, 0, &output_buffer, &errors);
			if (FAILED(hr))
			{
				puts((char*)errors->GetBufferPointer());
				puts(assembly.c_str());
				YELO_ASSERT_DISPLAY(true, "ERROR: Failed to assembly a shader");
			}
			safe_release(errors);
		}

		void c_shader_processor::RemoveComments(const DWORD* byte_code, LPD3DXBUFFER& output_buffer)
		{
			LPD3DXBUFFER assembly;
			Disassemble(byte_code, assembly);
			Assemble(std::string((char*)assembly->GetBufferPointer()), output_buffer);
			safe_release(assembly);
		}
	};};
};
#endif