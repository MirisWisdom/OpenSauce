/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#ifdef API_DEBUG

#include <YeloLib/automation/input_output/c_test_results_container.hpp>

namespace Yelo
{
    namespace Automation
    {
        enum class TestMessageVerbosity
        {
            Log,
            Warning,
            Error
        };

        class c_test_run_output final
        {
            c_test_results_container m_results;
            c_test_result* m_current_test;

        public:
            explicit c_test_run_output();

            const c_test_result* GetCurrentTest() const;
            void TestStarted(const std::string& name);
            void TestMessage(const TestMessageVerbosity verbosity, const std::string& message);
            void TestFinished(const std::string& name, const bool passed);
            void Save(const std::string& output);
        };
    };
};
#endif
