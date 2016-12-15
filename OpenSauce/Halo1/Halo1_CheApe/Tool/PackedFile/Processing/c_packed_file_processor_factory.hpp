/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#if PLATFORM_TYPE == PLATFORM_TOOL
#include "Tool/PackedFile/Processing/i_packed_file_processor.hpp"

namespace Yelo
{
	namespace Tool { namespace PackedFile
	{
		class c_packed_file_processor_factory final
		{
		public:
			static std::unique_ptr<i_packed_file_processor> GetPackedFileProcessor(const std::string& type);
		};
	};};
};
#endif