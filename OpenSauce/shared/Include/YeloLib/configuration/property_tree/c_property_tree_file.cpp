/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/configuration/property_tree/c_property_tree_file.hpp>

#include <YeloLib/configuration/property_tree/c_property_tree_leaf_iterator.hpp>

namespace Yelo
{
	namespace Configuration { namespace PropertyTree
	{
		c_property_tree_file::c_property_tree_file(const std::string& file_path)
			: c_property_tree_leaf(m_property_tree_file)
			, m_file_path(file_path)
		{
			m_property_tree_file.clear();
		}
		
		void c_property_tree_file::Load()
		{
			try
			{
				ReadPropertyTree(m_file_path, m_property_tree_file);
			}
			catch(...)
			{ //-V565
				// Doesn't really matter that the read failed, default values will be used instead and the settings are re-written when the game closes anyway
			}
		}
		
		void c_property_tree_file::Save()
		{
			WritePropertyTree(m_file_path, m_property_tree_file);
		}
		
		void c_property_tree_file::Clear()
		{
			m_property_tree_file.clear();
		}

		std::unique_ptr<i_configuration_leaf> c_property_tree_file::Root()
		{
			return std::unique_ptr<i_configuration_leaf>(new c_property_tree_leaf(m_property_tree_file));
		}
	};};
};