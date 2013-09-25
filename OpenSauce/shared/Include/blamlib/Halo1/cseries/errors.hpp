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

	namespace blam
	{
		typedef void (PLATFORM_API* proc_error)(Enums::error_message_priority priority, cstring format, ...);

		extern proc_error error;
#ifndef YELO_NO_ERROR_MACRO
	#define YELO_ERROR(priority, format, ...) \
		Yelo::blam::error(Yelo::Enums::priority, format, __VA_ARGS__)
#else
	#define YELO_ERROR(priority, format, ...) __noop
#endif

#ifndef YELO_NO_WARN_MACRO
	#define YELO_WARN(format, ...) \
		Yelo::blam::error(Yelo::Enums::_error_message_priority_warning, format, __VA_ARGS__)
#else
	#define YELO_WARN(format, ...) __noop
#endif
	};
};