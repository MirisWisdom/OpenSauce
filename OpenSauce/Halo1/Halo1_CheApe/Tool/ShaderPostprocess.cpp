/*
    Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Common/Precompile.hpp"
#include "Tool/ShaderPostprocess.hpp"
#if PLATFORM_ID == PLATFORM_TOOL

#include <Blam/Halo1/shader_postprocess_definitions.hpp>
#include <Common/Halo1/YeloSharedSettings.hpp>
#include "Common/YeloSettings.hpp"
#include "Common/StringEditing.hpp"
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace Tool
	{
#include "Tool/CompileShaderPostprocess.inl"

		void PLATFORM_API compile_shader_postprocess(void** arguments)
		{
			struct s_arguments {
				cstring data_file_name;
			}* args = CAST_PTR(s_arguments*, arguments);

			std::string search_path(Settings::Get().GetDataPath());

			search_path.append(args->data_file_name);
			std::string::reverse_iterator r_iter = search_path.rbegin();
			if((*r_iter) != '\\')
				search_path.append("\\");
			search_path.append("*.fx");

			puts("");

			WIN32_FIND_DATA data;
			HANDLE search_handle = FindFirstFile(search_path.c_str(), &data);
			do
			{
				if(search_handle == INVALID_HANDLE_VALUE)
					break;

				std::string file_name(data.cFileName);
				if(file_name.size() < 3)
					continue;
				if(file_name.compare(file_name.size() - 3, 3, ".fx") != 0)
					continue;

				std::string fx_path(Settings::Get().GetDataPath());
				fx_path.append(args->data_file_name);
				fx_path.append("\\");
				fx_path.append(file_name);

				std::string tag_path(args->data_file_name);	
				tag_path.append("\\");
				tag_path.append(file_name.c_str(), file_name.size() - 3);

				// open the shader tag
				datum_index shader_index = datum_index::null;
				TagGroups::s_shader_postprocess_definition* shader_tag = NULL;

				HRESULT hr = LoadShader(tag_path.c_str(), shader_index, shader_tag);

				// create effect compiler
				LPD3DXEFFECTCOMPILER effect_compiler = NULL;
				if(SUCCEEDED(hr)) hr = CreateEffectCompiler(
					effect_compiler, 
					shader_tag,
					fx_path.c_str()
					);

				// compile the effect
				LPD3DXBUFFER effect_buffer = NULL;
				if(SUCCEEDED(hr)) hr = CompileEffect(
					effect_compiler, 
					effect_buffer);

				// put new shader code
				if(SUCCEEDED(hr)) hr = ReplaceShaderCode(effect_buffer, shader_tag);
				// save shader tag
				if(SUCCEEDED(hr)) hr = SaveShader(shader_index);

				puts("");

				if(!shader_index.IsNull())
					tag_unload(shader_index);

				safe_release(effect_compiler);
				safe_release(effect_buffer);
			}
			while(FindNextFile(search_handle, &data));
			FindClose(search_handle);

			DWORD error = GetLastError();
			if(error)
			{
				switch(error)
				{
				case ERROR_FILE_NOT_FOUND:
					YELO_ERROR(_error_message_priority_warning, 
						"OS_Tool: no fx files found in directory \"%s\"\n",
						args->data_file_name);
					break;
				case ERROR_PATH_NOT_FOUND:
					YELO_ERROR(_error_message_priority_warning, 
						"OS_Tool: directory does not exist\"%s\"\n",
						args->data_file_name);
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