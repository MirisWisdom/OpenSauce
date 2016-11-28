/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib\configuration\i_configuration_value.hpp>

namespace Yelo
{
	namespace Configuration
	{
#include <PshPack4.h>
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	A configuration value. </summary>
		///
		/// <typeparam name="T">	The value's type. </typeparam>
		template<typename T>
		class c_configuration_value final
			: public i_configuration_value
		{
			const std::string m_node_name;
			const T m_default;
			T m_value;
			
		public:

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Initializes a new instance of the c_configuration_value class. </summary>
			///
			/// <param name="node_name">		Name of the configuration node. </param>
			/// <param name="default_value">	The node's default value. </param>
			c_configuration_value(const std::string& node_name, const T& default_value)
				: m_node_name(node_name)
				, m_default(default_value)
				, m_value(default_value)
			{ }
			
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Get's the nodes name. </summary>
			///
			/// <returns>	A std::string containing the nodes name. </returns>
			const std::string& GetName() const final override
			{
				return m_node_name;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the objects value from the supplied node. </summary>
			///
			/// <param name="node">	The node to get the value from. </param>
			void GetValue(i_configuration_leaf& node) final override
			{
				m_value = node.GetValue(m_default);
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the objects value to the supplied node. </summary>
			///
			/// <param name="node">	The node to set the value to. </param>
			void SetValue(i_configuration_leaf& node) final override
			{
				node.SetValue(m_value);
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// 	Gets the value from the first child of the supplied node. If the child does not exist, it is created.
			/// </summary>
			///
			/// <param name="parent_node">	The parent node to get the value from. </param>
			void GetValueFromParent(i_configuration_leaf& parent_node) final override
			{
				auto value_node = parent_node.GetChild(GetName());
				if(!value_node)
				{
					value_node = parent_node.AddChild(GetName());
				}

				GetValue(*value_node);
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the value to a child added to the supplied node. </summary>
			///
			/// <param name="parent_node">	The parent node to set the value to. </param>
			void SetValueToParent(i_configuration_leaf& parent_node) final override
			{
				auto value_node = parent_node.AddChild(m_node_name);

				SetValue(*value_node);
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the raw configuration value. </summary>
			///
			/// <returns>	The configuration's value as a reference. </returns>
			T& Get()
			{
				return m_value;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the raw configuration value. </summary>
			///
			/// <returns>	The configuration's value as a const reference. </returns>
			const T& GetConst() const
			{
				return m_value;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the raw configuration value. </summary>
			///
			/// <returns>	The configuration's value as a reference. </returns>
			T* GetPtr()
			{
				return &m_value;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Assignment operator. Sets the raw conifguration value to the provided value. </summary>
			///
			/// <param name="value">	The value to set. </param>
			///
			/// <returns>	A shallow copy of the configuration value. </returns>
			T& operator=(const T& value)
			{
				m_value = value;

				return *this;
			}

			/// <summary>	Support for casting the object to the templated type. </summary>
			operator T&()
			{
				return m_value;
			}
		};
#include <PopPack.h>
	};
};