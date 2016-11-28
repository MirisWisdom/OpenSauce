/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/configuration/i_configuration_leaf.hpp>

namespace Yelo
{
	namespace Configuration
	{
		/// <summary>	Base configuration value interface. </summary>
		class i_configuration_value
			abstract
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Get's the nodes name. </summary>
			///
			/// <returns>	A std::string containing the nodes name. </returns>
			virtual const std::string& GetName() const = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the objects value from the supplied node. </summary>
			///
			/// <param name="node">	The node to get the value from. </param>
			virtual void GetValue(i_configuration_leaf& node) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the objects value to the supplied node. </summary>
			///
			/// <param name="node">	The node to set the value to. </param>
			virtual void SetValue(i_configuration_leaf& node) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// 	Gets the value from the first child of the supplied node. If the child does not exist, it is created.
			/// </summary>
			///
			/// <param name="parent_node">	The parent node to get the value from. </param>
			virtual void GetValueFromParent(i_configuration_leaf& parent_node) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the value to a child added to the supplied node. </summary>
			///
			/// <param name="parent_node">	The parent node to set the value to. </param>
			virtual void SetValueToParent(i_configuration_leaf& parent_node) = 0;
		};
	};
};