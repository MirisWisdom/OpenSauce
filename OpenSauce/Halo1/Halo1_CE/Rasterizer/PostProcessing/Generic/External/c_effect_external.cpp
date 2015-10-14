/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/Generic/External/c_effect_external.hpp"

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
                    void c_effect_external::SetEffectID(const char* id)
                    {
                        strncpy_s(m_members_external.effect_id, sizeof(m_members_external.effect_id), id, sizeof(m_members_external.effect_id));
                    }

                    const char* c_effect_external::GetEffectID()
                    {
                        return m_members_external.effect_id;
                    }

                    void c_effect_external::Ctor()
                    {
                        c_effect_generic::Ctor();

                        m_members_external.effect_id[0] = '\0';
                    }

                    void c_effect_external::Dtor()
                    {
                        c_effect_generic::Dtor();

                        m_members_external.effect_id[0] = '\0';
                    }
                }
            }
        }
    }
}
#endif
