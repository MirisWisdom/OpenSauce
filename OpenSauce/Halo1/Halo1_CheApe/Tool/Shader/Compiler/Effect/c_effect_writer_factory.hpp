/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#if PLATFORM_TYPE == PLATFORM_TOOL
#include "Tool/Shader/Compiler/Effect/i_effect_writer.hpp"

namespace Yelo
{
	namespace Tool { namespace Shader { namespace Compiler
	{
		class c_effect_writer_factory
		{
		public:
			static const std::unique_ptr<i_effect_writer> GetEffectWriter(const std::string& type);
		};
	};};};
};
#endif