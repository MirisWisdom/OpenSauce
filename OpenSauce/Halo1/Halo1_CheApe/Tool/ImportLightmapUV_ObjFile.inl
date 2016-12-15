/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/

c_obj_file::~c_obj_file()
{
	m_groups.clear();
	m_texcoords.clear();
}

HRESULT c_obj_file::LoadFromFile(const std::string& obj_path)
{
	fputs("loading lightmaps obj file...", stdout);

	// Open the obj file
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

	// Read a line
	std::shared_ptr<s_group> current_group;
	char line[256];
	while (fgets(line, NUMBEROF(line)-1, file) != nullptr)
	{
		// Try to read the line as a texture coordinate
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
		// Try to read the line as the start of a group
		if(sscanf_s(line, "g %s[^\n]", id, NUMBEROF(id)-1))
		{
			if(id[0] != '\0')
			{
				// If current group is not null, add the current group to the list and start a new one
				if(current_group)
				{	
					m_groups.push_back(current_group);
					current_group = nullptr;
				}

				// Find the lightmap index at the end of the id
				std::string group_name(id);
				auto index_offset = group_name.find_last_not_of("0123456789");
				if(index_offset == std::string::npos)
				{
					YELO_WARN("OS_tool:import: obj file has a group name that does not end with the lightmap index");
					return E_FAIL;
				}
				index_offset++;

				if(index_offset >= group_name.size())
				{
					YELO_WARN("OS_tool:import: obj file has a group name that does not end with the lightmap index");
					return E_FAIL;
				}

				// Convert the index section of the group name to an int
				auto index_string = group_name.substr(index_offset);
				int lightmap_index = atoi(index_string.c_str());

				// Create a new group
				current_group = std::make_shared<s_group>(id, lightmap_index);
			}
		}
		// Try to read the line as a face in the current group
		else if(sscanf_s(line, "f %i/%i/%i %i/%i/%i %i/%i/%i[^\n]",
			&face.vertex_indicies[2].position_index, &face.vertex_indicies[2].texcoord_index, &face.vertex_indicies[2].normals_index, 
			&face.vertex_indicies[1].position_index, &face.vertex_indicies[1].texcoord_index, &face.vertex_indicies[1].normals_index, 
			&face.vertex_indicies[0].position_index, &face.vertex_indicies[0].texcoord_index, &face.vertex_indicies[0].normals_index) == 9)
		{
			current_group->faces.push_back(face);
		}
		// Try to read the line as a face in the current group
		else if(sscanf_s(line, "f %i/%i %i/%i %i/%i[^\n]", 
			&face.vertex_indicies[2].position_index, &face.vertex_indicies[2].texcoord_index, 
			&face.vertex_indicies[1].position_index, &face.vertex_indicies[1].texcoord_index, 
			&face.vertex_indicies[0].position_index, &face.vertex_indicies[0].texcoord_index) == 6)
		{
			current_group->faces.push_back(face);
		}
	}

	// Add the last group to the list
	if(current_group)
	{	
		m_groups.push_back(current_group);
		current_group = nullptr;
	}
	fputs("done\n", stdout);

	// Verify that one or more groups/texcoord was read
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

	// Print stats
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

	// Lightmap count does not include the non-uv'd transparent meshes
	int32 lightmaps_count = bsp->lightmaps.Count - 1;

	// Verify that the lightmap count matches the loaded group count
	if(lightmaps_count != m_groups.size() )
	{
		fputs("failed\n", stdout);
		YELO_WARN("obj group count (%i) does not match bsp lightmap count (%i)\n",
			m_groups.size(), lightmaps_count);
		return E_FAIL;
	}

	for(int32 i = 0; i < lightmaps_count; i++)
	{
		auto* bsp_lightmap = blam::tag_block_get_element(bsp->lightmaps, i);

		int32 surface_count = 0;
		for (const auto& bsp_material : bsp_lightmap->materials)
		{
			surface_count += bsp_material.surface_count;
		}

		// Find the group for this lightmap index
		auto group_iter = std::find_if(m_groups.begin(), m_groups.end(),
			[i](std::shared_ptr<s_group>& entry)
			{
				return i == entry->lightmap_index;
			}
		);

		if(group_iter == m_groups.end())
		{
			YELO_WARN("OS_tool:import: obj file contains no group for lightmap %i", i);
			return E_FAIL;
		}

		// Verify that the group's face count matches the lightmaps
		if(surface_count != (*group_iter)->faces.size())
		{
			printf_s("\nfailed\n");
			YELO_WARN("lightmap %i surface count (%i) does not match the obj group face count (%i)",
				i, surface_count, m_groups[i]->faces.size());
			return E_FAIL;
		}
	}

	fputs("done\n", stdout);
	return S_OK;
}

HRESULT c_obj_file::ReplaceVertexUVs(TagGroups::structure_bsp* bsp)
{
	fputs("replacing lightmap texture coordinates...", stdout);

	for (int32 i = 0; i < (bsp->lightmaps.Count - 1); ++i)
	{
		auto& bsp_lightmap = bsp->lightmaps[i];		

		if(bsp_lightmap.bitmap == NONE)
			continue;

		// Find the group for this lightmap index
		auto group_iter = std::find_if(m_groups.begin(), m_groups.end(),
			[i](std::shared_ptr<s_group>& entry)
			{
				return i == entry->lightmap_index;
			}
		);

		if(group_iter == m_groups.end())
		{
			YELO_WARN("OS_tool:import: obj file contains no group for lightmap %i", i);
			return E_FAIL;
		}

		const auto& group = *group_iter->get();

		int32 surface_offset = 0;
		for (auto& bsp_material : bsp_lightmap.materials)
		{
			// Get the lightmap vertices by skipping over the bsp's vertices
			size_t lightmap_verticies_offset = 
				bsp_material.vertices.vertex_start_index * sizeof(Rasterizer::environment_vertex_uncompressed);

			size_t face_index = surface_offset;
			for (int32 k = 0; k < bsp_material.surface_count; k++, face_index++)
			{
				auto* surface = blam::tag_block_get_element(bsp->surfaces, bsp_material.surfaces + k);

				const auto& face = group.faces.at(face_index);

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