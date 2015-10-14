/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Rasterizer/PostProcessing/Generic/c_effect_generic.hpp"

namespace Yelo
{
    namespace Rasterizer
    {
        namespace PostProcessing
        {
            namespace Generic
            {
                namespace External
                {
                    class c_effect_external : public c_effect_generic
                    {
                    protected:
                        struct
                        {
                            tag_string effect_id;
                        } m_members_external;

                    public:
                        void SetEffectID(const char* id);
                        const char* GetEffectID();

                        void Ctor() override;
                        void Dtor() override;
                    };
                }
            }
        }
    }
}
#endif
