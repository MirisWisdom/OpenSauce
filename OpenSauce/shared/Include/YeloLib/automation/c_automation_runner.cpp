/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/automation/c_automation_runner.hpp>

#ifdef API_DEBUG

#include <YeloLib/automation/i_automated_test.hpp>
#include <YeloLib/automation/input_output/c_test_run_logger.hpp>

namespace Yelo
{
    namespace Automation
    {
        c_automation_runner::c_automation_runner()
            : m_registered_tests() { }
        
        void c_automation_runner::RegisterTest(const std::string& name, i_automated_test* test)
        {
            m_registered_tests[name] = test;
            LOG(Log, "Test Registered: " + name);
        }

        void c_automation_runner::RunTests(c_test_run_logger& output)
        {
            for (const auto& test : m_registered_tests)
            {
                try
                {
                    output.TestStarted(test.first);
                    LOG(Log, "Test Started: " + test.first);

                    RunTest(test.first);
                    
                    LOG(Log, "Test Passed: " + test.first);
                    output.TestFinished(test.first, true);
                    continue;
                }
                catch (AssertionException& exception)
                {
                    LOG(Error, exception.GetMessageA());
                }
                catch (std::exception& exception)
                {
                    LOG(Error, exception.what());
                }
                catch (...)
                {
                    LOG(Error, "Unknown exception occurred");
                }
                output.TestFinished(test.first, false);
                LOG(Log, "Test Failed: " + test.first);
            }
        }

        void c_automation_runner::RunTest(const std::string& name)
        {
            auto& test = *m_registered_tests[name];
            test.BeforeTest();
            test.Run();
            test.AfterTest();
        }

        void c_automation_runner::Run(const std::string& test_results_file)
        {
            c_test_run_logger output;
            Logging::c_log_singleton::Get().AddLogger(&output);
            RunTests(output);
            Logging::c_log_singleton::Get().RemoveLogger(&output);

            output.Save(test_results_file);
        }
    };
};
#endif
