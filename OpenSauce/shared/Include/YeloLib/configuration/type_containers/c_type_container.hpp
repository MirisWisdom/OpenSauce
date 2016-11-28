/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/configuration/c_configuration_container.hpp>
#include <YeloLib/configuration/c_configuration_value.hpp>

namespace Yelo
{
	namespace Configuration
	{
#include <PshPack4.h>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	A configuration container wrapper for complex/custom value types. </summary>
		///
		/// <typeparam name="Type">	Type of the value being wrapped. </typeparam>
		template<typename Type>
		class c_type_container
			abstract
			: public Configuration::c_configuration_container
		{
		protected:
			Type m_value;

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Constructor. </summary>
			///
			/// <param name="name">	The name of the node. </param>
			c_type_container(const std::string& name)
				: Configuration::c_configuration_container(name)
			{ }

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the value as a reference. </summary>
			///
			/// <returns>	The wrapped value. </returns>
			Type& Get()
			{
				return m_value;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the value as a const reference. </summary>
			///
			/// <returns>	The wrapped value. </returns>
			const Type& GetConst() const
			{
				return m_value;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// 	Assignment operator. Sets the raw configuration value to the provided value.
			/// </summary>
			///
			/// <param name="value">	The value to set. </param>
			///
			/// <returns>	A shallow copy of the configuration value. </returns>
			Type& operator=(const Type& value)
			{
				m_value = value;

				return *this;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Support for casting the object to the templated type. </summary>
			///
			/// <returns>	The result of the operation. </returns>
			operator Type&()
			{
				return m_value;
			}
		};
#include <PopPack.h>
	};
};