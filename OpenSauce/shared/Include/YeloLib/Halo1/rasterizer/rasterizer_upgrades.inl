/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

class c_render_upgrades
{
private:
	class c_settings
		: public Configuration::c_configuration_container
	{
	public:
		Configuration::c_configuration_value<bool> m_dynamic_triangles;
		Configuration::c_configuration_value<bool> m_model_node_stretching_fix;
	
		c_settings()
			: Configuration::c_configuration_container("Rasterizer.Upgrades")
			, m_dynamic_triangles("DynamicTriangles", true)
			, m_model_node_stretching_fix("MaximumModelNodes", true)
		{ }

	protected:
		const std::vector<Configuration::i_configuration_value* const> GetMembers()
		{
			std::vector<Configuration::i_configuration_value* const> values =
			{
				&m_dynamic_triangles,
				&m_model_node_stretching_fix,
			};

			return values;
		}
	};
	std::unique_ptr<c_settings> m_settings;
	
public:
	// If we're not compiling with any upgrades, then don't waste space in the compiled code
	uint32 dynamic_triangles[Enums::k_rastizer_maximum_dynamic_triangles_upgrade <= Enums::k_rastizer_maximum_dynamic_triangles ? 1 :
							Enums::k_rastizer_maximum_dynamic_triangles_upgrade];

	void Initialize()
	{
		m_settings = std::make_unique<c_settings>();
		Settings::RegisterConfigurationContainer(m_settings.get(), nullptr,
			[this]()
			{
				if(m_settings->m_dynamic_triangles && NUMBEROF(dynamic_triangles) > 1)
				{
					// redirect all dynamic triangle pointers to the new array
					for(auto ptr : K_DYNAMIC_TRIANGLE_ARRAY_UPGRADE_ADDRESS_LIST)
						*ptr = dynamic_triangles;
					// change all references to the dynamic triangle array to our new size
					for(auto ptr : K_MAXIMUM_DYNAMIC_TRIANGLES_UPGRADE_ADDRESS_LIST)
						*ptr = Enums::k_rastizer_maximum_dynamic_triangles_upgrade;
				}

				if(m_settings->m_model_node_stretching_fix)
					InitializeMaximumNodesPerModelFixes();
			}
		);
	}

	void Dispose()
	{
		Settings::UnregisterConfigurationContainer(m_settings.get());
		m_settings.reset();
	}

	// Not actually an upgrade, but a fix (nodes at index >43 would get stretched). However, we fix it in both sapien and ingame so I put the code here
	static void InitializeMaximumNodesPerModelFixes()
	{
		static real_matrix3x4 vsh_constants__nodematrices[Enums::k_maximum_nodes_per_model+1];

		for(auto ptr : K_VSH_CONSTANTS__NODEMATRICES_REFERENCES)
			*ptr = &vsh_constants__nodematrices[0];
		for(auto ptr : K_VSH_CONSTANTS__NODEMATRICES_REFERENCES_PLUS_8)
			*ptr = CAST_PTR(byte*,&vsh_constants__nodematrices[0]) + 8;

		for(auto ptr : K_RASTERIZER_GLOBALS_MAXIMUM_NODES_PER_MODEL_WRITE_VALUES)
			*ptr = Enums::k_maximum_nodes_per_model;
		RasterizerGlobals()->maximum_nodes_per_model = Enums::k_maximum_nodes_per_model;
	}
};
static c_render_upgrades g_render_upgrades;