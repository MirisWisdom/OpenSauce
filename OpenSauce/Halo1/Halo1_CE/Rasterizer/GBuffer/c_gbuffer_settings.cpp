/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/GBuffer/c_gbuffer_settings.hpp"

#if !PLATFORM_IS_DEDI

#include "Rasterizer/GBuffer.hpp"

namespace Yelo
{
    namespace Rasterizer
    {
        namespace GBuffer
        {
            c_settings_container::c_settings_container()
                : c_configuration_container("Rasterizer.GBuffer"),
                  m_enabled("Enabled", true) { }

            const std::vector<Configuration::i_configuration_value* const> c_settings_container::GetMembers()
            {
                return std::vector<i_configuration_value* const> { &m_enabled };
            }

            void c_settings_gbuffer::PostLoad()
            {
                Enabled() = Get().m_enabled;
            }

            void c_settings_gbuffer::PreSave()
            {
                Get().m_enabled = Enabled();
            }
        };
    };
};
#endif
