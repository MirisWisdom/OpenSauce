/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/Shader/Compiler/Effect/c_effect_writer_factory.hpp"

#if PLATFORM_TYPE == PLATFORM_TOOL
#include "Tool/Shader/Compiler/Effect/c_effect_writer_pixel.hpp"
#include "Tool/Shader/Compiler/Effect/c_effect_writer_vertex.hpp"

namespace Yelo
{
	namespace Tool { namespace Shader { namespace Compiler
	{
		const std::unique_ptr<i_effect_writer> c_effect_writer_factory::GetEffectWriter(const std::string& type)
		{
			if (type == "PixelShader")
			{
				return std::make_unique<c_effect_writer_pixel>();
			}
			else if (type == "VertexShader")
			{
				return std::make_unique<c_effect_writer_vertex>();
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