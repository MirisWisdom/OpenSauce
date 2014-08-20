/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/Shader/Compiler/Effect/c_effect_writer.hpp"

#if PLATFORM_TYPE == PLATFORM_TOOL
#include <YeloLib/files/files.hpp>
#include "Tool/Shader/c_shader_processor.hpp"

using namespace boost::filesystem;

namespace Yelo
{
	namespace Tool { namespace Shader { namespace Compiler
	{
#pragma region s_shader
		c_effect_writer::c_shader_list::s_shader::s_shader(std::string name, LPD3DXBUFFER pixel_shader_code, LPD3DXBUFFER vertex_shader_code)
			: m_name(name)
			, m_pixel_shader_code(pixel_shader_code)
			, m_vertex_shader_code(vertex_shader_code)
		{ }
#pragma endregion

#pragma region c_shader_list
		c_effect_writer::c_shader_list::c_shader_list()
			: m_shaders()
		{ }

		c_effect_writer::c_shader_list::~c_shader_list()
		{
			for (auto& technique : m_shaders)
			{
				safe_release(technique.m_pixel_shader_code);
				safe_release(technique.m_vertex_shader_code);
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Builds the shader list. </summary>
		///
		/// <param name="effect">	The effect to process. </param>
		void c_effect_writer::c_shader_list::BuildShaderList(const LPD3DXEFFECT& effect)
		{
			D3DXEFFECT_DESC effect_desc;
			YELO_ASSERT_DISPLAY(SUCCEEDED(effect->GetDesc(&effect_desc)), "ERROR: failed to get an effects description");

			for (uint32 i = 0; i < effect_desc.Techniques; i++)
			{
				D3DXHANDLE technique_handle = effect->GetTechnique(i);
				YELO_ASSERT_DISPLAY(technique_handle, "ERROR: Invalid technique handle");

				D3DXHANDLE pass_handle = effect->GetPass(technique_handle, 0);
				YELO_ASSERT_DISPLAY(technique_handle, "ERROR: Invalid pass handle");

				D3DXTECHNIQUE_DESC technique_desc;
				YELO_ASSERT_DISPLAY(SUCCEEDED(effect->GetTechniqueDesc(technique_handle, &technique_desc)), "ERROR: Failed to get an techniques description");

				D3DXPASS_DESC pass_desc;
				YELO_ASSERT_DISPLAY(SUCCEEDED(effect->GetPassDesc(pass_handle, &pass_desc)), "ERROR: Failed to get a pass' description");

				LPD3DXBUFFER pixel_shader_buffer(nullptr);
				LPD3DXBUFFER vertex_shader_buffer(nullptr);
				if (pass_desc.pPixelShaderFunction)
				{
					c_shader_processor::RemoveComments(pass_desc.pPixelShaderFunction, pixel_shader_buffer);
				}
				if (pass_desc.pVertexShaderFunction)
				{
					c_shader_processor::RemoveComments(pass_desc.pVertexShaderFunction, vertex_shader_buffer);
				}

				m_shaders.emplace(m_shaders.end(), technique_desc.Name, pixel_shader_buffer, vertex_shader_buffer);
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets an iterator at the start of the list. </summary>
		///
		/// <returns>	An iterator. </returns>
		std::vector<c_effect_writer::c_shader_list::s_shader>::const_iterator c_effect_writer::c_shader_list::begin()
		{
			return m_shaders.cbegin();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets an iterator at the end of the list. </summary>
		///
		/// <returns>	An iterator. </returns>
		std::vector<c_effect_writer::c_shader_list::s_shader>::const_iterator c_effect_writer::c_shader_list::end()
		{
			return m_shaders.cend();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the number of shaders in the list. </summary>
		///
		/// <returns>	An int. </returns>
		int c_effect_writer::c_shader_list::size()
		{
			return m_shaders.size();
		}
#pragma endregion

#pragma region c_effect_writer
		void c_effect_writer::WriteEffect(const path& output_file, const std::string& effect_name, const std::string& shader_target, const LPD3DXEFFECT& effect)
		{
			// Create the effect file
			FileIO::s_file_info effect_file;
			auto open_result = FileIO::OpenFile(effect_file
				, output_file.string().c_str()
				, Enums::_file_io_open_access_type_write
				, Enums::_file_io_open_create_option_new);
			YELO_ASSERT_DISPLAY(open_result == Enums::_file_io_open_error_none, "ERROR: Failed to open an effect file for writing. [%s]", output_file.string().c_str());

			WriteHeader(effect_file, effect_name);
			WriteShaders(effect_file, shader_target, effect);

			FileIO::CloseFile(effect_file);
		}
#pragma endregion
	};};};
};
#endif