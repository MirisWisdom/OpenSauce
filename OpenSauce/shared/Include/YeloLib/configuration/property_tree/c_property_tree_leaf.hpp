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
			c_property_tree_leaf(boost::property_tree::ptree& property_tree);
		
			bool GetValue(const bool default_value = false) final override;
			sbyte GetValue(const sbyte default_value = 0) final override;
			int16 GetValue(const int16 default_value = 0) final override;
			int32 GetValue(const int32 default_value = 0) final override;
			byte GetValue(const byte default_value = 0) final override;
			uint16 GetValue(const uint16 default_value = 0) final override;
			uint32 GetValue(const uint32 default_value = 0) final override;
			real GetValue(const real default_value = 0) final override;
			double GetValue(const double default_value = 0) final override;
			std::string GetValue(const std::string& default_value = "") final override;
			
			void SetValue(const bool value) final override;
			void SetValue(const sbyte value) final override;
			void SetValue(const int16 value) final override;
			void SetValue(const int32 value) final override;
			void SetValue(const byte value) final override;
			void SetValue(const uint16 value) final override;
			void SetValue(const uint32 value) final override;
			void SetValue(const real value) final override;
			void SetValue(const double value) final override;
			void SetValue(const std::string& value) final override;

			bool HasChild(const std::string& child_name) const final override;
			std::unique_ptr<i_configuration_leaf_iterator> GetChildIterator(const std::string& child_name) final override;
			std::unique_ptr<i_configuration_leaf> GetChild(const std::string& child_name) const final override;
			std::unique_ptr<i_configuration_leaf> AddChild(const std::string& child_name) final override;
		};
	};};
};