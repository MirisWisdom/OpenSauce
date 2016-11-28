/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/configuration/type_containers/c_point2d_container.hpp>

namespace Yelo
{
	namespace Configuration
	{
		c_point2d_container::c_point2d_container(const std::string& name, int16 default_x, int16 default_y)
			: Configuration::c_type_container<point2d>(name)
			, m_x("X", default_x)
			, m_y("Y", default_y)
		{
			m_value.x = default_x;
			m_value.y = default_y;
		}

		c_point2d_container::c_point2d_container(const std::string& name)
			: c_point2d_container(name, 0, 0)
		{ }

		const std::vector<i_configuration_value* const> c_point2d_container::GetMembers()
		{
			return std::vector<i_configuration_value* const>
			{
				&m_x,
				&m_y
			};
		}

		void c_point2d_container::PostGet()
		{
			m_value.x = m_x;
			m_value.y = m_y;
		}

		void c_point2d_container::PreSet()
		{
			m_x = m_value.x;
			m_y = m_value.y;
		}
	};
};