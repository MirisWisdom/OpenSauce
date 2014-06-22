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

			for (auto& sub_tree : tree)
			{	
				OutputDebugString((std::string("").assign(depth * 2, ' ') + "* ").c_str());
				OutputDebugString((sub_tree.first + "=\"" + sub_tree.second.get_value("") + "\"\r\n").c_str());

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