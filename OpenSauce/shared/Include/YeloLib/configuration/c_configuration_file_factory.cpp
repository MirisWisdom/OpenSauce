/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/configuration/c_configuration_file_factory.hpp>

#include <YeloLib/configuration/property_tree/c_property_tree_file_xml.hpp>
#include <YeloLib/configuration/property_tree/c_property_tree_file_json.hpp>

namespace Yelo
{
	namespace Configuration
	{
		configuration_file_ptr_t c_configuration_file_factory::CreateConfigurationFile(const std::string& file_path)
		{
			configuration_file_ptr_t configuration_file(nullptr);

			// Get the extension
			std::string::size_type extension_index = file_path.rfind('.');
			
			if(extension_index == std::string::npos)
			{
				return configuration_file;
			}

			std::string extension = file_path.substr(extension_index, std::string::npos);

			// Determine the file type
			if(extension == ".xml")
			{
				configuration_file.reset(new PropertyTree::c_property_tree_file_xml(file_path));
			}
			else if(extension == ".json")
			{
				configuration_file.reset(new PropertyTree::c_property_tree_file_json(file_path));
			}

			return configuration_file;
		}
	};
};