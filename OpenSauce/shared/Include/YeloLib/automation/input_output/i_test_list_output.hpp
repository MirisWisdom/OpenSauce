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
        class i_test_list_output
        {
        public:
            virtual ~i_test_list_output() {}

            virtual void TestFound(const std::string& name) = 0;
        };
    };
};
#endif