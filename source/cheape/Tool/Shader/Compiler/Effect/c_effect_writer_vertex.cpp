/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/Shader/Compiler/Effect/c_effect_writer_vertex.hpp"

#if PLATFORM_TYPE == PLATFORM_TOOL
#include <YeloLib/files/files.hpp>

using namespace Yelo::FileIO;

namespace Yelo
{
	namespace Tool { namespace Shader { namespace Compiler
	{
		void c_effect_writer_vertex::WriteShaders(s_file_info& output_file, const std::string& shader_target, const LPD3DXEFFECT& effect)
		{
			// Build the shader list
			c_shader_list shader_list;
			shader_list.BuildShaderList(effect);
			YELO_ASSERT_DISPLAY(shader_list.size() == 1, "ERROR: A vertex shader effect has more than one shader in it");
					
			auto& shader = *shader_list.begin();

			// Write shader to the effect binary
			DWORD shader_length = shader.m_vertex_shader_code->GetBufferSize();
			auto write_result = WriteToFile(output_file, CAST_PTR(char*, &shader_length), sizeof(shader_length));
			YELO_ASSERT_DISPLAY(write_result == Enums::_file_io_write_error_none, "ERROR: Failed to write the shader byte length to a vertex shader effect file");

			write_result = WriteToFile(output_file, CAST_PTR(char*, shader.m_vertex_shader_code->GetBufferPointer()), shader.m_vertex_shader_code->GetBufferSize());
			YELO_ASSERT_DISPLAY(write_result == Enums::_file_io_write_error_none, "ERROR: Failed to write the shader byte code to a vertex shader effect file");
		}

		void c_effect_writer_vertex::WriteHeader(s_file_info& output_file, const std::string& effect_name)
		{ }
	};};};
};
#endif