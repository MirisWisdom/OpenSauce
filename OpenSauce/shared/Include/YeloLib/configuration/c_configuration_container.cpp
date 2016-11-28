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
		c_configuration_container::c_configuration_container(const std::string& node_name)
			: m_node_name(node_name)
		{ }

		const std::string& c_configuration_container::GetName() const
		{
			return m_node_name;
		}

		void c_configuration_container::GetValue(i_configuration_leaf& node)
		{
			PreGet();

			for(auto value : GetMembers())
			{
				value->GetValueFromParent(node);
			}

			PostGet();
		}

		void c_configuration_container::SetValue(i_configuration_leaf& node)
		{
			PreSet();

			for(auto value : GetMembers())
			{
				value->SetValueToParent(node);
			}

			PostSet();
		}

		void c_configuration_container::GetValueFromParent(i_configuration_leaf& parent_node)
		{
			auto container_node = parent_node.GetChild(GetName());
			if(!container_node)
			{
				container_node = parent_node.AddChild(GetName());
			}

			GetValue(*container_node);
		}

		void c_configuration_container::SetValueToParent(i_configuration_leaf& parent_node)
		{
			auto container_node = parent_node.GetChild(GetName());
			if(!container_node)
			{
				container_node = parent_node.AddChild(GetName());
			}

			SetValue(*container_node);
		}
	};
};