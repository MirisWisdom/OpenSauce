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
		class c_effect_writer_vertex final
			: public c_effect_writer
		{
			void WriteShaders(FileIO::s_file_info& output_file
				, const std::string& shader_target
				, const LPD3DXEFFECT& effect) override;

			void WriteHeader(FileIO::s_file_info& output_file, const std::string& effect_name) override;
		};
	};};};
};
#endif