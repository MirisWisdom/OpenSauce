/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/hs/hs_structures.hpp>

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
		extern const cstring k_external_global_opensauce_override_name;

		// Interpret [data] as [type] data. Takes the [data].pointer and sets [data] to the dereferenced value.
		// If [data].pointer is NULL, then this sets [data] to [type]'s NONE equivalent.
		void UpdateTypeHolderFromPtrToData(TypeHolder& data, const Enums::hs_type type);
		// Interpret [ptr] as a [type] pointer. Takes [ptr], deferences it and sets [data] to the value.
		// [data] is 'const' as this doesn't modify the pointer, but the data which it points to.
		void UpdateTypeHolderDataFromPtr(const TypeHolder& data, const Enums::hs_type type, void* ptr);
	};
};

#define HS_TYPE(hstype) BOOST_JOIN(Yelo::Enums::_hs_type_,hstype)


//////////////////////////////////////////////////////////////////////////
// Macro glue for declaring/defining a hs function which takes no arguments
#define DECLARE_HS_FUNCTION(name) extern Yelo::Scripting::hs_function_definition function_##name##_definition

//////////////////////////////////////////////////////////////////////////
// Macro glue for declaring/defining a hs function which takes various arguments for input
#define DECLARE_HS_FUNCTION_WITH_PARAMS(name) extern hs_function_definition function_##name##_definition

#define GET_HS_FUNCTION(name) Yelo::Scripting::function_##name##_definition


//////////////////////////////////////////////////////////////////////////
// Macro glue for declaring/defining a normal hs global
#define DECLARE_HS_GLOBAL(name) extern Yelo::Scripting::hs_global_definition global_##name##_definition

//////////////////////////////////////////////////////////////////////////
// Macro glue for declaring/defining an hs global with special flags
#define DECLARE_HS_GLOBAL_EX(name) extern Yelo::Scripting::hs_global_definition global_##name##_definition

//////////////////////////////////////////////////////////////////////////
// Macro glue for declaring/defining a hs global whose value is stored
// in the engine itself. Was useful in the case of 'gravity'
#define DECLARE_HS_GLOBAL2(name) extern Yelo::Scripting::hs_global_definition global_##name##_definition

#define GET_HS_GLOBAL(name) Yelo::Scripting::global_##name##_definition
