/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Flags
	{
		enum hs_yelo_definition_flags : word_flags
		{
			_hs_yelo_definition_internal_bit,
			_hs_yelo_definition_is_synchronized_bit,

			_hs_yelo_definition_internal_flag =	FLAG(_hs_yelo_definition_internal_bit),
			// changes to the global are sync'd
			_hs_yelo_definition_is_synchronized = FLAG(_hs_yelo_definition_is_synchronized_bit),
		};
	};

	namespace Scripting
	{
		typedef void* (API_FUNC* hs_yelo_function_proc)();
		typedef void* (API_FUNC* hs_yelo_function_with_params_proc)(void** arguments);

		// This is the name of a blam global which isn't used in release builds of the game.
		// We use this global to expose the build version to scripts without causing harm when not using OS.
		// It was chosen due to the hs_type being a 'real', which allows us to specify the version number as "MAJ.MIN"
		// See: K_OPENSAUCE_VERSION
		const cstring k_external_global_opensauce_override_name = "ai_debug_path_maximum_radius";
	};
};

#define HS_TYPE(hstype) BOOST_JOIN(Yelo::Enums::_hs_type_,hstype)