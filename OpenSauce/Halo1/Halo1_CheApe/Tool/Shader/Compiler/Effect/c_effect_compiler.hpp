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
		class c_effect_compiler
		{
		public:
			static bool CompileEffect(const boost::filesystem::path& file_location, const std::string& target, LPD3DXEFFECT& effect_out);
		};
	};};};
};
#endif