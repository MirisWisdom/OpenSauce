/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include "Memory/MemoryInterface.hpp"
#include <Blam/Halo1/BlamScriptingDefinitions.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct scripting_block;
	};
};

namespace Yelo
{
	namespace Enums
	{
#define __SCRIPTLIBRARY_INCLUDE_ID __SCRIPTLIBRARY_ENUMERATIONS
#include "Game/ScriptLibrary.inl"
	};

	namespace Scripting
	{
		#pragma region hs_function
//////////////////////////////////////////////////////////////////////////
// Macro glue for declaring/defining a hs function which takes no arguments
#define DECLARE_HS_FUNCTION(name) extern Yelo::Scripting::hs_function_definition function_##name##_definition

//////////////////////////////////////////////////////////////////////////
// Macro glue for declaring/defining a hs function which takes various arguments for input
#define DECLARE_HS_FUNCTION_WITH_PARAMS(name) extern hs_function_definition function_##name##_definition

#define GET_HS_FUNCTION(name) Yelo::Scripting::function_##name##_definition


		const hs_function_definition* HSFunctionTable();

		const hs_function_definition& HSYeloFunction(int16 index);
		int32 HSYeloFunctionCount();
		#pragma endregion

		#pragma region hs_global
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
		// Set the function's (which expects parameters) evaluator to one 
		// which does nothing and returns zero
		void NullifyScriptFunctionWithParams(hs_function_definition& function);

		// Initialize the function's evaluator to one which we've defined 
		// in our code. Evaluator takes no parameters but may return a value.
		void InitializeScriptFunction(Enums::hs_function_enumeration function, 
			hs_yelo_function_proc proc);
		// Initialize the function's evaluator to one which we've defined 
		// in our code. Evaluator expects parameters and may return a value.
		void InitializeScriptFunctionWithParams(Enums::hs_function_enumeration function, 
			hs_yelo_function_with_params_proc proc);

		// True if the script definitions in [data] match the functions/globals 
		// defined by Yelo's code.
		bool DefinitionsMatch(const TagGroups::scripting_block& data);

	};
};