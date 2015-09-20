/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/automation/c_automation_runner.hpp>

#ifdef API_DEBUG

#include <map>
#include <YeloLib/automation/automation_macros.hpp>
#include <YeloLib/automation/i_automated_test.hpp>
#include <YeloLib/automation/input_output/i_test_run_input.hpp>
#include <YeloLib/automation/input_output/c_test_run_input_xml.hpp>
#include <YeloLib/automation/input_output/i_test_run_output.hpp>
#include <YeloLib/automation/input_output/c_test_list_output_xml.hpp>
#include <YeloLib/automation/input_output/c_test_run_output_xml.hpp>

namespace Yelo
{
    namespace Automation
    {
        c_automation_runner c_automation_runner::m_instance;

        c_automation_runner::c_automation_runner()
            : m_registered_tests() { }

        c_automation_runner& c_automation_runner::Get()
        {
            return m_instance;
        }

        void c_automation_runner::RegisterTest(const std::string& name, i_automated_test* test)
        {
            m_registered_tests.emplace(name, test);
        }

        void c_automation_runner::ListTests(i_test_list_output& output) const
        {            
            for(auto& test : m_registered_tests)
            {
                output.TestFound(test.first);
            }
        }

        void c_automation_runner::CreateTestList(const std::string& test_list) const
        {
            c_test_list_output_xml output;
            ListTests(output);
            output.Save(test_list);
        }

        void c_automation_runner::RunTests(i_test_run_input& input, i_test_run_output& output)
        {
            auto test_list = input.GetTests();
            while(test_list.size() > 0)
            {
                auto& name = *test_list.begin();
                test_list.erase(test_list.begin());

                output.TestStarted(name);
                auto result = RunTest(name);
                output.TestFinished(name, result);
            }
        }

        bool c_automation_runner::RunTest(const std::string& name)
        {
            return m_registered_tests[name]->Run();
        }

        void c_automation_runner::Run(const std::string& test_list_file, const std::string& test_results_file)
        {
            c_test_run_input_xml input;
            c_test_run_output_xml output;

            input.Load(test_list_file);
            RunTests(input, output);
            output.Save(test_results_file);
        }

        YELO_UNIT_TEST(UnitTest_WhenDefined_IsAddedToRunner, Yelo.Automation.Runner)
        {
            return true;
        }
    };
};
#endif
