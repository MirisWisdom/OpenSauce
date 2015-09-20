/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/automation/input_output/c_test_run_input_xml.hpp>

#ifdef API_DEBUG

#include <YeloLib/configuration/c_configuration_file_factory.hpp>

namespace Yelo
{
    namespace Automation
    {
        c_test_run_input_xml::c_test_run_input_xml()
            : m_list() { }

        std::vector<std::string> c_test_run_input_xml::GetTests()
        {
            std::vector<std::string> test_list;
            for(auto& test : m_list.m_tests)
            {
                test_list.push_back(test);
            }
            return test_list;
        }

        void c_test_run_input_xml::Load(const std::string& input)
        {
            auto input_xml = Configuration::c_configuration_file_factory::CreateConfigurationFile(input);
            if(!input_xml)
            {
                throw new std::exception("Failed to create the test results");
            }
            input_xml->Load();

            auto root_node = input_xml->Root();
            if(!root_node)
            {
                throw new std::exception("Failed to load the test list");
            }
            m_list.GetValueFromParent(*root_node);
        }
    };
};
#endif
