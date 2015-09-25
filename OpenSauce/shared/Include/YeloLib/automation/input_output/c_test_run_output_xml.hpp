/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#ifdef API_DEBUG

#include <YeloLib/automation/input_output/i_test_run_output.hpp>
#include <YeloLib/automation/input_output/c_test_results_xml.hpp>

namespace Yelo
{
    namespace Automation
    {
        class c_test_run_output_xml final : public i_test_run_output
        {
            c_test_results_xml m_results;
            c_test_result* m_current_test;

        public:
            explicit c_test_run_output_xml();

            void TestStarted(const std::string& name) override;
            void TestMessage(const std::string& name, const TestMessageVerbosity verbosity, const std::string& message) override;
            void TestFinished(const std::string& name, const bool passed) override;
            void Save(const std::string& output);
        };
    };
};
#endif
