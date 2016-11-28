/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/cseries/profile.hpp>

namespace Yelo
{
	namespace Debug
	{
		c_profile_section_scope::c_profile_section_scope(s_profile_section& section)
			: m_section(section)
		{
			if (blam::profiling_enabled() && m_section.active)
			{
				blam::profile_enter_private(m_section);
			}
		}

		c_profile_section_scope::~c_profile_section_scope()
		{
			if (blam::profiling_enabled() && m_section.active)
			{
				blam::profile_exit_private(m_section);
			}
		}
	};
};