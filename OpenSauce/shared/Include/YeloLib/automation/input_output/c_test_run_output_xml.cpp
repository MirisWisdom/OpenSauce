/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/automation/input_output/c_test_run_output_xml.hpp>

#ifdef API_DEBUG

#include <YeloLib/configuration/c_configuration_file_factory.hpp>

namespace Yelo
{
    namespace Automation
    {
        c_test_run_output_xml::c_test_run_output_xml()
            : m_results(),
              m_current_test(nullptr) { }

        void c_test_run_output_xml::TestStarted(const std::string& name)
        {
            m_current_test = &m_results.m_tests.AddEntry();

            m_current_test->m_test_name = name;
            m_current_test->m_test_started = true;
        }

        void c_test_run_output_xml::TestMessage(const std::string& name, const TestMessageVerbosity verbosity, const std::string& message)
        {
            
        }

        void c_test_run_output_xml::TestFinished(const std::string& name, const bool passed)
        {
            m_current_test->m_test_finished = true;
            m_current_test->m_test_result = passed;

            m_current_test = nullptr;
        }

        void c_test_run_output_xml::Save(const std::string& output)
        {
            auto output_xml = Configuration::c_configuration_file_factory::CreateConfigurationFile(output);
            if (!output_xml)
            {
                throw new std::exception("Failed to create the test results");
            }
            auto root_node = output_xml->Root();
            if (!root_node)
            {
                throw new std::exception("Failed to load the test list");
            }
            m_results.SetValueToParent(*root_node);
            output_xml->Save();
        }
    };
};
#endif
