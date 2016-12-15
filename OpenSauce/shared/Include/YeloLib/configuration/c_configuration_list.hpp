/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib\configuration\i_configuration_leaf.hpp>
#include <YeloLib\configuration\i_configuration_value.hpp>
#include <YeloLib\configuration\i_configuration_container.hpp>

namespace Yelo
{
	namespace Configuration
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Abstract list object for a list of configuration values. </summary>
		///
		/// <typeparam name="T">	The configuration value type. </typeparam>
		template<typename T>
		class c_configuration_list
			abstract
			: public i_configuration_value
		{
			//static_assert(std::is_convertible<T, i_configuration_value>::value, "c_configuration_list can only be used with types that derive from i_configuration_value");

			const std::string m_node_name;
			std::vector<T> m_values;
			const std::function<T()> m_create_new;
			
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Initializes a new instance of the c_configuration_list class. </summary>
			///
			/// <param name="node_name"> 	Name of the node. </param>
			/// <param name="create_new">	The function use when creating a new instance of T. </param>
			c_configuration_list(const std::string& node_name, const std::function<T()>& create_new)
				: m_node_name(node_name)
				, m_values()
				, m_create_new(create_new)
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
			/// <summary>	Unused in lists. Will throw if used. </summary>
			///
			/// <param name="node"/>
			void GetValue(i_configuration_leaf& node) final override
			{
				throw;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Unused in lists. Will throw if used. </summary>
			///
			/// <param name="node"/>
			void SetValue(i_configuration_leaf& node) final override
			{
				throw;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the list of configuration values from the provided parent leaf. </summary>
			///
			/// <param name="node">	The leaf to get the values from. </param>
			void GetValueFromParent(i_configuration_leaf& node) final override
			{
				auto& iterator = node.GetChildIterator(m_node_name);

				if(!iterator)
				{
					return;
				}

				while(iterator->MoveNext())
				{
					auto& value = AddEntry();

					value.GetValue(*(iterator->Current()));
				}
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Adds the lists values as children of the provided leaf. </summary>
			///
			/// <param name="node">	The leaf node to add children to. </param>
			void SetValueToParent(i_configuration_leaf& node) final override
			{
				for(auto& value : m_values)
				{
					auto child_node = node.AddChild(m_node_name);

					value.SetValue(*child_node);
				}
			}

			/// <summary>	Clears the list. </summary>
			void Clear()
			{
				m_values.clear();
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Adds an entry to the list. </summary>
			///
			/// <returns>	A reference to the object added to the list. </returns>
			auto AddEntry() -> decltype(m_values.back())
			{
				m_values.push_back(m_create_new());

				return m_values.back();
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets an iterator at the start of the list. </summary>
			///
			/// <returns>	An iterator. </returns>
			auto begin() -> decltype(m_values.begin())
			{
				return m_values.begin();
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets an iterator at the end of the list. </summary>
			///
			/// <returns>	An iterator. </returns>
			auto end() -> decltype(m_values.end())
			{
				return m_values.end();
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the size of the list. </summary>
			///
			/// <returns>	The size of the list. </returns>
			auto size() const -> decltype(m_values.size()) 
			{
				return m_values.size();
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the list as a reference. </summary>
			///
			/// <returns>	A std::vector<T> reference. </returns>
			std::vector<T>& Get()
			{
				return m_values;
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the list as a const reference. </summary>
			///
			/// <returns>	A const std::vector<T> reference. </returns>
			const std::vector<T>& GetConst() const
			{
				return m_values;
			}
		};
	};
};