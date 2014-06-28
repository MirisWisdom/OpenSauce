/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/Shader/Compiler/Collection/c_collection_writer_factory.hpp"

#if PLATFORM_TYPE == PLATFORM_TOOL
#include "Tool/Shader/Compiler/Collection/c_collection_writer_pixel.hpp"
#include "Tool/Shader/Compiler/Collection/c_collection_writer_vertex.hpp"

namespace Yelo
{
	namespace Tool { namespace Shader { namespace Compiler
	{
		const std::unique_ptr<i_collection_writer> c_collection_writer_factory::GetCollectionWriter(const std::string& type)
		{
			if (type == "PixelShader")
			{
				return std::make_unique<c_collection_writer_pixel>();
			}
			else if (type == "VertexShader")
			{
				return std::make_unique<c_collection_writer_vertex>();
			}
			else
			{
				YELO_ASSERT_DISPLAY(false, "ERROR: Unknown shader type");
			}

			return nullptr;
		}
	};};};
};
#endif