/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#if PLATFORM_TYPE == PLATFORM_TOOL
namespace Yelo
{
	namespace Tool { namespace Shader { namespace Compiler
	{
		class i_effect_writer
			abstract
		{
		public:
			virtual void WriteEffect(const boost::filesystem::path& output_file, const std::string& effect_name, const std::string& shader_target, const LPD3DXEFFECT& effect) = 0;
		};
	};};};
};
#endif