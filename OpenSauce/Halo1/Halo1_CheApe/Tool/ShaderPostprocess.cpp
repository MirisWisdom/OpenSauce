/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/ShaderPostprocess.hpp"
#if PLATFORM_TYPE == PLATFORM_TOOL

#include <YeloLib/Halo1/shaders/shader_postprocess_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/settings/c_settings_cheape.hpp>

#include "Settings/Settings.hpp"
#include "Common/StringEditing.hpp"
#include "Engine/EngineFunctions.hpp"
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace Flags
	{
		enum shader_model_mask
		{
			_shader_model_mask_1_0 = 1 << 0,
			_shader_model_mask_2_0 = 1 << 1,
			_shader_model_mask_3_0 = 1 << 2,
		};
	};
	namespace Tool
	{
#include "Tool/CompileShaderPostprocess.inl"

		void PLATFORM_API compile_shader_postprocess(char* arguments[])
		{
			struct s_arguments {
				cstring data_path;
			}* args = CAST_PTR(s_arguments*, arguments);

			
			std::string search_path(Settings::c_settings_cheape::Profile().GetDataPath());

			// append a folder divider if necessary
			search_path.append(args->data_path);
			StringEditing::AppendCharIfAbsent(search_path, '\\');

			// add *.fx to finish the search filter
			std::string search_filter(search_path);
			search_filter.append("*.fx");

			puts("");

			// search for the first file that matches the filter
			WIN32_FIND_DATA data;
			HANDLE search_handle;
			BOOL find_success = TRUE;

			// iterate through all matching files
			for(search_handle = FindFirstFile(search_filter.c_str(), &data);
				(search_handle != INVALID_HANDLE_VALUE) && find_success;
				find_success = FindNextFile(search_handle, &data))
			{
				std::string file_name(data.cFileName);

				// build the shader files path
				std::string fx_path(search_path);
				fx_path.append(file_name);

				std::string tag_path(args->data_path);
				StringEditing::AppendCharIfAbsent(tag_path, '\\');
				tag_path.append(file_name.c_str(), file_name.size() - 3);

				// open the shader tag
				datum_index shader_index = datum_index::null;
				TagGroups::s_shader_postprocess_definition* shader_tag = nullptr;

				HRESULT hr = LoadShader(tag_path.c_str(), shader_index, shader_tag);

				// create effect compiler
				LPD3DXEFFECTCOMPILER effect_compiler = nullptr;
				if(SUCCEEDED(hr)) hr = CreateEffectCompiler(
					effect_compiler,
					shader_tag,
					fx_path.c_str()
					);

				// put new shader code
				if(SUCCEEDED(hr)) hr = SetPostprocessTag(effect_compiler, shader_tag);
				// save shader tag
				if(SUCCEEDED(hr)) hr = SaveShader(shader_index);

				puts("");

				if(!shader_index.IsNull())
					blam::tag_unload(shader_index);

				safe_release(effect_compiler);
			}
			// close the search handle
			FindClose(search_handle);

			DWORD error = GetLastError();
			if(error)
			{
				switch(error)
				{
				case ERROR_FILE_NOT_FOUND:
					YELO_WARN("OS_Tool: no fx files found in directory \"%s\"\n",
						args->data_path);
					break;
				case ERROR_PATH_NOT_FOUND:
					YELO_WARN("OS_Tool: directory does not exist\"%s\"\n",
						args->data_path);
					break;
				case ERROR_NO_MORE_FILES:
				default:
					break;
				}
			}
		}
	};
};
#endif