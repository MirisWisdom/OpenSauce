/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#ifdef API_DEBUG

#include <YeloLib/cseries/c_singleton.hpp>

namespace Yelo
{
    namespace Automation
    {
        class i_automated_test;
        class c_test_run_output;

        class c_automation_runner final
        {
            std::map<std::string, i_automated_test*> m_registered_tests;

        public:
            c_automation_runner();

            void RegisterTest(const std::string& name, i_automated_test* test);
            void Run(const std::string& test_results_file);
            void RunTest(const std::string& name);

        private:
            void RunTests(c_test_run_output& output);
        };

        class c_automation_runner_singleton : public c_singleton<c_automation_runner> {};
    };
};
#endif
