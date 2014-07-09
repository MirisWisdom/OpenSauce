/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/Shader/Compiler/Collection/c_collection_writer_vertex.hpp"

#if PLATFORM_TYPE == PLATFORM_TOOL
#include <YeloLib/files/files.hpp>
#include "Tool/Shader/Compiler/Collection/i_collection_definition.hpp"

using namespace boost::filesystem;

namespace Yelo
{
	namespace Tool { namespace Shader { namespace Compiler
	{
		void c_collection_writer_vertex::WriteEffects(FileIO::s_file_info& collection_file, const path& effects_directory, i_collection_definition& collection_definition)
		{
			for (auto& effect : collection_definition.GetEffectList())
			{
				path effect_path(effects_directory);
				effect_path /= effect;
				effect_path.replace_extension(".bin");

				YELO_ASSERT_DISPLAY(exists(effect_path), "ERROR: A required vertex shader effect is missing [%s]", effect.c_str());
					
				AppendEffectToCollection(collection_file, effect_path);
			}
		}
	};};};
};
#endif