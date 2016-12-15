/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/ImportLightmapUV.hpp"
#if PLATFORM_TYPE == PLATFORM_TOOL

#include <blamlib/Halo1/structures/structure_bsp_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/settings/c_settings_cheape.hpp>

#include "Engine/EngineFunctions.hpp"
#include "TagGroups/TagGroups.hpp"
#include "Settings/Settings.hpp"
#include "Common/StringEditing.hpp"

namespace Yelo
{
	namespace Tool
	{
		class c_obj_file
		{
			struct s_vertex_indices
			{
				const uint32 position_index;
				const uint32 normals_index;
				const uint32 texcoord_index;

				s_vertex_indices()
					: position_index(0)
					, normals_index(0)
					, texcoord_index(0)
				{ }
			};

			struct s_face
			{
				s_vertex_indices vertex_indicies[3];

				s_face()
					: vertex_indicies()
				{ }
			};

			struct s_group
			{
				static const size_t k_name_length = 255;

				std::vector<s_face> faces;
				const std::string group_name;
				const int lightmap_index;

				s_group(cstring name, const int index)
					: faces()
					, group_name(name)
					, lightmap_index(index)
				{ }
			};

			std::vector<real_point2d> m_texcoords;
			std::vector<std::shared_ptr<s_group>> m_groups;

		public:
			c_obj_file()
				: m_texcoords()
				, m_groups()
			{ }

			~c_obj_file();

			HRESULT LoadFromFile(const std::string& obj_path);
			HRESULT CheckBspCompatibility(TagGroups::structure_bsp* bsp);
			HRESULT ReplaceVertexUVs(TagGroups::structure_bsp* bsp);
		};
#include "Tool/ImportLightmapUV_ObjFile.inl"

		void PLATFORM_API import_structure_lightmap_uvs(char* arguments[])
		{
			printf_s("\n");

			struct s_arguments {
				cstring bsp_name;
				cstring obj_file;
			}* args = CAST_PTR(s_arguments*, arguments);

			// Load the BSP tag and check it is not readonly
			datum_index bsp_index = blam::tag_load<TagGroups::structure_bsp>(args->bsp_name, FLAG(Flags::_tag_load_non_resolving_references_bit));

			if(bsp_index.IsNull() || blam::tag_read_only(bsp_index))
			{
				YELO_WARN("OS_tool: failed to open structure_bsp for editing: %s", args->bsp_name);
				return;
			}

			auto* bsp = blam::tag_get<TagGroups::structure_bsp>(bsp_index);

			// Build the obj file path
			std::string obj_file_path(Yelo::Settings::c_settings_cheape::Profile().GetDataPath());
			StringEditing::AppendCharIfAbsent(obj_file_path, '\\');

			// Skip any leading folder dividers
			cstring obj_argument = args->obj_file;
			while((obj_argument[0] == '\\') || (obj_argument[0] == '/'))
			{
				obj_argument++;
			}

			obj_file_path.append(obj_argument);

			// Add the obj extension if missing
			if(obj_file_path.find(".obj") == std::string::npos)
			{
				obj_file_path.append(".obj");
			}

			auto obj_file = c_obj_file();

			HRESULT hr = S_OK;
			if(SUCCEEDED(hr)) hr = obj_file.LoadFromFile(obj_file_path);
			if(SUCCEEDED(hr)) hr = obj_file.CheckBspCompatibility(bsp);
			if(SUCCEEDED(hr)) hr = obj_file.ReplaceVertexUVs(bsp);
			if(SUCCEEDED(hr)) hr = blam::tag_save(bsp_index) ? S_OK : E_FAIL;
			if(FAILED(hr))
				YELO_WARN("OS_tool: failed to replace lightmap-uvs for structure_bsp: %s", args->bsp_name);
		}
	};
};
#endif