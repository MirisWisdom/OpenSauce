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

#include <Blam/Halo1/shader_postprocess_definitions.hpp>
#include <Common/Halo1/YeloSharedSettings.hpp>
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
				cstring remove_text_data_cstr;
				cstring compile_debug_cstr;
			}* args = CAST_PTR(s_arguments*, arguments);

			bool remove_text_data = Settings::ParseBoolean(args->remove_text_data_cstr);
			bool compile_debug = Settings::ParseBoolean(args->compile_debug_cstr);

			if(remove_text_data && compile_debug)
			{
				YELO_ERROR(_error_message_priority_warning, 
					"OS_tool: you can not remove text shader data if compiling as debug\n"
					"OS_tool: the shader must be finalised before removing the text data\n");
				return;
			}

			// open the shader tag
			datum_index shader_index = datum_index::null;
			TagGroups::s_shader_postprocess_definition* shader_tag = NULL;

			HRESULT hr = LoadShader(args->data_file_name, shader_index, shader_tag);

			// create effect compiler
			LPD3DXEFFECTCOMPILER effect_compiler = NULL;
			if(SUCCEEDED(hr)) hr = CreateEffectCompiler(
				effect_compiler, 
				shader_tag, 
				(compile_debug ? D3DXSHADER_DEBUG | D3DXSHADER_OPTIMIZATION_LEVEL0 : D3DXSHADER_OPTIMIZATION_LEVEL3)
				);

			// compile the effect
			LPD3DXBUFFER effect_buffer = NULL;
			if(SUCCEEDED(hr)) hr = CompileEffect(
				effect_compiler, 
				effect_buffer,
				(compile_debug ? D3DXSHADER_DEBUG | D3DXSHADER_OPTIMIZATION_LEVEL0 : D3DXSHADER_OPTIMIZATION_LEVEL3)
				);

			// delete old shader code
			// put new shader code
			// remove text shader code
			if(SUCCEEDED(hr)) hr = ReplaceShaderCode(effect_buffer, shader_tag, remove_text_data);
			// save shader tag
			if(SUCCEEDED(hr)) hr = SaveShader(shader_index);

			safe_release(effect_compiler);
			safe_release(effect_buffer);
		}
	};
};