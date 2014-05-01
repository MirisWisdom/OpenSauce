/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/configuration/property_tree/c_property_tree_leaf_iterator.hpp>

namespace Yelo
{
	namespace Configuration { namespace PropertyTree
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Initializes a new instance of the c_property_tree_leaf_iterator class. </summary>
		///
		/// <param name="tree">			The tree to iterate over. </param>
		/// <param name="node_name">	Name of the nodes to find. </param>
		c_property_tree_leaf_iterator::c_property_tree_leaf_iterator(boost::property_tree::ptree* tree, const std::string node_name)
			: m_is_started(false)
			, m_tree(*tree)
			, m_iterator()
			, m_node_name(node_name)
			, m_current_leaf(nullptr)
		{ }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Determines if we can move to the next entry. </summary>
		///
		/// <returns>	true if another entry is available, false if the end of the list was reached. </returns>
		bool c_property_tree_leaf_iterator::MoveNext()
		{
			if(m_is_started && (m_iterator == m_tree.end()))
			{
				m_current_leaf.reset();
				return false;
			}

			if(!m_is_started)
			{
				m_iterator = m_tree.begin();
				m_is_started = true;
			}
			else
			{
				++m_iterator;
			}

			while(m_iterator != m_tree.end())
			{
				if(m_iterator->first == m_node_name)
				{
					m_current_leaf.reset(new c_property_tree_leaf(&m_iterator->second));
					return true;
				}

				++m_iterator;
			}
			
			m_current_leaf.reset();
			return false;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the current entry. </summary>
		///
		/// <returns>	A i_configuration_leaf representing the current tree child. </returns>
		std::shared_ptr<i_configuration_leaf> c_property_tree_leaf_iterator::Current()
		{
			return m_current_leaf;
		}
	}}
}