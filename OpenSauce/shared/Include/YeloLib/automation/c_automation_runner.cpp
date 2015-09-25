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
#include <YeloLib/automation/input_output/i_test_run_output.hpp>
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

        void c_automation_runner::RunTests(i_test_run_output& output)
        {
            for(const auto& test : m_registered_tests)
            {
                try
                {
                    output.TestStarted(test.first);
                    auto result = RunTest(test.first);
                    output.TestFinished(test.first, true);
                }
                catch(std::exception& exception)
                {
                    output.TestMessage(test.first, TestMessageVerbosity::Error, exception.what());
                    output.TestFinished(test.first, false);
                }
            }
        }

        bool c_automation_runner::RunTest(const std::string& name)
        {
            return m_registered_tests[name]->Run();
        }

        void c_automation_runner::Run(const std::string& test_results_file)
        {
            c_test_run_output_xml output;

            RunTests(output);
            output.Save(test_results_file);
        }

        YELO_UNIT_TEST(UnitTest_WhenDefined_IsAddedToRunner, Yelo.Automation.Runner)
        {
            return true;
        }
    };
};
#endif
