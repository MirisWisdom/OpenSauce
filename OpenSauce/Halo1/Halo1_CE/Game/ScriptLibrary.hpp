/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/hs/hs.hpp>

#include "Memory/MemoryInterface.hpp"

namespace Yelo
{
	namespace TagGroups
	{
		struct s_scripting_definitions;
	};

	namespace Enums
	{
#define __SCRIPTLIBRARY_INCLUDE_ID __SCRIPTLIBRARY_ENUMERATIONS
#include "Game/ScriptLibrary.inl"
	};

	namespace Scripting
	{
		#pragma region hs_function
		const hs_function_definition* HSFunctionTable();

		const hs_function_definition& HSYeloFunction(int16 index);
		int32 HSYeloFunctionCount();
		#pragma endregion

		#pragma region hs_global
		const hs_global_definition* HSExternalGlobals();

		const hs_global_definition& HSYeloGlobal(int16 index);
		int32 HSYeloGlobalCount();
		#pragma endregion


		void InitializeLibrary();
		void DisposeLibrary();

		// Unlocks or Locks all functions\globals that can't be used normally
		void AllowFullAccess(bool allow);

		// Set the function's (with no parameters) evaluator to one which 
		// does nothing and returns zero
		void NullifyScriptFunction(hs_function_definition& function);
		void NullifyScriptFunction(Enums::hs_function_enumeration function);
		// Set the function's (which expects parameters) evaluator to one 
		// which does nothing and returns zero
		void NullifyScriptFunctionWithParams(hs_function_definition& function);
		void NullifyScriptFunctionWithParams(Enums::hs_function_enumeration function);

		// Initialize the function's evaluator to one which we've defined 
		// in our code. Evaluator takes no parameters but may return a value.
		void InitializeScriptFunction(Enums::hs_function_enumeration function, 
			proc_hs_yelo_function proc);
		// Initialize the function's evaluator to one which we've defined 
		// in our code. Evaluator expects parameters and may return a value.
		void InitializeScriptFunctionWithParams(Enums::hs_function_enumeration function, 
			proc_hs_yelo_function_with_params proc);

#define YELO_INIT_SCRIPT_FUNCTION(function_index, proc)							Scripting::InitializeScriptFunction(function_index, proc)
#define YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS(function_index, proc)				Scripting::InitializeScriptFunctionWithParams(function_index, proc)
#if PLATFORM_IS_USER
	#define YELO_INIT_SCRIPT_FUNCTION_USER(function_index, proc)				YELO_INIT_SCRIPT_FUNCTION(function_index, proc)
	#define YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(function_index, proc)	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS(function_index, proc)

	#define YELO_INIT_SCRIPT_FUNCTION_DEDI(function_index, proc)				Scripting::NullifyScriptFunction(function_index)
	#define YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_DEDI(function_index, proc)	Scripting::NullifyScriptFunctionWithParams(function_index)
#else
	#define YELO_INIT_SCRIPT_FUNCTION_USER(function_index, proc)				Scripting::NullifyScriptFunction(function_index)
	#define YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(function_index, proc)	Scripting::NullifyScriptFunctionWithParams(function_index)

	#define YELO_INIT_SCRIPT_FUNCTION_DEDI(function_index, proc)				YELO_INIT_SCRIPT_FUNCTION(function_index, proc)
	#define YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_DEDI(function_index, proc)	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS(function_index, proc)
#endif

		// True if the script definitions in [data] match the functions/globals 
		// defined by Yelo's code.
		bool DefinitionsMatch(const TagGroups::s_scripting_definitions& data);
	};
};