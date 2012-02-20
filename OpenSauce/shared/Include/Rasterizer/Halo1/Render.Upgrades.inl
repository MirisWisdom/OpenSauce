/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

struct s_render_upgrades {
	bool dynamic_triangles_upgrades_enabled;
	PAD24;

	void LoadSettings(TiXmlElement* dx9_element)
	{
		if(dx9_element != NULL)
		{
			TiXmlElement* element = dx9_element->FirstChildElement("Upgrades");

			if(element != NULL)
				dynamic_triangles_upgrades_enabled = Settings::ParseBoolean( element->Attribute("dynamicTriangles") );
		}
	}
	void SaveSettings(TiXmlElement* dx9_element)
	{
		TiXmlElement* upgrades_element = NULL;

		upgrades_element = new TiXmlElement("Upgrades");
			dx9_element->LinkEndChild(upgrades_element);

		upgrades_element->SetAttribute("dynamicTriangles", Settings::BooleanToString(dynamic_triangles_upgrades_enabled));
	}

	// If we're not compiling with any upgrades, then don't waste space in the compiled code
	uint32 dynamic_triangles[Enums::k_rastizer_maximum_dynamic_triangles_upgrade <= Enums::k_rastizer_maximum_dynamic_triangles ? 1 :
							Enums::k_rastizer_maximum_dynamic_triangles_upgrade];

	void Initialize()
	{
		if(dynamic_triangles_upgrades_enabled && NUMBEROF(dynamic_triangles) > 1)
		{
			// redirect all dynamic triangle pointers to the new array
			for(int32 x = 0; x < NUMBEROF(K_DYNAMIC_TRIANGLE_ARRAY_UPGRADE_ADDRESS_LIST); x++)
				*K_DYNAMIC_TRIANGLE_ARRAY_UPGRADE_ADDRESS_LIST[x] = dynamic_triangles;
			// change all references to the dynamic triangle array to our new size
			for(int32 x = 0; x < NUMBEROF(K_MAXIMUM_DYNAMIC_TRIANGLES_UPGRADE_ADDRESS_LIST); x++)
				*K_MAXIMUM_DYNAMIC_TRIANGLES_UPGRADE_ADDRESS_LIST[x] = Enums::k_rastizer_maximum_dynamic_triangles_upgrade;
		}
	}

	void Dispose()
	{
	}

}g_render_upgrades;

// Not actually an upgrade, but a fix (nodes at index >43 would get stretched). However, we fix it in both sapien and ingame so I put the code here
void InitializeMaximumNodesPerModelFixes()
{
	static real_matrix3x4 vsh_constants__nodematrices[Enums::k_maximum_nodes_per_model+1];

	for(size_t x = 0; x < NUMBEROF(K_VSH_CONSTANTS__NODEMATRICES_REFERENCES); x++)
		*K_VSH_CONSTANTS__NODEMATRICES_REFERENCES[x] = &vsh_constants__nodematrices[0];
	for(size_t x = 0; x < NUMBEROF(K_VSH_CONSTANTS__NODEMATRICES_REFERENCES_PLUS_8); x++)
		*K_VSH_CONSTANTS__NODEMATRICES_REFERENCES_PLUS_8[x] = CAST_PTR(byte*,&vsh_constants__nodematrices[0]) + 8;

	for(size_t x = 0; x < NUMBEROF(K_RASTERIZER_GLOBALS_MAXIMUM_NODES_PER_MODEL_WRITE_VALUES); x++)
		*K_RASTERIZER_GLOBALS_MAXIMUM_NODES_PER_MODEL_WRITE_VALUES[x] = Enums::k_maximum_nodes_per_model;
	RasterizerGlobals()->maximum_nodes_per_model = Enums::k_maximum_nodes_per_model;
}