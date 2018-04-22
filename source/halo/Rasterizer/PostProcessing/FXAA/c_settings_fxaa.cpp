/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include <halo/halo.h>
#include "Rasterizer/PostProcessing/FXAA/c_settings_fxaa.hpp"

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/FXAA/c_system_fxaa.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace FXAA
	{
		c_settings_container::c_settings_container()
			: Configuration::c_configuration_container("Rasterizer.PostProcessing.AntiAliasing")
			, m_enabled("Enabled", false)
		{ }

		std::vector<Configuration::i_configuration_value*> c_settings_container::GetMembers()
		{
			return std::vector<i_configuration_value*> { &m_enabled };
		}

		void c_settings_fxaa::PostLoad()
		{
			c_system_fxaa::Instance().Enabled() = Get().m_enabled;
		}

		void c_settings_fxaa::PreSave()
		{
			Get().m_enabled = c_system_fxaa::Instance().Enabled();
		}
	};};};
};
#endif