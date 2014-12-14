/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/Screen/c_screen_definition_registry.hpp"

#if !PLATFORM_IS_DEDI

#include <YeloLib/configuration/c_configuration_file_factory.hpp>

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Screen
	{
		c_screen_definition_registry::c_screen_definition_registry(c_packed_file& ui_package)
			: m_ui_packages(ui_package)
		{ }

		bool c_screen_definition_registry::GetScreenDefinition(const std::string& screen_id, Definitions::c_screen_definition& output)
		{
			uint32 data_size = 0;
			auto* data_ptr = CAST_PTR(cstring, m_ui_packages.GetDataPointer(screen_id.c_str(), &data_size));
			if(!data_ptr)
			{
				return false;
			}

			// Get the screen definition
			auto& config_file = Configuration::c_configuration_file_factory::CreateConfigurationFile(data_ptr, "json");
			if(!config_file->Load())
			{
				return false;
			}

			auto& screen = config_file->Root()->GetChild("Screen");
			output.GetValue(*screen);

			return true;
		}
	};};};
};
#endif