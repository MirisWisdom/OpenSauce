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
        class i_test_run_input
        {
        public:
            virtual ~i_test_run_input() {}

            virtual std::vector<std::string> GetTests() = 0;
        };
    };
};
#endif
