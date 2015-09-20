/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/automation/input_output/c_test_results_xml.hpp>

#ifdef API_DEBUG

#include <YeloLib/configuration/c_configuration_container.hpp>
#include <YeloLib/configuration/c_configuration_value.hpp>
#include <YeloLib/configuration/c_configuration_container_list.hpp>

namespace Yelo
{
    namespace Automation
    {
        c_test_result::c_test_result()
            : c_configuration_container("Test"),
              m_test_name("Name", ""),
              m_test_started("Started", false),
              m_test_finished("Finished", false),
              m_test_result("Result", false) {}

        Configuration::t_configuration_value_node_list c_test_result::GetMembers()
        {
            return Configuration::t_configuration_value_node_list
            {
                &m_test_name,
                &m_test_started,
                &m_test_finished,
                &m_test_result
            };
        }

        c_test_results_xml::c_test_results_xml()
            : c_configuration_container("Tests"),
              m_tests("Test", []()
                      {
                          return c_test_result();
                      }) {}

        Configuration::t_configuration_value_node_list c_test_results_xml::GetMembers()
        {
            return Configuration::t_configuration_value_node_list
            {
                &m_tests
            };
        }
    };
};
#endif
