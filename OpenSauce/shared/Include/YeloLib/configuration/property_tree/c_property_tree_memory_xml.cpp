/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/configuration/property_tree/c_property_tree_memory_xml.hpp>

using namespace boost::property_tree;

namespace Yelo
{
	namespace Configuration { namespace PropertyTree
	{
		c_property_tree_memory_xml::c_property_tree_memory_xml(cstring data)
			: c_property_tree_memory(data)
		{ }

		void c_property_tree_memory_xml::ReadPropertyTree(std::istringstream& data, ptree& property_tree)
		{
			read_xml(data, property_tree, xml_parser::trim_whitespace);
		}
	};};
};