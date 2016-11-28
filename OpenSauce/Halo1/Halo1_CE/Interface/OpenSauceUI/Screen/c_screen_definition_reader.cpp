/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/Screen/c_screen_definition_reader.hpp"

#if !PLATFORM_IS_DEDI

#include <YeloLib/configuration/c_configuration_file_factory.hpp>

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Screen
	{
		bool c_screen_definition_registry::GetScreenDefinition(c_packed_file& ui_package, const std::string& screen_id, Definitions::c_screen_definition& output)
		{
			uint32 data_size = 0;
			auto* data_ptr = CAST_PTR(cstring, ui_package.GetDataPointer(screen_id.c_str(), &data_size));

			YELO_ASSERT_DISPLAY(data_ptr, "Failed to find a screen definition called %s", screen_id.c_str());

			if(!data_ptr)
			{
				return false;
			}

			std::string data_string(data_ptr, data_size);

			// Get the screen definition
			auto& config_file = Configuration::c_configuration_file_factory::CreateConfigurationFile(data_string.c_str(), "json");
			if(!config_file->Load())
			{
				YELO_ASSERT_DISPLAY(false, "Failed to load a screen definition called %s", screen_id.c_str());

				return false;
			}

			auto& screen = config_file->Root()->GetChild("Screen");
			output.GetValue(*screen);

			return true;
		}
	};};};
};
#endif