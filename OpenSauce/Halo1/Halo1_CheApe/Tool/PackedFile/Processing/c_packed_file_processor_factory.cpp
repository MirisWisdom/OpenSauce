/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/PackedFile/Processing/c_packed_file_processor_factory.hpp"

#if PLATFORM_TYPE == PLATFORM_TOOL
#include "Tool/PackedFile/Processing/c_packed_file_processor_archive.hpp"
#include "Tool/PackedFile/Processing/c_packed_file_processor_shader.hpp"
#include "Tool/PackedFile/Processing/c_packed_file_processor_texture.hpp"

namespace Yelo
{
	namespace Tool { namespace PackedFile
	{
		std::unique_ptr<i_packed_file_processor> c_packed_file_processor_factory::GetPackedFileProcessor(const std::string& type)
		{
			if (type == "Archive")
			{
				return std::make_unique<c_packed_file_processor_archive>();
			}
			else if (type == "Shader")
			{
				return std::make_unique<c_packed_file_processor_shader>();
			}
			else if (type == "Texture")
			{
				return std::make_unique<c_packed_file_processor_texture>();
			}
			else
			{
				YELO_ASSERT_DISPLAY(false, "ERROR: Unknown shader type: %s", type.c_str());
			}

			return nullptr;
		}
	};};
};
#endif