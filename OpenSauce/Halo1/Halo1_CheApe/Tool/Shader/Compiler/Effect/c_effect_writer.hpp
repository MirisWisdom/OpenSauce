/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#if PLATFORM_TYPE == PLATFORM_TOOL
#include <YeloLib/files/files.hpp>
#include "Tool/Shader/Compiler/Effect/i_effect_writer.hpp"

namespace Yelo
{
	namespace Tool { namespace Shader { namespace Compiler
	{
		class c_effect_writer
			: public i_effect_writer
		{
		protected:
			class c_shader_list final
			{
				struct s_shader
				{
					std::string m_name;
					LPD3DXBUFFER m_pixel_shader_code;
					LPD3DXBUFFER m_vertex_shader_code;

					s_shader(std::string name, LPD3DXBUFFER pixel_shader_code, LPD3DXBUFFER vertex_shader_code);
				};

				std::vector<s_shader> m_shaders;

			public:
				c_shader_list();

				~c_shader_list();

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Builds the shader list. </summary>
				///
				/// <param name="effect">	The effect to process. </param>
				void BuildShaderList(const LPD3DXEFFECT& effect);

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Gets an iterator at the start of the list. </summary>
				///
				/// <returns>	An iterator. </returns>
				std::vector<s_shader>::const_iterator begin();

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Gets an iterator at the end of the list. </summary>
				///
				/// <returns>	An iterator. </returns>
				std::vector<s_shader>::const_iterator end();

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Gets the number of shaders in the list. </summary>
				///
				/// <returns>	An int. </returns>
				int size();
			};

		private:
			virtual void WriteShaders(FileIO::s_file_info& output_file, const std::string& shader_target, const LPD3DXEFFECT& effect) = 0;
			virtual void WriteHeader(FileIO::s_file_info& output_file, const std::string& effect_name) = 0;

		public:
			void WriteEffect(const boost::filesystem::path& output_file, const std::string& effect_name, const std::string& shader_target, const LPD3DXEFFECT& effect) final override;
		};};};
	};
};
#endif