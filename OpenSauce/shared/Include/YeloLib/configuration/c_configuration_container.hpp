/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/configuration/i_configuration_container.hpp>

namespace Yelo
{
	namespace Configuration
	{
		/// <summary>	A container for a configuration node with multiple child values in it. </summary>
		class c_configuration_container
			abstract
			: public i_configuration_container
		{
		private:
			const std::string m_node_name;
			
		public:
			c_configuration_container(const std::string& node_name);
			
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Get's the nodes name. </summary>
			///
			/// <returns>	A std::string containing the nodes name. </returns>
			const std::string& GetName() const final override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the containers values from the supplied node. </summary>
			///
			/// <param name="node">	The node to get the values from. </param>
			void GetValue(i_configuration_leaf& node) final override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the objects values to the supplied node. </summary>
			///
			/// <param name="node">	The node to set the values to. </param>
			void SetValue(i_configuration_leaf& node) final override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// 	Gets the values from the first child of the supplied node. If the child does not exist, it is created.
			/// </summary>
			///
			/// <param name="parent_node">	The parent node to get the values from. </param>
			void GetValueFromParent(i_configuration_leaf& parent_node) final override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the values to a child added to the supplied node. </summary>
			///
			/// <param name="parent_node">	The parent node to set the values to. </param>
			void SetValueToParent(i_configuration_leaf& parent_node) final override;

		protected:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets a vector containing pointers to the containers members. </summary>
			///
			/// <returns>	A vector containing pointers to the containers members. </returns>
			virtual const std::vector<i_configuration_value* const> GetMembers() = 0;

			/// <summary>	Used in derived containers for logic that is run before getting a containers values. </summary>
			virtual void PreGet() {}

			/// <summary>	Used in derived containers for logic that is run after getting a containers values. </summary>
			virtual void PostGet() {}

			/// <summary>	Used in derived containers for logic that is run before setting a containers values. </summary>
			virtual void PreSet() {}

			/// <summary>	Used in derived containers for logic that is run after setting a containers values. </summary>
			virtual void PostSet() {}
		};
	};
};