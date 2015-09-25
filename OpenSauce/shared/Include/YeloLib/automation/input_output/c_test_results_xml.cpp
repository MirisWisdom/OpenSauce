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
              m_test_result("Result", false),
              m_test_messages("Message", "") {}

        const std::vector<Configuration::i_configuration_value* const> c_test_result::GetMembers()
        {
            return std::vector<i_configuration_value* const>
            {
                &m_test_name,
                &m_test_started,
                &m_test_finished,
                &m_test_result,
                &m_test_messages
            };
        }

        c_test_results_xml::c_test_results_xml()
            : c_configuration_container("Tests"),
              m_tests("Test", []()
                      {
                          return c_test_result();
                      }) {}

        const std::vector<Configuration::i_configuration_value* const> c_test_results_xml::GetMembers()
        {
            return std::vector<i_configuration_value* const>
            {
                &m_tests
            };
        }
    };
};
#endif
