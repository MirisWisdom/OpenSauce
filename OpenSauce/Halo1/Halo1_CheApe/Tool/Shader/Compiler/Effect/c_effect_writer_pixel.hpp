/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#if PLATFORM_TYPE == PLATFORM_TOOL
#include <YeloLib/files/files.hpp>
#include "Tool/Shader/Compiler/Effect/c_effect_writer.hpp"

namespace Yelo
{
	namespace Tool { namespace Shader { namespace Compiler
	{
		class c_effect_writer_pixel final
			: public c_effect_writer
		{
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Writes the pixel shaders to the output file. </summary>
			/// 
			/// <param name="output_file">   	[in] The output file. </param>
			/// <param name="target">		 	[in] The pixel shader target. </param>
			/// <param name="effect">		 	The shader effect to write. </param>
			void WriteShaders(FileIO::s_file_info& output_file
				, const std::string& shader_target
				, const LPD3DXEFFECT& effect) override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Writes the effect files header. </summary>
			///
			/// <param name="output_file">	[in] The output file. </param>
			/// <param name="effect_name">	Name of the effect. </param>
			void WriteHeader(FileIO::s_file_info& output_file, const std::string effect_name) override;
		};
	};};};
};
#endif