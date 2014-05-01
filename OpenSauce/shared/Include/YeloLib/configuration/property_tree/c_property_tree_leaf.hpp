/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/configuration/i_configuration_leaf.hpp>
#include <YeloLib/configuration/i_configuration_leaf_iterator.hpp>

namespace Yelo
{
	namespace Configuration { namespace PropertyTree
	{
		class c_property_tree_leaf
			: public i_configuration_leaf
		{
			boost::property_tree::ptree& m_property_tree;
			
		public:
			c_property_tree_leaf(boost::property_tree::ptree* property_tree);
		
			bool GetValue(const bool default_value = false);
			sbyte GetValue(const sbyte default_value = 0);
			int16 GetValue(const int16 default_value = 0);
			int32 GetValue(const int32 default_value = 0);
			byte GetValue(const byte default_value = 0);
			uint16 GetValue(const uint16 default_value = 0);
			uint32 GetValue(const uint32 default_value = 0);
			real GetValue(const real default_value = 0);
			double GetValue(const double default_value = 0);
			std::string GetValue(const std::string default_value = "");
			
			void SetValue(const bool value);
			void SetValue(const sbyte value);
			void SetValue(const int16 value);
			void SetValue(const int32 value);
			void SetValue(const byte value);
			void SetValue(const uint16 value);
			void SetValue(const uint32 value);
			void SetValue(const real value);
			void SetValue(const double value);
			void SetValue(const std::string value);

			bool HasChild(const std::string child_name) const;
			std::unique_ptr<i_configuration_leaf_iterator> GetChildIterator(const std::string child_name);
			std::unique_ptr<i_configuration_leaf> GetChild(const std::string child_name);
			std::unique_ptr<i_configuration_leaf> AddChild(const std::string child_name);
		};
	}}
}