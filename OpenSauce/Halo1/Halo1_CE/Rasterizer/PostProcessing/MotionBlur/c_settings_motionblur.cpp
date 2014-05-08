/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/MotionBlur/c_settings_motionblur.hpp"

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/MotionBlur/c_system_motionblur.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace MotionBlur
	{
		c_settings_container::c_settings_container()
			: Configuration::c_configuration_container("Rasterizer.PostProcessing.MotionBlur")
			, m_enabled("Enabled", true)
			, m_blur_amount("BlurAmount", 1.0f)
		{ }

		const std::vector<Configuration::i_configuration_value* const> c_settings_container::GetMembers()
		{
			return std::vector<Configuration::i_configuration_value* const>
			{
				&m_enabled,
				&m_blur_amount
			};
		}

		void c_settings_motionblur::Register()
		{
			Settings::RegisterConfigurationContainer(GetPtr(), nullptr, &PostLoad, &PreSave);
		}

		void c_settings_motionblur::Unregister()
		{
			Settings::UnregisterConfigurationContainer(GetPtr());
		}

		void c_settings_motionblur::PostLoad()
		{
			c_system_motionblur::Instance().Enabled() = Instance().Get().m_enabled;
			c_system_motionblur::Instance().BlurAmount() = Instance().Get().m_blur_amount;
		}

		void c_settings_motionblur::PreSave()
		{
			Instance().Get().m_enabled = c_system_motionblur::Instance().Enabled();
			Instance().Get().m_blur_amount = c_system_motionblur::Instance().BlurAmount();
		}
	};};};
};
#endif