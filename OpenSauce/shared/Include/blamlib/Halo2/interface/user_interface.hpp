/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			// Maximum number of users per machine
			k_number_of_users = 
#if PLATFORM_TARGET == PLATFORM_TARGET_PC
			1,
#else
			4,
#endif
		};
	};
};