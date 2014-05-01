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
			static t_configuration_file_ptr CreateConfigurationFile(const std::string file_path);
		};
	}
}