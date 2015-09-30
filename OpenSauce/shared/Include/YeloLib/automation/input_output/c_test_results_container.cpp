/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/automation/input_output/c_test_results_container.hpp>

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
              m_name("Name", ""),
              m_started("Started", false),
              m_result("Result", false),
              m_messages("Message", "") {}

        const std::vector<Configuration::i_configuration_value* const> c_test_result::GetMembers()
        {
            return std::vector<i_configuration_value* const>
                {
                    &m_name,
                    &m_started,
                    &m_result,
                    &m_messages
                };
        }

        c_test_results_container::c_test_results_container()
            : c_configuration_container("Tests"),
              m_tests("Test", []()
                      {
                          return c_test_result();
                      }) {}

        const std::vector<Configuration::i_configuration_value* const> c_test_results_container::GetMembers()
        {
            return std::vector<i_configuration_value* const>
                {
                    &m_tests
                };
        }
    };
};
#endif
