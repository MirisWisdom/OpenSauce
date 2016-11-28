/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/configuration/property_tree/c_property_tree_file_xml.hpp>

using namespace boost::property_tree;

namespace Yelo
{
	namespace Configuration { namespace PropertyTree
	{
		c_property_tree_file_xml::c_property_tree_file_xml(const std::string& file_path)
			: c_property_tree_file(file_path)
		{ }

		void c_property_tree_file_xml::ReadPropertyTree(const std::string& file_path, ptree& property_tree)
		{
			read_xml(file_path, property_tree, xml_parser::trim_whitespace);
		}

		void c_property_tree_file_xml::WritePropertyTree(const std::string& file_path, ptree& property_tree) 
		{
			xml_writer_settings<char> settings('\t', 1);
			write_xml(file_path, property_tree, std::locale(), settings);
		}
	};};
};