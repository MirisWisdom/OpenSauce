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
#include <YeloLib/automation/input_output/c_test_run_output.hpp>

namespace Yelo
{
    namespace Automation
    {
        c_automation_runner::c_automation_runner()
            : m_registered_tests() { }

        c_automation_runner& c_automation_runner::Get()
        {
            static c_automation_runner instance;
            return instance;
        }

        void c_automation_runner::RegisterTest(const std::string& name, i_automated_test* test)
        {
            m_registered_tests[name] = test;
        }

        void c_automation_runner::RunTests(c_test_run_output& output)
        {
            for (const auto& test : m_registered_tests)
            {
                try
                {
                    output.TestStarted(test.first);
                    RunTest(test.first);
                    output.TestFinished(test.first, true);
                }
                catch (AssertionException& exception)
                {
                    output.TestMessage(TestMessageVerbosity::Error, exception.GetMessageA());
                    output.TestFinished(test.first, false);
                }
                catch (std::exception& exception)
                {
                    output.TestMessage(TestMessageVerbosity::Error, exception.what());
                    output.TestFinished(test.first, false);
                }
                catch (...)
                {
                    output.TestMessage(TestMessageVerbosity::Error, "Unknown exception occurred");
                    output.TestFinished(test.first, false);
                }
            }
        }

        void c_automation_runner::RunTest(const std::string& name)
        {
            m_registered_tests[name]->Run();
        }

        void c_automation_runner::Run(const std::string& test_results_file)
        {
            c_test_run_output output;

            RunTests(output);
            output.Save(test_results_file);
        }
    };
};
#endif
