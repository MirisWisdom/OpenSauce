/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <yelo/configuration/i_configuration_value.hpp>

namespace Yelo
{
	namespace Configuration
	{
		/// <summary>	Differentiation interface for configuration containers. </summary>
		class i_configuration_container
			: public i_configuration_value
		{ };
	};
};