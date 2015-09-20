/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#ifdef API_DEBUG

#include <YeloLib/configuration/c_configuration_container.hpp>
#include <YeloLib/configuration/c_configuration_value_list.hpp>

namespace Yelo
{
    namespace Automation
    {
        class c_test_list_xml
            : public Configuration::c_configuration_container
        {
        public:
            Configuration::c_configuration_value_list<std::string> m_tests;

            c_test_list_xml();

        protected:
            Configuration::t_configuration_value_node_list GetMembers() override;
        };
    };
};
#endif
