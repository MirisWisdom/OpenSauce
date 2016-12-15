/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/configuration/property_tree/property_tree_logging.hpp>

#ifdef PRINT_PROPERTY_TREES
#include <iostream> 

using namespace boost::property_tree;

namespace Yelo
{
	namespace Configuration { namespace PropertyTree
	{
		void PrintPropertyTree(const ptree& tree, const int depth)
		{
			if (depth == 0)
			{
				OutputDebugString("PrintPropertyTree:\r\n");
			}

			std::string depth_string(depth * 2, ' ');
			for (auto& sub_tree : tree)
			{				
				OutputDebugString(depth_string.c_str());
				OutputDebugString("* ");
				OutputDebugString(sub_tree.first.c_str());
				OutputDebugString("=\"");
				OutputDebugString(sub_tree.second.get_value("").c_str());
				OutputDebugString("\"\r\n");

				PrintPropertyTree(sub_tree.second, depth + 1);
			}

			if (depth == 0)
			{
				OutputDebugString("\r\n");
			}
		};
	};};
};
#endif