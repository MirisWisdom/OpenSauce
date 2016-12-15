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
		class i_configuration_leaf;

		/// <summary>	A configuration leaf iterator interface. </summary>
		class i_configuration_leaf_iterator
			abstract
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Determines if we can move to the next iterator value. </summary>
			///
			/// <returns>	true if it succeeds, false if the end of the list has been reached. </returns>
			virtual bool MoveNext() = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the iterators current value. </summary>
			///
			/// <returns>	An i_configuration_leaf pointer. </returns>
			virtual std::shared_ptr<i_configuration_leaf> Current() = 0;
		};
	};
};