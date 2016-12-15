/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/configuration/property_tree/c_property_tree_leaf.hpp>

#include <YeloLib/configuration/property_tree/c_property_tree_leaf_iterator.hpp>

using namespace boost::property_tree;

namespace Yelo
{
	namespace Configuration { namespace PropertyTree
	{
		c_property_tree_leaf::c_property_tree_leaf(ptree& property_tree)
			: m_property_tree(property_tree)
		{ }
		
		bool c_property_tree_leaf::GetValue(const bool default_value)
		{
			return m_property_tree.get_value(default_value);
		}
		sbyte c_property_tree_leaf::GetValue(const sbyte default_value)
		{
			return m_property_tree.get_value(default_value);
		}
		int16 c_property_tree_leaf::GetValue(const int16 default_value)
		{
			return m_property_tree.get_value(default_value);
		}
		int32 c_property_tree_leaf::GetValue(const int32 default_value)
		{
			return m_property_tree.get_value(default_value);
		}
		byte c_property_tree_leaf::GetValue(const byte default_value)
		{
			return m_property_tree.get_value(default_value);
		}
		uint16 c_property_tree_leaf::GetValue(const uint16 default_value)
		{
			return m_property_tree.get_value(default_value);
		}
		uint32 c_property_tree_leaf::GetValue(const uint32 default_value)
		{
			return m_property_tree.get_value(default_value);
		}
		real c_property_tree_leaf::GetValue(const real default_value)
		{
			return m_property_tree.get_value(default_value);
		}
		double c_property_tree_leaf::GetValue(const double default_value)
		{
			return m_property_tree.get_value(default_value);
		}
		std::string c_property_tree_leaf::GetValue(const std::string& default_value)
		{
			return m_property_tree.get_value(default_value);
		}
			
		void c_property_tree_leaf::SetValue(const bool value)
		{
			m_property_tree.put_value(value);
		}
		void c_property_tree_leaf::SetValue(const sbyte value)
		{
			m_property_tree.put_value(value);
		}
		void c_property_tree_leaf::SetValue(const int16 value)
		{
			m_property_tree.put_value(value);
		}
		void c_property_tree_leaf::SetValue(const int32 value)
		{
			m_property_tree.put_value(value);
		}
		void c_property_tree_leaf::SetValue(const byte value)
		{
			m_property_tree.put_value(value);
		}
		void c_property_tree_leaf::SetValue(const uint16 value)
		{
			m_property_tree.put_value(value);
		}
		void c_property_tree_leaf::SetValue(const uint32 value)
		{
			m_property_tree.put_value(value);
		}
		void c_property_tree_leaf::SetValue(const real value)
		{
			m_property_tree.put_value(value);
		}
		void c_property_tree_leaf::SetValue(const double value)
		{
			m_property_tree.put_value(value);
		}
		void c_property_tree_leaf::SetValue(const std::string& value)
		{
			m_property_tree.put_value(value);
		}

		bool c_property_tree_leaf::HasChild(const std::string& child_name) const
		{
			return m_property_tree.get_child_optional(child_name) != nullptr;
		}

		std::unique_ptr<i_configuration_leaf_iterator> c_property_tree_leaf::GetChildIterator(const std::string& child_name)
		{
			if(!HasChild(child_name))
			{
				return nullptr;
			}

			// Property tree iterators only work one level deep so when a multi depth node is used, we get the first instances of the nodes at each level
			// and only iterate the nodes at the end of the first tree traversal
			std::string::size_type index = child_name.rfind('.');
			if(index == std::string::npos)
			{
				return std::make_unique<c_property_tree_leaf_iterator>(m_property_tree, child_name);
			}
			else
			{
				auto parent_node = GetChild(child_name.substr(0, index));

				return parent_node->GetChildIterator(child_name.substr(index + 1));
			}
		}

		std::unique_ptr<i_configuration_leaf> c_property_tree_leaf::GetChild(const std::string& child_name) const
		{
			if(!HasChild(child_name))
			{
				return nullptr;
			}

			ptree* current_tree = &m_property_tree;
			std::string search_string(child_name);
			
			// Split the node path into tokens and find each child node
			while(search_string.length() != 0)
			{
				auto index = search_string.find('.');

				std::string token;
				if(index == std::string::npos)
				{
					token = search_string;
					search_string = "";
				}
				else
				{
					token = search_string.substr(0, index);
					search_string.replace(0, index + 1, "");
				}

				if(current_tree->find(token) == m_property_tree.not_found())
				{
					return nullptr;
				}

				current_tree = &current_tree->get_child(token);
			};

			return std::make_unique<c_property_tree_leaf>(*current_tree);
		}

		std::unique_ptr<i_configuration_leaf> c_property_tree_leaf::AddChild(const std::string& child_name)
		{
			ptree new_child;
			return std::make_unique<c_property_tree_leaf>(m_property_tree.add_child(child_name, new_child));
		}
	};};
};