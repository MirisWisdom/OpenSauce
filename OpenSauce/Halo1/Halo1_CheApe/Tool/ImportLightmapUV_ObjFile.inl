/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/

c_obj_file::~c_obj_file()
{
	for(std::vector<s_group*>::const_iterator iter = m_groups.begin();
		iter != m_groups.end(); ++iter)
		delete *iter;

	m_groups.clear();
	m_texcoords.clear();
}

HRESULT c_obj_file::LoadFromFile(const std::string& obj_path)
{
	fputs("loading lightmaps obj file...", stdout);

	FILE* file = nullptr;
	fopen_s(&file, obj_path.c_str(), "r+");
	if(!file || ferror(file))
	{
		if(file)
			fclose(file);

		fputs("failed\n", stdout);
		YELO_WARN("OS_tool:import: failed to open obj file");
		return E_FAIL;
	}

	s_group* current_group = nullptr;
	char line[256];
	while (fgets(line, NUMBEROF(line)-1, file) != nullptr)
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
			if(id[0] != '\0')
			{
				if(current_group)
				{	
					m_groups.push_back(current_group);
					current_group = nullptr;
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
		current_group = nullptr;
	}
	fputs("done\n", stdout);

	fclose(file);
	if(m_groups.size() == 0)
	{
		YELO_WARN("OS_tool:import: obj file contains no groups");
		return E_FAIL;
	}
	if(m_texcoords.size() == 0)
	{
		YELO_WARN("OS_tool:import: obj file contains no texture coordinates");
		return E_FAIL;
	}

	fputs("\n", stdout);
	fputs("obj file stats\n", stdout);
	printf_s("\tgroups: %u\n", m_groups.size());
	printf_s("\ttexture coordinates: %u\n", m_texcoords.size());
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
		YELO_WARN("obj group count (%i) does not match bsp lightmap count (%i)\n",
			m_groups.size(), lightmaps_count);
		return E_FAIL;
	}
	for(int32 i = 0; i < lightmaps_count; i++)
	{
		TagGroups::structure_bsp_lightmap* bsp_lightmap = blam::tag_block_get_element(bsp->lightmaps, i);

		int32 surface_count = 0;
		for (const auto& bsp_material : bsp_lightmap->materials)
		{
			surface_count += bsp_material.surface_count;
		}

		if(surface_count != m_groups[i]->faces.size())
		{
			printf_s("\nfailed\n");
			YELO_WARN("lightmap %i surface count (%i) does not match obj group %i face count (%i)",
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

	size_t group_index = 0;
	for (auto& bsp_lightmap : bsp->lightmaps)
	{
		const auto group = m_groups.at(group_index++);

		if(bsp_lightmap.bitmap == NONE)
			continue;

		int32 surface_offset = 0;
		for (auto& bsp_material : bsp_lightmap.materials)
		{
			// Get the lightmap vertices by skipping over the bsp's vertices
			size_t lightmap_verticies_offset = 
				bsp_material.vertices.vertex_start_index * sizeof(Rasterizer::environment_vertex_uncompressed);

			size_t face_index = surface_offset;
			for (int32 k = 0; k < bsp_material.surface_count; k++, face_index++)
			{
				TagGroups::structure_surface* surface = blam::tag_block_get_element(bsp->surfaces, bsp_material.surfaces + k);

				const s_face& face = group->faces.at(face_index);

				for (int32 x = 0; x < NUMBEROF(surface->a); x++)
				{
					auto lightmap_vertex = 
						blam::tag_data_get_pointer<Rasterizer::environment_lightmap_vertex_uncompressed>(
							bsp_material.uncompressed_vertices, lightmap_verticies_offset, surface->a[x]);

					lightmap_vertex->texcoord.x =
						m_texcoords[face.vertex_indicies[x].texcoord_index - 1].x;
					lightmap_vertex->texcoord.y =
						(m_texcoords[face.vertex_indicies[x].texcoord_index - 1].y - 1) * -1;
				}
			}

			surface_offset += bsp_material.surface_count;
		}				
	}

	printf_s("done\n");
	return S_OK;
}