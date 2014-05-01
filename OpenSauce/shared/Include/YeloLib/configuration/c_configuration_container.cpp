/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/configuration/c_configuration_container.hpp>

namespace Yelo
{
	namespace Configuration
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Initializes a new instance of the c_configuration_container class. </summary>
		///
		/// <param name="node_name">	Full pathname of the source node. </param>
		c_configuration_container::c_configuration_container(const std::string node_name)
			: m_node_name(node_name)
		{ }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get's the nodes name. </summary>
		///
		/// <returns>	A std::string containing the nodes name. </returns>
		const std::string c_configuration_container::GetName() const
		{
			return m_node_name;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the containers values from the provided node. </summary>
		///
		/// <param name="node">	The container node. </param>
		void c_configuration_container::GetValue(i_configuration_leaf& node)
		{
			PreGet();

			for(auto value : GetMembers())
			{
				value->GetValueFromParent(node);
			}

			PostGet();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the containers values to the provided node. </summary>
		///
		/// <param name="node">	The container node. </param>
		void c_configuration_container::SetValue(i_configuration_leaf& node)
		{
			PreSet();

			for(auto value : GetMembers())
			{
				value->SetValueToParent(node);
			}

			PostSet();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the container node from the parent, then gets its values. </summary>
		///
		/// <param name="parent_node">	The parent node. </param>
		void c_configuration_container::GetValueFromParent(i_configuration_leaf& parent_node)
		{
			auto container_node = parent_node.GetChild(GetName());
			if(!container_node)
			{
				container_node = parent_node.AddChild(GetName());
			}

			GetValue(*container_node);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds the container node to the parent, then sets its values. </summary>
		///
		/// <param name="parent_node">	The parent node. </param>
		void c_configuration_container::SetValueToParent(i_configuration_leaf& parent_node)
		{
			auto container_node = parent_node.GetChild(GetName());
			if(!container_node)
			{
				container_node = parent_node.AddChild(GetName());
			}

			SetValue(*container_node);
		}
	}
}