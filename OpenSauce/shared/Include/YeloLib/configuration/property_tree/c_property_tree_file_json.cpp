/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/configuration/property_tree/c_property_tree_file_json.hpp>

using namespace boost::property_tree;

namespace Yelo
{
	namespace Configuration { namespace PropertyTree
	{
		c_property_tree_file_json::c_property_tree_file_json(const std::string& file_path)
			: c_property_tree_file(file_path)
		{ }

		void c_property_tree_file_json::ReadPropertyTree(const std::string& file_path, ptree& property_tree)
		{
			read_json(file_path, property_tree);
		}

		void c_property_tree_file_json::WritePropertyTree(const std::string& file_path, ptree& property_tree)
		{
			write_json(file_path, property_tree);
		}
	};};
};