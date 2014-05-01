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
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Initializes a new instance of the c_real_vector3d_container class. </summary>
		///
		/// <param name="name">			The configuration node name for the value. </param>
		/// <param name="default_i">	The default value for i. </param>
		/// <param name="default_j">	The default value for j. </param>
		/// <param name="default_k">	The default value for k. </param>
		c_real_vector3d_container::c_real_vector3d_container(std::string name, real default_i, real default_j, real default_k)
			: Configuration::c_type_container<real_vector3d>(name)
			, m_i("I", default_i)
			, m_j("J", default_j)
			, m_k("K", default_k)
		{
			m_value.i = default_i;
			m_value.j = default_j;
			m_value.k = default_k;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Initializes a new instance of the c_real_vector3d_container class. </summary>
		///
		/// <param name="name">	The configuration node name for the value. </param>
		c_real_vector3d_container::c_real_vector3d_container(const std::string name)
			: c_real_vector3d_container(name, 0.0f, 0.0f, 0.0f)
		{ }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a vector containing pointers to the containers members. </summary>
		///
		/// <returns>	A vector containing pointers to the containers members. </returns>
		const std::vector<i_configuration_value* const> c_real_vector3d_container::GetMembers()
		{
			std::vector<i_configuration_value* const> values =
			{
				&m_i,
				&m_j,
				&m_k
			};

			return values;
		}

		/// <summary>	Updates the raw value to the values that were retrieved form the configuration file. </summary>
		void c_real_vector3d_container::PostGet()
		{
			m_value.i = m_i;
			m_value.j = m_j;
			m_value.k = m_k;
		}

		/// <summary>	Updates the configuration values to those in the raw value. </summary>
		void c_real_vector3d_container::PreSet()
		{
			m_i = m_value.i;
			m_j = m_value.j;
			m_k = m_value.k;
		}
	}
}