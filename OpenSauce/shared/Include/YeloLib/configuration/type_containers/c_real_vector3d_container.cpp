/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/configuration/type_containers/c_real_vector3d_container.hpp>

namespace Yelo
{
	namespace Configuration
	{
		c_real_vector3d_container::c_real_vector3d_container(const std::string& name, real default_i, real default_j, real default_k)
			: Configuration::c_type_container<real_vector3d>(name)
			, m_i("I", default_i)
			, m_j("J", default_j)
			, m_k("K", default_k)
		{
			m_value.i = default_i;
			m_value.j = default_j;
			m_value.k = default_k;
		}

		c_real_vector3d_container::c_real_vector3d_container(const std::string& name)
			: c_real_vector3d_container(name, 0.0f, 0.0f, 0.0f)
		{ }

		const std::vector<i_configuration_value* const> c_real_vector3d_container::GetMembers()
		{
			return std::vector<i_configuration_value* const>
			{
				&m_i,
				&m_j,
				&m_k
			};
		}

		void c_real_vector3d_container::PostGet()
		{
			m_value.i = m_i;
			m_value.j = m_j;
			m_value.k = m_k;
		}

		void c_real_vector3d_container::PreSet()
		{
			m_i = m_value.i;
			m_j = m_value.j;
			m_k = m_value.k;
		}
	};
};