/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#if PLATFORM_TYPE == PLATFORM_TOOL
#include "Tool/Shader/Compiler/Collection/i_collection_definition.hpp"

namespace Yelo
{
	namespace Tool { namespace Shader { namespace Compiler
	{
		class i_collection_writer
		{
		public:
			virtual void WriteCollection(const boost::filesystem::path& source_directory
				, const boost::filesystem::path& collection_path
				, i_collection_definition& collection_definition) = 0;
		};
	};};};
};
#endif