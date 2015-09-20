/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/automation/input_output/c_test_list_xml.hpp>

#ifdef API_DEBUG

namespace Yelo
{
    namespace Automation
    {
        c_test_list_xml::c_test_list_xml()
            : c_configuration_container("Tests"),
              m_tests("Test", "") {}

        Configuration::t_configuration_value_node_list c_test_list_xml::GetMembers()
        {
            return Configuration::t_configuration_value_node_list
            {
                &m_tests
            };
        }
    };
};
#endif
