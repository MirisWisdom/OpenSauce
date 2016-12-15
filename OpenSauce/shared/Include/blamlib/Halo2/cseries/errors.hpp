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
			_error_message_priority,
		};

		enum error_category : long_enum
		{
			_error_category_none,
			_error_category_warning,	// _error_category_internal_full
			_error_category_debug,		// _error_category_internal_subfolder
			_error_category_animation,
			_error_category_ai,
			_error_category_shaders,
			_error_category_geometry,
			_error_category_environment,
			_error_category_objects,
			_error_category_networking,
			_error_category_tags,
			_error_category_ui,
			_error_category_sound,
			_error_category_multiplayer,
			_error_category_effects,
			_error_category_animation_audio_content,
			_error_category_environment_materials,
			_error_category_object_materials,
			_error_category_design,
			_error_category_localization,
			_error_category,
		};
	};
};