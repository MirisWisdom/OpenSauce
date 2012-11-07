/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum error_message_priority : long_enum
		{
			_error_message_priority_none, // ?
			_error_message_priority_warning,
			_error_message_priority_assert,
			_error_message_priority_critical,

			k_number_of_error_message_priorities
		};
	};
};