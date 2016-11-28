/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/configuration/property_tree/c_property_tree_file.hpp>

#include <YeloLib/configuration/property_tree/c_property_tree_leaf_iterator.hpp>
#include <YeloLib/configuration/property_tree/property_tree_logging.hpp>

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
		
		bool c_property_tree_file::Load()
		{
			try
			{
				ReadPropertyTree(m_file_path, m_property_tree_file);

			#ifdef PRINT_PROPERTY_TREES
				PrintPropertyTree(m_property_tree_file);
			#endif
			}
			catch(boost::property_tree::ptree_error e)
			{ //-V565
#ifdef DEBUG
				if(IsDebuggerPresent())
				{
					OutputDebugString("Exception occurred when reading a property tree:\n");
					OutputDebugString(e.what());
					OutputDebugString("\n");
				}
#endif
				return false;
			}
			return true;
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