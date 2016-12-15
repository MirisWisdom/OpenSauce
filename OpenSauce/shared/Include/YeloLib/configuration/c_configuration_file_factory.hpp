/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/configuration/i_configuration_file.hpp>

namespace Yelo
{
	namespace Configuration
	{
		/// <summary>	Static factory class for creating a configuration file. </summary>
		class c_configuration_file_factory
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Creates a configuration file for a file on disc. </summary>
			///
			/// <param name="file_path">	Full pathname of the file. </param>
			///
			/// <returns>	The new configuration file. </returns>
			static configuration_file_ptr_t CreateConfigurationFile(const std::string& file_path);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Creates a readonly configuration file from data in memory. </summary>
			///
			/// <param name="data">  	The file data. </param>
			/// <param name="format">	Describes the format to use. </param>
			///
			/// <returns>	The new configuration file. </returns>
			static configuration_file_ptr_t CreateConfigurationFile(cstring data, cstring format);
		};
	};
};