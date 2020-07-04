/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/configuration/type_containers/c_type_container.hpp>
#include <YeloLib/configuration/c_configuration_value.hpp>

namespace Yelo
{
	namespace Configuration
	{
		/// <summary>	A real point 2D configuration container. </summary>
		class c_real_point2d_container
			: public Configuration::c_type_container<real_point2d>
		{
		private:
			Configuration::c_configuration_value<real> m_x;
			Configuration::c_configuration_value<real> m_y;

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Initializes a new instance of the c_real_point2d_container class. </summary>
			///
			/// <param name="name">			The configuration node name for the value. </param>
			/// <param name="default_x">	The default value for x. </param>
			/// <param name="default_y">	The default value for y. </param>
			c_real_point2d_container(const std::string& name, const real default_x, const real default_y);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Initializes a new instance of the c_real_point2d_container class. </summary>
			///
			/// <param name="name">	The configuration node name for the value. </param>
			c_real_point2d_container(const std::string& name);

		protected:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets a vector containing pointers to the containers members. </summary>
			///
			/// <returns>	A vector containing pointers to the containers members. </returns>
			const std::vector<i_configuration_value* const> GetMembers() final override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// 	Updates the raw value to the values that were retrieved from the configuration file.
			/// </summary>
			void PostGet();

			/// <summary>	Updates the configuration values to those in the raw value. </summary>
			void PreSet();
		};
	};
};