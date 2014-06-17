/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/configuration/property_tree/c_property_tree_leaf_iterator.hpp>

using namespace boost::property_tree;
using namespace std;

namespace Yelo
{
	namespace Configuration { namespace PropertyTree
	{
		c_property_tree_leaf_iterator::c_property_tree_leaf_iterator(ptree& tree, const string& node_name)
			: m_is_started(false)
			, m_tree(tree)
			, m_iterator()
			, m_node_name(node_name)
			, m_current_leaf(nullptr)
		{ }

		bool c_property_tree_leaf_iterator::MoveNext()
		{
			if (!m_is_started)
			{
				m_iterator = m_tree.find(m_node_name);
				m_is_started = true;
			}
			else
			{
				m_iterator++;
			}

			if (m_iterator != m_tree.not_found())
			{
				m_current_leaf = make_unique<c_property_tree_leaf>(m_iterator->second);
				return true;
			}
			else
			{
				m_current_leaf.reset();
				return false;
			}
		}

		shared_ptr<i_configuration_leaf> c_property_tree_leaf_iterator::Current()
		{
			return m_current_leaf;
		}
	};};
};