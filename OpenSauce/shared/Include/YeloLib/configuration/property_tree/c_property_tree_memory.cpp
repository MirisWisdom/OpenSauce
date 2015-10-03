/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/configuration/property_tree/c_property_tree_memory.hpp>

#include <YeloLib/configuration/property_tree/property_tree_logging.hpp>

namespace Yelo
{
    namespace Configuration
    {
        namespace PropertyTree
        {
            c_property_tree_memory::c_property_tree_memory(cstring data)
                : c_property_tree_leaf(m_property_tree_file),
                  m_input_stream(data)
            {
                m_property_tree_file.clear();
            }

            bool c_property_tree_memory::Load()
            {
                try
                {
                    ReadPropertyTree(m_input_stream, m_property_tree_file);

#ifdef PRINT_PROPERTY_TREES
                    PrintPropertyTree(m_property_tree_file);
#endif
                }
                catch (boost::property_tree::ptree_error e)
                { //-V565
#ifdef API_DEBUG
                    LOG(Error, std::string("Exception occurred when reading a property tree: ") + e.what());
#endif
                    return false;
                }
                return true;
            }

            std::unique_ptr<i_configuration_leaf> c_property_tree_memory::Root()
            {
                return std::make_unique<c_property_tree_leaf>(m_property_tree_file);
            }
        };
    };
};
