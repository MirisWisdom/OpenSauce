/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
    template<typename TType>
    class c_singleton
    {
    public:
        static TType& Get()
        {
            static TType instance;
            return instance;
        }
    };
};