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
        enum class TestMessageVerbosity
        {
            Log,
            Warning,
            Error
        };

        class i_test_run_output
        {
        public:
            virtual ~i_test_run_output() {}

            virtual void TestStarted(const std::string& name) = 0;
            virtual void TestMessage(const std::string& name, const TestMessageVerbosity verbosity, const std::string& message) = 0;
            virtual void TestFinished(const std::string& name, const bool passed) = 0;
        };
    };
};
#endif