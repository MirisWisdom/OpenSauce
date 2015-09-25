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
        class i_test_run_output;

        class c_automation_runner final
        {
            static c_automation_runner m_instance;
        public:
            static c_automation_runner& Get();

        private:
            std::map<const std::string, i_automated_test*> m_registered_tests;

            c_automation_runner();

            void RunTests(i_test_run_output& output);

        public:
            void RegisterTest(const std::string& name, i_automated_test* test);
            void Run(const std::string& test_results_file);
            bool RunTest(const std::string& name);
        };
    };
};
#endif
