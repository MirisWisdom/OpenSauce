/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/configuration/property_tree/c_property_tree_memory.hpp>

namespace Yelo
{
	namespace Configuration { namespace PropertyTree
	{
		class c_property_tree_memory_xml final
			: public c_property_tree_memory
		{
		public:
			c_property_tree_memory_xml(cstring data);

		private:
			void ReadPropertyTree(std::istringstream& data, boost::property_tree::ptree& property_tree) override;
		};
	};};
};