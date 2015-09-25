/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#ifdef API_DEBUG

#include <YeloLib/configuration/c_configuration_container.hpp>
#include <YeloLib/configuration/c_configuration_value.hpp>
#include <YeloLib/configuration/c_configuration_container_list.hpp>
#include <YeloLib/configuration/c_configuration_value_list.hpp>

namespace Yelo
{
    namespace Automation
    {
        class c_test_result : public Configuration::c_configuration_container
        {
        public:
            Configuration::c_configuration_value<std::string> m_test_name;
            Configuration::c_configuration_value<bool> m_test_started;
            Configuration::c_configuration_value<bool> m_test_finished;
            Configuration::c_configuration_value<bool> m_test_result;
            Configuration::c_configuration_value_list<std::string> m_test_messages;

            c_test_result();

        protected:
            const std::vector<i_configuration_value* const> GetMembers() override;
        };

        class c_test_results_xml : public Configuration::c_configuration_container
        {
        public:
            Configuration::c_configuration_container_list<c_test_result> m_tests;

            c_test_results_xml();

        protected:
            const std::vector<i_configuration_value* const> GetMembers() override;
        };
    };
};
#endif
