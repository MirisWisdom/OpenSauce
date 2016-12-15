/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/configuration/i_configuration_file.hpp>
#include <YeloLib/configuration/property_tree/c_property_tree_leaf.hpp>

namespace Yelo
{
	namespace Configuration { namespace PropertyTree
	{		
		class c_property_tree_file
			abstract
			: public c_property_tree_leaf
			, public i_configuration_file
		{
			std::string m_file_path;

			boost::property_tree::ptree m_property_tree_file;
		
		private:
			virtual void ReadPropertyTree(const std::string& file_path, boost::property_tree::ptree& property_tree) = 0;
			virtual void WritePropertyTree(const std::string& file_path, boost::property_tree::ptree& property_tree) = 0;
			
		public:
			c_property_tree_file(const std::string& file_path);

			bool Load() final override;
			void Save() final override;
			void Clear() final override;
			
			std::unique_ptr<i_configuration_leaf> Root() final override;
		};
	};};
};