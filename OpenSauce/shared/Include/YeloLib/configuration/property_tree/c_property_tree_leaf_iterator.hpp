/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/configuration/i_configuration_leaf_iterator.hpp>
#include <YeloLib/configuration/property_tree/c_property_tree_leaf.hpp>
#include <YeloLib/configuration/property_tree/c_property_tree_file.hpp>

namespace Yelo
{
	namespace Configuration { namespace PropertyTree
	{
		/// <summary>	A property tree leaf iterator. </summary>
		class c_property_tree_leaf_iterator
			: public i_configuration_leaf_iterator
		{
			bool m_is_started; PAD24;
			boost::property_tree::ptree& m_tree;
			boost::property_tree::ptree::assoc_iterator m_iterator;
			const std::string m_node_name;

			std::shared_ptr<c_property_tree_leaf> m_current_leaf;

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Initializes a new instance of the c_property_tree_leaf_iterator class. </summary>
			///
			/// <param name="tree">			The tree to iterate over. </param>
			/// <param name="node_name">	Name of the nodes to find. </param>
			c_property_tree_leaf_iterator(boost::property_tree::ptree& tree, const std::string& node_name);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Determines if we can move to the next entry. </summary>
			///
			/// <returns>	true if another entry is available, false if the end of the list was reached. </returns>
			bool MoveNext() final override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the current entry. </summary>
			///
			/// <returns>	A i_configuration_leaf representing the current tree child. </returns>
			std::shared_ptr<i_configuration_leaf> Current() final override;
		};
	};};
};