/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/ImportLightmapUV.hpp"
#if PLATFORM_ID == PLATFORM_TOOL

#include <blamlib/Halo1/structures/structure_bsp_definitions.hpp>

#include "Engine/EngineFunctions.hpp"
#include "TagGroups/TagGroups.hpp"
#include "Common/YeloSettings.hpp"
#include "Common/StringEditing.hpp"

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

			HRESULT LoadFromFile(std::string obj_path);
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

				std::string obj_file_path(Yelo::Settings::Get().GetDataPath());
				StringEditing::AppendCharIfAbsent(obj_file_path, '\\');

				char* obj_argument = CAST_QUAL(char*, args->obj_file);
				while(obj_argument[0] == '\\')
					obj_argument++;

				obj_file_path.append(obj_argument);

				bool has_extension = false;
				if(obj_file_path.find(".obj", obj_file_path.length() - 4) != std::string::npos)
					has_extension = true;

				if(!has_extension)
					obj_file_path.append(".obj");

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