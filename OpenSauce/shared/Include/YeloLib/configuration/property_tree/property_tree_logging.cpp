/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/configuration/property_tree/property_tree_logging.hpp>

#ifdef PRINT_PROPERTY_TREES

using namespace boost::property_tree;

namespace Yelo
{
    namespace Configuration
    {
        namespace PropertyTree
        {
            void PrintPropertyTree(const ptree& tree, const int depth)
            {
                if (depth == 0)
                {
                    LOG(Verbose, "PrintPropertyTree:");
                }

                std::string depth_string(depth * 2, ' ');
                for (auto& sub_tree : tree)
                {
                    LOG(Verbose, depth_string + "* " + sub_tree.first + "=\"" + sub_tree.second.get_value("") + "\"");
                    PrintPropertyTree(sub_tree.second, depth + 1);
                }
            };
        };
    };
};
#endif
