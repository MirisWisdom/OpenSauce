/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#ifdef API_DEBUG

namespace Yelo
{
    namespace Automation
    {
        class i_automated_test;
        class c_test_run_output;

        class c_automation_runner final
        {
        public:
            static c_automation_runner& Get();

        private:
            std::map<std::string, i_automated_test*> m_registered_tests;

            c_automation_runner();

            void RunTests(c_test_run_output& output);

        public:
            void RegisterTest(const std::string& name, i_automated_test* test);
            void Run(const std::string& test_results_file);
            void RunTest(const std::string& name);
        };
    };
};
#endif
