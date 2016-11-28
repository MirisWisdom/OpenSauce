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
		/// <summary>	Base interface for a configuration file. </summary>
		class i_configuration_file
			abstract
		{
		public:
			/// <summary>	Loads the configuration from a file. </summary>
			virtual bool Load() = 0;

			/// <summary>	Saves the configuration to a file. </summary>
			virtual void Save() = 0;

			/// <summary>	Clears the configuration to it's initial state. </summary>
			virtual void Clear() = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the root configuration leaf. </summary>
			///
			/// <returns>	A i_configuration_leaf object representing the configuration root. </returns>
			virtual std::unique_ptr<i_configuration_leaf> Root() = 0;
		};
		typedef std::shared_ptr<i_configuration_file> configuration_file_ptr_t;
	};
};