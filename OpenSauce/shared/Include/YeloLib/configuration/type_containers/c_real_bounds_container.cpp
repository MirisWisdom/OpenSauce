/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/configuration/type_containers/c_real_bounds_container.hpp>

namespace Yelo
{
	namespace Configuration
	{
		c_real_bounds_container::c_real_bounds_container(const std::string& name, real default_lower, real default_upper)
			: Configuration::c_type_container<real_bounds>(name)
			, m_lower("Lower", default_lower)
			, m_upper("Upper", default_upper)
		{
			m_value.lower = default_lower;
			m_value.upper = default_upper;
		}

		c_real_bounds_container::c_real_bounds_container(const std::string& name)
			: c_real_bounds_container(name, 0.0f, 0.0f)
		{ }

		const std::vector<i_configuration_value* const> c_real_bounds_container::GetMembers()
		{
			return std::vector<i_configuration_value* const>
			{
				&m_lower,
				&m_upper
			};
		}

		void c_real_bounds_container::PostGet()
		{
			m_value.lower = m_lower;
			m_value.upper = m_upper;
		}

		void c_real_bounds_container::PreSet()
		{
			m_lower = m_value.lower;
			m_upper = m_value.upper;
		}
	};
};