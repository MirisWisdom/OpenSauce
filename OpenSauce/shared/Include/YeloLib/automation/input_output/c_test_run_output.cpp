/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/automation/input_output/c_test_run_output.hpp>

#ifdef API_DEBUG

#include <YeloLib/automation/automation_macros.hpp>
#include <YeloLib/configuration/c_configuration_file_factory.hpp>

namespace Yelo
{
    namespace Automation
    {
        c_test_run_output::c_test_run_output()
            : m_results(),
              m_current_test(nullptr) { }

        const c_test_result* c_test_run_output::GetCurrentTest() const
        {
            return m_current_test;
        }

        void c_test_run_output::TestStarted(const std::string& name)
        {
            if (name.empty())
            {
                throw std::exception("Start test called with an empty test name");
            }
            if (find_if(m_results.m_tests.begin(), m_results.m_tests.end(),
                        [name](const c_test_result& entry)
                        {
                            return entry.m_name.GetConst() == name;
                        }) != m_results.m_tests.end())
            {
                throw std::exception("Start test called with a test that has already been run");
            }
            if (m_current_test)
            {
                throw std::exception("Start test called with a test already in progress");
            }
            m_current_test = &m_results.m_tests.AddEntry();

            m_current_test->m_name = name;
            m_current_test->m_started = true;
        }

        void c_test_run_output::TestLog(const TestMessageVerbosity verbosity, const std::string& message)
        {
            if (!m_current_test)
            {
                throw std::exception("Test message called when no test is in progress");
            }
            auto verbosity_prefix = "";
            switch (verbosity)
            {
                case TestMessageVerbosity::Warning:
                    verbosity_prefix = "Warning: ";
                    break;
                case TestMessageVerbosity::Error:
                    verbosity_prefix = "Error: ";
                    break;
                default:
                    break;
            }
            auto testMessage = verbosity_prefix + message;
            m_current_test->m_messages.AddEntry().Get() = testMessage;
        }

        void c_test_run_output::TestFinished(const std::string& name, const bool passed)
        {
            if (name.empty())
            {
                throw std::exception("Finish test called with an empty test name");
            }
            if (!m_current_test)
            {
                throw std::exception("Finish test called when no test is in progress");
            }
            if (m_current_test->m_name.GetConst() != name)
            {
                throw std::exception("Attempted to finish a test that was not in progress");
            }
            m_current_test->m_result = passed;

            m_current_test = nullptr;
        }

        void c_test_run_output::Save(const std::string& output)
        {
            auto output_file = Configuration::c_configuration_file_factory::CreateConfigurationFile(output);
            if (!output_file)
            {
                throw std::exception("Failed to create the test results");
            }
            auto root_node = output_file->Root();
            if (!root_node)
            {
                throw std::exception("Failed to save the test list");
            }
            m_results.SetValueToParent(*root_node);
            output_file->Save();
        }

#define YELO_UNIT_TEST_TEST_RUN_OUTPUT(name) YELO_UNIT_TEST(name, Automation.TestRunOutput)

        static const char* g_any_test_name = "anyTestName";

        YELO_UNIT_TEST_TEST_RUN_OUTPUT(GetCurrentTest_BeforeTestStarted_ReturnsNull)
        {
            c_test_run_output output;

            auto result = output.GetCurrentTest();

            AssertThat(result, IsNull());
        }

        YELO_UNIT_TEST_TEST_RUN_OUTPUT(GetCurrentTest_AfterTestStarted_ReturnsTest)
        {
            c_test_run_output output;

            output.TestStarted(g_any_test_name);
            auto result = output.GetCurrentTest();

            AssertThat(result, Is().Not().Null());
            AssertThat(result->m_name.GetConst(), Equals(g_any_test_name));
        }

        YELO_UNIT_TEST_TEST_RUN_OUTPUT(GetCurrentTest_AfterTestFinished_ReturnsNull)
        {
            c_test_run_output output;

            output.TestStarted(g_any_test_name);
            output.TestFinished(g_any_test_name, true);
            auto result = output.GetCurrentTest();

            AssertThat(result, Is().Null());
        }

        YELO_UNIT_TEST_TEST_RUN_OUTPUT(TestStarted_WithEmptyTestName_Throws)
        {
            c_test_run_output output;

            AssertThrows(std::exception, output.TestStarted(""));
        }

        YELO_UNIT_TEST_TEST_RUN_OUTPUT(TestStarted_WithTestInProcess_Throws)
        {
            c_test_run_output output;

            output.TestStarted(g_any_test_name);

            AssertThrows(std::exception, output.TestStarted(g_any_test_name));
        }

        YELO_UNIT_TEST_TEST_RUN_OUTPUT(TestStarted_WithAlreadyCompletedTest_Throws)
        {
            c_test_run_output output;

            output.TestStarted(g_any_test_name);
            output.TestFinished(g_any_test_name, false);

            AssertThrows(std::exception, output.TestStarted(g_any_test_name));
        }

        YELO_UNIT_TEST_TEST_RUN_OUTPUT(TestStarted_WithTestName_StartsTest)
        {
            c_test_run_output output;

            output.TestStarted(g_any_test_name);

            AssertThat(output.GetCurrentTest()->m_started.GetConst(), Is().True());
        }

        YELO_UNIT_TEST_TEST_RUN_OUTPUT(TestMessage_WithNoTestInProgress_Throws)
        {
            c_test_run_output output;

            AssertThrows(std::exception, output.TestLog(TestMessageVerbosity::Log, "anyMessage"));
        }

        YELO_UNIT_TEST_TEST_RUN_OUTPUT(TestMessage_WithTestInProgress_AddsMessageToTest)
        {
            c_test_run_output output;

            output.TestStarted(g_any_test_name);
            output.TestLog(TestMessageVerbosity::Log, "anyMessage");
            auto result = output.GetCurrentTest()->m_messages.GetConst()[0];

            AssertThat(result.GetConst(), Equals("anyMessage"));
        }

        YELO_UNIT_TEST_TEST_RUN_OUTPUT(TestMessage_WithWarningMessage_AddsWarningPrefix)
        {
            c_test_run_output output;

            output.TestStarted(g_any_test_name);
            output.TestLog(TestMessageVerbosity::Warning, "anyMessage");
            auto result = output.GetCurrentTest()->m_messages.GetConst()[0];

            AssertThat(result.GetConst(), Equals("Warning: anyMessage"));
        }

        YELO_UNIT_TEST_TEST_RUN_OUTPUT(TestMessage_WithErrorMessage_AddsErrorPrefix)
        {
            c_test_run_output output;

            output.TestStarted(g_any_test_name);
            output.TestLog(TestMessageVerbosity::Error, "anyMessage");
            auto result = output.GetCurrentTest()->m_messages.GetConst()[0];

            AssertThat(result.GetConst(), Equals("Error: anyMessage"));
        }

        YELO_UNIT_TEST_TEST_RUN_OUTPUT(TestFinished_BeforeTestStarted_Throws)
        {
            c_test_run_output output;

            AssertThrows(std::exception, output.TestFinished(g_any_test_name, false));
        }

        YELO_UNIT_TEST_TEST_RUN_OUTPUT(TestFinished_WithEmptyTestName_Throws)
        {
            c_test_run_output output;

            AssertThrows(std::exception, output.TestFinished("", false));
        }

        YELO_UNIT_TEST_TEST_RUN_OUTPUT(TestFinished_WithDifferentTestName_Throws)
        {
            c_test_run_output output;
            output.TestStarted(g_any_test_name);

            AssertThrows(std::exception, output.TestFinished("anyOtherTestName", false));
        }

        YELO_UNIT_TEST_TEST_RUN_OUTPUT(TestFinished_WithCurrentTestName_FinishesTestWithResult)
        {
            c_test_run_output output;
            output.TestStarted(g_any_test_name);

            auto in_progress_test = output.GetCurrentTest();
            output.TestFinished(g_any_test_name, true);

            AssertThat(in_progress_test->m_result.GetConst(), Is().True());
        }

        YELO_UNIT_TEST_TEST_RUN_OUTPUT(Save_WithUnknownExtension_Throws)
        {
            c_test_run_output output;

            AssertThrows(std::exception, output.Save("anyPath.unknownExtension"));
        }

#undef YELO_UNIT_TEST_TEST_RUN_OUTPUT
    };
};
#endif
