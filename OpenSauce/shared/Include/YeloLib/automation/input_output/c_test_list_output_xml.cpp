/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/automation/input_output/c_test_list_output_xml.hpp>

#ifdef API_DEBUG

#include <YeloLib/configuration/c_configuration_file_factory.hpp>

namespace Yelo
{
    namespace Automation
    {
        c_test_list_output_xml::c_test_list_output_xml()
            : m_list() { }

        void c_test_list_output_xml::TestFound(const std::string& name)
        {
            m_list.m_tests.AddEntry().Get().assign(name);
        }

        void c_test_list_output_xml::Save(const std::string& output)
        {
            auto output_xml = Configuration::c_configuration_file_factory::CreateConfigurationFile(output);
            if (!output_xml)
            {
                throw new std::exception("Failed to create the test list");
            }
            auto root_node = output_xml->Root();
            if (!root_node)
            {
                throw new std::exception("Failed to load the test list");
            }
            m_list.SetValueToParent(*root_node);
            output_xml->Save();
        }
    };
};
#endif
