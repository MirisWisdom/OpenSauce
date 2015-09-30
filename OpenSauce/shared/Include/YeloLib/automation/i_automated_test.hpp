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
        class i_automated_test
        {
        public:
            virtual ~i_automated_test() {}

            virtual void Run() = 0;
        };
    };
};
#endif
