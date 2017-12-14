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
		class i_collection_definition
		{
		public:
			virtual const std::string GetName() = 0;
			virtual const std::string GetSourceDirectory() = 0;
			virtual const std::string GetShaderType() = 0;
			virtual const std::string GetCompilationTarget() = 0;
			virtual const std::vector<std::string> GetEffectList() = 0;
		};
	};};};
};
#endif