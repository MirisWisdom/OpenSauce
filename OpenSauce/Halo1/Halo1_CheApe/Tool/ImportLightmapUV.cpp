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
#include "Tool/ImportLightmapUV.hpp"
#if PLATFORM_ID == PLATFORM_TOOL

#include <TagGroups/Halo1/structure_bsp_definitions.hpp>
#include "Engine/EngineFunctions.hpp"
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace Tool
	{
		class c_obj_file
		{
			struct s_vertex_indices
			{
				uint32 position_index, normals_index, texcoord_index;

				s_vertex_indices() : position_index(0), normals_index(0), texcoord_index(0)
				{
				}
			};
			struct s_face
			{
				s_vertex_indices vertex_indicies[3];

				s_face() : vertex_indicies()
				{
				}
			};
			struct s_group
			{
				static const size_t k_name_length = 255;

				std::vector<s_face> faces;
				char name[k_name_length + 1];

				s_group(cstring name) : faces()
				{
					size_t name_length = strlen(name);
					YELO_ASSERT(name_length <= k_name_length);

					strcpy_s(this->name, sizeof(this->name), name);
				}
			};

			std::vector<real_point2d> m_texcoords;
			std::vector<s_group*> m_groups;

		public:
			c_obj_file() : m_texcoords(), m_groups()
			{
			}
			~c_obj_file();

			HRESULT LoadFromFile(cstring obj_path);
			HRESULT CheckBspCompatibility(TagGroups::structure_bsp* bsp);
			HRESULT ReplaceVertexUVs(TagGroups::structure_bsp* bsp);
		};
#include "Tool/ImportLightmapUV_ObjFile.inl"

		void PLATFORM_API import_structure_lightmap_uvs(void** arguments)
		{
			printf_s("\n");

			struct s_arguments {
				cstring bsp_name;
				cstring obj_file;
			}* args = CAST_PTR(s_arguments*, arguments);

			datum_index bsp_index = tag_load<TagGroups::structure_bsp>(args->bsp_name, Flags::_tag_load_non_resolving_references);

			if(!bsp_index.IsNull() && !TagGroups::tag_is_read_only(bsp_index))
			{
				HRESULT hr = S_OK;

				TagGroups::structure_bsp* bsp = tag_get<TagGroups::structure_bsp>(bsp_index);

				char obj_file_path[_MAX_PATH];
				sprintf_s(obj_file_path, sizeof(obj_file_path), "data\\%s", args->obj_file);

				c_obj_file obj_file = c_obj_file();
				if(SUCCEEDED(hr)) hr = obj_file.LoadFromFile(obj_file_path);
				if(SUCCEEDED(hr)) hr = obj_file.CheckBspCompatibility(bsp);
				if(SUCCEEDED(hr)) hr = obj_file.ReplaceVertexUVs(bsp);
				if(SUCCEEDED(hr)) hr = tag_save(bsp_index) ? S_OK : E_FAIL;
				if(FAILED(hr))
					YELO_ERROR(_error_message_priority_warning, "OS_tool: failed to replace lightmap-uvs for structure_bsp: %s", args->bsp_name);
			}
			else
				YELO_ERROR(_error_message_priority_warning, "OS_tool: failed to open structure_bsp for editing: %s", args->bsp_name);
		}
	};
};
#endif