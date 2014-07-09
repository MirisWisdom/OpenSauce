/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/Shader/Compiler/Collection/c_collection_writer_pixel.hpp"

#if PLATFORM_TYPE == PLATFORM_TOOL
#include <YeloLib/files/files.hpp>

using namespace boost::filesystem;

namespace Yelo
{
	namespace Tool { namespace Shader { namespace Compiler
	{
		void c_collection_writer_pixel::WriteEffects(FileIO::s_file_info& collection_file, const path& effects_directory, i_collection_definition& collection_definition)
		{
			DWORD effect_count = std::count_if(directory_iterator(effects_directory), directory_iterator(),
				[](const path& path)
				{
					return path.extension() == ".bin";
				}
			);
			FileIO::WriteToFile(collection_file, &effect_count, sizeof(DWORD));

			directory_iterator end;
			for (directory_iterator iter(effects_directory); iter != end; ++iter)
			{
				path effect_path(iter->path());
				if (is_directory(effect_path) || !exists(effect_path))
				{
					continue;
				}

				if (effect_path.extension() != ".bin")
				{
					continue;
				}

				AppendEffectToCollection(collection_file, effect_path);
			}
		}
	};};};
};
#endif