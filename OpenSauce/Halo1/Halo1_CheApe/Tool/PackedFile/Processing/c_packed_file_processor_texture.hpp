/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#if PLATFORM_TYPE == PLATFORM_TOOL
#include <YeloLib/Halo1/files/packed_file.hpp>

#include "Tool/PackedFile/Processing/i_packed_file_processor.hpp"

namespace Yelo
{
	namespace Tool { namespace PackedFile
	{
		class c_packed_file_processor_texture final
			: public i_packed_file_processor
		{
		public:
			bool ProcessElement(c_packed_file::s_element_editor& element, const boost::filesystem::path& file_path) override;
		};
	};};
};
#endif