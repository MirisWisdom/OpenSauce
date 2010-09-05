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

c_obj_file::~c_obj_file()
{
	for(std::vector<s_group*>::const_iterator iter = m_groups.begin();
		iter != m_groups.end(); ++iter)
		delete *iter;

	m_groups.clear();
	m_texcoords.clear();
}

HRESULT c_obj_file::LoadFromFile(cstring obj_path)
{
	fputs("loading lightmaps obj file...", stdout);
	FILE* file = NULL;
	fopen_s(&file, obj_path, "r+");
	if(!file || ferror(file))
	{
		if(file)
			fclose(file);

		fputs("failed\n", stdout);
		YELO_ERROR(_error_message_priority_warning, "OS_tool:import: failed to open obj file");
		return E_FAIL;
	}

	s_group* current_group = NULL;
	char line[256];
	while (fgets(line, NUMBEROF(line)-1, file) != NULL)
	{
		real floatarr[3];
		if(sscanf_s(line, "vt %f %f %f[^\n]", &floatarr[0], &floatarr[1], &floatarr[2]))
		{
			real_point2d tex;
			tex.x = floatarr[0];
			tex.y = floatarr[1];
			m_texcoords.push_back(tex);
		}

		char id[s_group::k_name_length+1];
		id[0] = '\0';

		s_face face;
		if(sscanf_s(line, "g %s[^\n]", id, NUMBEROF(id)-1))
		{
			if(strlen(id) != 0)
			{
				if(current_group)
				{	
					m_groups.push_back(current_group);
					current_group = NULL;
				}
				current_group = new s_group(id);
			}
		}
		else if(sscanf_s(line, "f %i/%i/%i %i/%i/%i %i/%i/%i[^\n]", 
			&face.vertex_indicies[2].position_index, &face.vertex_indicies[2].texcoord_index, &face.vertex_indicies[2].normals_index, 
			&face.vertex_indicies[1].position_index, &face.vertex_indicies[1].texcoord_index, &face.vertex_indicies[1].normals_index, 
			&face.vertex_indicies[0].position_index, &face.vertex_indicies[0].texcoord_index, &face.vertex_indicies[0].normals_index) == 9)
		{
			current_group->faces.push_back(face);
		}
		else if(sscanf_s(line, "f %i/%i %i/%i %i/%i[^\n]", 
			&face.vertex_indicies[2].position_index, &face.vertex_indicies[2].texcoord_index, 
			&face.vertex_indicies[1].position_index, &face.vertex_indicies[1].texcoord_index, 
			&face.vertex_indicies[0].position_index, &face.vertex_indicies[0].texcoord_index) == 6)
		{
			current_group->faces.push_back(face);
		}
	}
	if(current_group)
	{	
		m_groups.push_back(current_group);
		current_group = NULL;
	}
	fputs("done\n", stdout);

	fclose(file);
	if(m_groups.size() == 0)
	{
		YELO_ERROR(_error_message_priority_warning, "OS_tool:import: obj file contains no groups");
		return E_FAIL;
	}
	if(m_texcoords.size() == 0)
	{
		YELO_ERROR(_error_message_priority_warning, "OS_tool:import: obj file contains no texture coordinates");
		return E_FAIL;
	}

	fputs("\n", stdout);
	fputs("obj file stats\n", stdout);
	printf_s("\tgroups: %s\n", m_groups.size());
	printf_s("\ttexture coordinates: %s\n", m_texcoords.size());
	fputs("\n", stdout);

	return S_OK;
}

HRESULT c_obj_file::CheckBspCompatibility(TagGroups::structure_bsp* bsp)
{
	fputs("checking obj file compatibility with structure bsp...", stdout);

	int32 lightmaps_count = bsp->lightmaps.Count - 1;

	if(lightmaps_count != m_groups.size() )
	{
		fputs("failed\n", stdout);
		YELO_ERROR(_error_message_priority_warning, "obj group count (%i) does not match bsp lightmap count (%i)\n", 
			m_groups.size(), lightmaps_count);
		return E_FAIL;
	}
	for(int32 i = 0; i < lightmaps_count; i++)
	{
		TagGroups::structure_bsp_lightmap* bsp_lightmap = tag_block_get_element(bsp->lightmaps, i);

		int32 surface_count = 0;
		for(int32 j = 0; j < bsp_lightmap->materials.Count; j++)
		{
			TagGroups::structure_bsp_material* bsp_material = tag_block_get_element(bsp_lightmap->materials, j);

			surface_count += bsp_material->surface_count;
		}

		if(surface_count != m_groups[i]->faces.size())
		{
			printf_s("\nfailed\n");
			YELO_ERROR(_error_message_priority_warning, "lightmap %i surface count (%i) does not match obj group %i face count (%i)",
				i, surface_count, i, m_groups[i]->faces.size());
			return E_FAIL;					
		}
	}

	fputs("done\n", stdout);
	return S_OK;
}

HRESULT c_obj_file::ReplaceVertexUVs(TagGroups::structure_bsp* bsp)
{
	fputs("replacing lightmap texture coordinates...", stdout);

	for(int32 i = 0; i < bsp->lightmaps.Count; i++)
	{
		TagGroups::structure_bsp_lightmap* bsp_lightmap = tag_block_get_element(bsp->lightmaps, i);

		if(bsp_lightmap->bitmap == NONE)
			continue;

		int32 surface_offset = 0;
		for(int32 j = 0; j < bsp_lightmap->materials.Count; j++)
		{
			TagGroups::structure_bsp_material* bsp_material = tag_block_get_element(bsp_lightmap->materials, j);

			// Get the lightmap vertices by skipping over the bsp's vertices
			size_t lightmap_verticies_offset = 
				bsp_material->vertices.vertex_start_index * sizeof(Rasterizer::environment_vertex_uncompressed);

			Rasterizer::environment_lightmap_vertex_uncompressed* lightmap_verticies = 
				CAST_PTR(Rasterizer::environment_lightmap_vertex_uncompressed*, 
					bsp_material->uncompressed_vertices.Bytes() + lightmap_verticies_offset);

			for(int32 k = 0; k < bsp_material->surface_count; k++)
			{
				TagGroups::structure_surface* surface = tag_block_get_element(bsp->surfaces, bsp_material->surfaces + k);

				s_face face = m_groups.at(i)->faces.at(surface_offset + k);

				for(int32 x = 0; x < NUMBEROF(surface->a); x++)
				{
					lightmap_verticies[surface->a[x]].texcoord.x = 
						m_texcoords[face.vertex_indicies[x].texcoord_index - 1].x;
					lightmap_verticies[surface->a[x]].texcoord.y = 
						(m_texcoords[face.vertex_indicies[x].texcoord_index - 1].y - 1) * -1;
				}
			}

			surface_offset += bsp_material->surface_count;
		}				
	}

	printf_s("done\n");
	return S_OK;
}