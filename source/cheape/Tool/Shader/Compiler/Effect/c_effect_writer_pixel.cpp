/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/Shader/Compiler/Effect/c_effect_writer_pixel.hpp"

#if PLATFORM_TYPE == PLATFORM_TOOL
#include "Tool/Console.hpp"
#include <YeloLib/files/files.hpp>

using namespace Yelo::FileIO;

namespace Yelo
{
	namespace Tool { namespace Shader { namespace Compiler
	{
		void c_effect_writer_pixel::WriteShaders(s_file_info& output_file
			, const std::string& shader_target
			, const LPD3DXEFFECT& effect)
		{
			std::string written_target(shader_target);
			if (written_target == "ps_2_a")
			{
				written_target = "ps_2_2";
			}
			else if (written_target == "ps_2_b")
			{
				written_target = "ps_2_1";
			}

			// Build the shader list
			c_shader_list shader_list;
			shader_list.BuildShaderList(effect);
				
			DWORD shader_count = shader_list.size();
			auto write_result = WriteToFile(output_file, CAST_PTR(char*, &shader_count), sizeof(shader_count));
			YELO_ASSERT_DISPLAY(write_result == Enums::_file_io_write_error_none, "ERROR: Failed to write the shader count to a pixel shader effect file");
						
			Console::ColorPrint(0x000B, "Shaders:\r\n");
			for (auto& shader : shader_list)
			{
				Console::ColorPrintF(0x0005, "\t%s\r\n", shader.m_name.c_str());

				// Write the shader name					
				std::string shader_name("PS_" + shader.m_name + "_" + written_target);
					
				DWORD shader_name_length = shader_name.length();
				write_result = WriteToFile(output_file, CAST_PTR(char*, &shader_name_length), sizeof(shader_name_length));
				YELO_ASSERT_DISPLAY(write_result == Enums::_file_io_write_error_none, "ERROR: Failed to write the shader name length to a pixel shader effect file");

				write_result = WriteToFile(output_file, shader_name.c_str(), shader_name_length);
				YELO_ASSERT_DISPLAY(write_result == Enums::_file_io_write_error_none, "ERROR: Failed to write the shader name to a pixel shader effect file");

				// Write shader byte code
				DWORD shader_length = shader.m_pixel_shader_code->GetBufferSize() / 4;
				write_result = WriteToFile(output_file, CAST_PTR(char*, &shader_length), sizeof(shader_length));
				YELO_ASSERT_DISPLAY(write_result == Enums::_file_io_write_error_none, "ERROR: Failed to write the shader dword length to a pixel shader effect file");

				write_result = WriteToFile(output_file, CAST_PTR(char*, shader.m_pixel_shader_code->GetBufferPointer()), shader.m_pixel_shader_code->GetBufferSize());
				YELO_ASSERT_DISPLAY(write_result == Enums::_file_io_write_error_none, "ERROR: Failed to write the shader byte code to a pixel shader effect file");
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Writes the effect files header. </summary>
		///
		/// <param name="output_file">	[in] The output file. </param>
		/// <param name="effect_name">	Name of the effect. </param>
		void c_effect_writer_pixel::WriteHeader(s_file_info& output_file, const std::string& effect_name)
		{
			// Write the effect name
			DWORD name_length = effect_name.size();
			auto write_result = WriteToFile(output_file, CAST_PTR(char*, &name_length), sizeof(name_length));
			YELO_ASSERT_DISPLAY(write_result == Enums::_file_io_write_error_none, "ERROR: Failed to write to a pixel shader file");

			write_result = WriteToFile(output_file, effect_name.c_str(), name_length);
			YELO_ASSERT_DISPLAY(write_result == Enums::_file_io_write_error_none, "ERROR: Failed to write to a pixel shader file");
		}
	};};};
};
#endif