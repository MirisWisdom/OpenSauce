/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#ifdef API_DEBUG

#include <YeloLib/automation/input_output/c_test_results_container.hpp>
#include <YeloLib/logging/c_logger_base.hpp>

namespace Yelo
{
    namespace Automation
    {
        class c_test_run_logger final : public Logging::c_logger_base
        {
            c_test_results_container m_results;
            c_test_result* m_current_test;

        public:
            explicit c_test_run_logger();

            const c_test_result* GetCurrentTest() const;
            void TestStarted(const std::string& name);
            void TestFinished(const std::string& name, const bool passed);
            void Save(const std::string& output);

        private:
            void Write(const std::string& message) override;
        };
    };
};
#endif
