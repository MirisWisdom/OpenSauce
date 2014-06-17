/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/configuration/c_configuration_file_factory.hpp>

#include <YeloLib/configuration/property_tree/c_property_tree_file_xml.hpp>
#include <YeloLib/configuration/property_tree/c_property_tree_file_json.hpp>

using namespace std;

namespace Yelo
{
	namespace Configuration
	{
		configuration_file_ptr_t c_configuration_file_factory::CreateConfigurationFile(const string& file_path)
		{
			configuration_file_ptr_t configuration_file(nullptr);

			// Determine the file type
			if (EndsWith(file_path, ".xml"))
			{
				configuration_file.reset(new PropertyTree::c_property_tree_file_xml(file_path));
			}
			else if (EndsWith(file_path, ".json"))
			{
				configuration_file.reset(new PropertyTree::c_property_tree_file_json(file_path));
			}
			// TODO: .info?

			return configuration_file;
		}
	};
};