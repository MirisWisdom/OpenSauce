/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#if PLATFORM_TYPE == PLATFORM_TOOL
#include "Tool/Shader/Compiler/Collection/i_collection_writer.hpp"

namespace Yelo
{
	namespace Tool { namespace Shader { namespace Compiler
	{
		class c_collection_writer_factory final
		{
		public:
			static const std::unique_ptr<i_collection_writer> GetCollectionWriter(const std::string& type);
		};
	};};};
};
#endif