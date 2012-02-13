/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/ScriptLibrary.hpp"

#include "Common/YeloSettings.hpp"
#include "TagGroups/project_yellow_definitions.hpp"
#include "Game/Camera.hpp"
#include "Networking/MessageDefinitions.hpp"

#include <Blam/Halo1/BlamMemoryUpgrades.hpp>

// Includes for MiscScriptingFunctions.inl
#include "Game/GameEngine.hpp"
#include "Game/GameState.hpp"
#include "Networking/MessageDeltas.hpp"
#include "Networking/GameSpyApi.hpp"
#include "TagGroups/project_yellow_definitions.hpp"
#include "Rasterizer/GBuffer.hpp"
#include "Rasterizer/PostProcessing/PostProcessingScripting.hpp"

namespace Yelo
{
	namespace Scripting
	{
#include "Game/ScriptLibrary.Macros.inl"



#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_SCRIPT_LIBRARY
#include "Memory/_EngineLayout.inl"


#define __SCRIPTLIBRARY_INCLUDE_ID __SCRIPTLIBRARY_DEFINITIONS
#include "Game/ScriptLibrary.inl"

#define __SCRIPTLIBRARY_INCLUDE_ID __SCRIPTLIBRARY_DEFINITION_LISTING
#include "Game/ScriptLibrary.inl"

#pragma region script functions related

		static const int32 K_HS_YELO_FUNCTION_COUNT = NUMBEROF(hs_yelo_functions);
		int32 HSYeloFunctionCount() { return K_HS_YELO_FUNCTION_COUNT; }
		const hs_function_definition& HSYeloFunction(int16 index) { return *hs_yelo_functions[index]; }


		static hs_function_definition** hs_function_table;
		const hs_function_definition* HSFunctionTable() DPTR_IMP_GET(hs_function_table);

#pragma endregion

#pragma region script globals related

		static const int32 K_HS_YELO_GLOBALS_COUNT = NUMBEROF(hs_yelo_globals);
		int32 HSYeloGlobalCount() { return K_HS_YELO_GLOBALS_COUNT; }
		const hs_global_definition& HSYeloGlobal(int16 index) { return *hs_yelo_globals[index]; }


		static hs_global_definition** hs_external_globals;
		const hs_global_definition* HSExternalGlobals() DPTR_IMP_GET(hs_external_globals);

#pragma endregion

		// Initialize the [k_external_global_opensauce_override_name] global
		// Called from MemoryUpgrades (since we process the globals list there anyway)
		static void InitializeExternalGlobalOpenSauceOverride(hs_global_definition& global)
		{
			// Not really constant but should be treated as such
			static real k_opensauce_version = K_OPENSAUCE_VERSION;

			if(global.address == NULL && global.type == HS_TYPE(real))
				global.Value._real = &k_opensauce_version;
		}
#include "Game/ScriptLibrary.MemoryUpgrades.inl"
#include "Game/ScriptLibrary.MiscScriptingFunctions.inl"

		void InitializeCreateScriptFunction(); // forward declare

		void InitializeLibrary()
		{
			// Moved this code here to stop the compiler from creating dynamic initializers for them
			// Be sure you're not calling HSFunctionTable or HSExternalGlobals before InitializeLibrary has ran
			hs_function_table = GET_DPTR2(hs_function_table);
			hs_external_globals = GET_DPTR2(hs_external_globals);

			//GET_HS_FUNCTION(null).evaluate = (hs_evaluate_proc)GET_FUNC_VPTR(HS_NULL_EVALUTE);

			//////////////////////////////////////////////////////////////////////////
			MemoryUpgradesInitialize();

			InitializeCreateScriptFunction();


#ifdef API_DEBUG
	#if PLATFORM_IS_USER
			InitializeScriptFunctionWithParams(Enums::_hs_function_dump_view_state, Camera::DumpViewStateEvaluate);
	#else
			NullifyScriptFunctionWithParams( GET_HS_FUNCTION(dump_view_state) );
	#endif

	#if !defined(YELO_NO_NETWORK) && FALSE
			InitializeScriptFunction(Enums::_hs_function_test_networking, MessageDeltas::TestToNetwork);
	#else
			NullifyScriptFunction( GET_HS_FUNCTION(test_networking) );
	#endif
#endif

			InitializeMiscFunctions();
			GameState::InitializeScripting();

			//////////////////////////////////////////////////////////////////////////
			AllowFullAccess(true);
		}

		void DisposeLibrary()
		{
		}

		void AllowFullAccess(bool allow)
		{
			static byte code[] = {
				0xB0, 0x01, 0xC3,	// implicitly allow expression access to everything
				0x8A, 0xD1, 0xC0	// default expression access to the game's settings
			};
			if(allow)	Memory::WriteMemory(GET_FUNC_VPTR(HS_VALID_ACCESS_FLAGS), code, 3);
			else		Memory::WriteMemory(GET_FUNC_VPTR(HS_VALID_ACCESS_FLAGS), code+3, 3);
		}


		static void ScriptFunctionSetEvaluteProc(hs_function_definition& function, hs_evaluate_proc proc)
		{
			if(function.paramc != 0)
				YELO_DEBUG_FORMAT("Setting script function which has parameters (expected none): %s", function.name);

			function.evaluate = proc;
		}
		static void ScriptFunctionWithParamsSetEvaluteProc(hs_function_definition& function, hs_evaluate_proc proc)
		{
			if(function.paramc == 0)
				YELO_DEBUG_FORMAT("Setting script function which has no parameters (expected 1 or more): %s", function.name);

			function.evaluate = proc;
		}

		void NullifyScriptFunction(hs_function_definition& function)
		{
			ScriptFunctionSetEvaluteProc( function, CAST_PTR(hs_evaluate_proc,GET_FUNC_VPTR(HS_NULL_EVALUATE)) );
		}

		void NullifyScriptFunctionWithParams(hs_function_definition& function)
		{
			ScriptFunctionWithParamsSetEvaluteProc( function, CAST_PTR(hs_evaluate_proc,GET_FUNC_VPTR(HS_NULL_WITH_PARAMS_EVALUATE)) );
		}


		#pragma region hs function creatation internals

		// [hs_function_table] in hs_eval_func_has_param
#define HS_EVAL_INDEX_HS_FUNC_TABLE_WITH_PARAM			(5+3)

		// [func] in hs_eval_func_no_param
#define HS_EVAL_INDEX_FUNC_NO_PARAM						(0+2)

		// [func] in hs_eval_func_has_param
#define HS_EVAL_INDEX_FUNC_WITH_PARAM					(46+2)

		// [hs_return_address] in hs_eval_func_no_param
#define HS_EVAL_INDEX_HS_RETURN_NO_PARAM				(10+2)

		// [hs_return_address] in hs_eval_func_has_param
#define HS_EVAL_INDEX_HS_RETURN_WITH_PARAM				(55+2)

		// [hs_arguments_evaluate_address] in hs_eval_func_has_param
#define HS_EVAL_INDEX_HS_ARGUMENTS_EVALUTE_WITH_PARAM	(32+2)


		// opcodes for a hs_evaluate function that takes no parameters
		static byte hs_eval_func_no_param[] = {
			0xFF, 0x15, 0x00, 0x00, 0x00, 0x00, // 00 6, call [func]
			0x8B, 0x4C, 0x24, 0x08,				// 06 4, mov ecx, [esp+8]
			0xFF, 0x15, 0x00, 0x00, 0x00, 0x00, // 10 6, call [hs_return_address]
			0xC3								// 16 1, retn
		};

		// opcodes for a hs_evaluate function that takes parameters
		static byte hs_eval_func_has_param[] = {
			0x0F, 0xBF, 0x44, 0x24, 0x04,				// 00 5, movsx   eax, [esp+arg_0]
			0x8B, 0x04, 0x85, 0x00, 0x00, 0x00, 0x00,	// 05 7, mov     eax, hs_function_table[eax*4]
			0x8B, 0x4C, 0x24, 0x0C,						// 12 4, mov     ecx, [esp+arg_8]
			0x56,										// 16 1, push    esi
			0x8B, 0x74, 0x24, 0x0C,						// 17 4, mov     esi, [esp+4+arg_4]
			0x51,										// 21 1, push    ecx
			0x8D, 0x50, 0x1C,							// 22 3, lea     edx, [eax+1Ch]
			0x0F, 0xBF, 0x40, 0x1A,						// 25 4, movsx   eax, word ptr [eax+1Ah]
			0x52,										// 29 1, push	 edx
			0x50,										// 30 1, push    eax
			0x56,										// 31 1, push    esi
			0xFF, 0x15, 0x00, 0x00, 0x00, 0x00,			// 32 6, call    [hs_arguments_evaluate_address]
			0x83, 0xC4, 0x10,							// 38 3, add     esp, 10h
			0x85, 0xC0,									// 41 2, test    eax, eax
			0x74, 0x10,									// 43 2, jz      short failed
			0x50,										// 45 1, push    eax ; push the argument list
			0xFF, 0x15, 0x00, 0x00, 0x00, 0x00,			// 46 6, call    [func]
			0x8B, 0xCE,									// 52 2, mov     ecx, esi
			0x5E,										// 54 1, pop     esi
			0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,			// 55 6, jmp     [hs_return_address]
			// failed:
			0x5E,										// 61 1, pop     esi
			0xC3,										// 62 1, retn

			0, // 64 1, pad
		};


		// Only 39 functions in CE release are actually 'null'...but now in OS2, we no longer overwrite existing functions!
		// So ignore this comment :p
		API_CODEDATA static uint32 hs_eval_func_ptrs[Enums::k_hs_script_functions_count_upgrade - Enums::k_hs_functions_count];
		// next free hs_eval_func_ptrs index
		static int32 hs_eval_func = 0;
		API_CODEDATA static byte hs_func_pool[NUMBEROF(hs_eval_func_ptrs)][sizeof(hs_eval_func_has_param)];

#pragma warning( push )
#pragma warning( disable : 4311 ) // bitching about this typecast
#pragma warning( disable : 4312 ) // bitching about typecast
		static void InitializeCreateScriptFunction()
		{
			static uint32 hs_return_address = GET_FUNC_PTR(HS_RETURN);
			static uint32 hs_arguments_evaluate_address = GET_FUNC_PTR(HS_ARGUMENTS_EVALUATE);
			static uint32 hs_function_table_address = CAST_PTR(uint32, &_upgrade_globals.functions.table[0]);

			uint32* temp = NULL;

			// with params functions
			{
				temp = CAST_PTR(uint32*, &(hs_eval_func_has_param[HS_EVAL_INDEX_HS_FUNC_TABLE_WITH_PARAM]) );
				*temp = hs_function_table_address;

				temp = CAST_PTR(uint32*, &(hs_eval_func_has_param[HS_EVAL_INDEX_HS_ARGUMENTS_EVALUTE_WITH_PARAM]) );
				*temp = CAST_PTR(uint32, &hs_arguments_evaluate_address);

				temp = CAST_PTR(uint32*, &(hs_eval_func_has_param[HS_EVAL_INDEX_HS_RETURN_WITH_PARAM]) );
				*temp = CAST_PTR(uint32, &hs_return_address);
			}

			// no params functions
			{
				temp = CAST_PTR(uint32*, &(hs_eval_func_no_param[HS_EVAL_INDEX_HS_RETURN_NO_PARAM]) );
				*temp = CAST_PTR(uint32, &hs_return_address);
			}
		}

		// Creates a evaluate function that calls [func] and passes
		// an array of parameters to it if it [takes_params]
		//
		// [func] - pointer to a API_FUNC type function
		// Returns: evaluate function address
		static void* CreateScriptFunction(void* func, bool takes_params)
		{
			if(hs_eval_func >= NUMBEROF(hs_eval_func_ptrs)) return NULL; // we don't want to go over our set limit

			void* evaluate = NULL;
			hs_eval_func_ptrs[hs_eval_func] = CAST_PTR(uint32, func);
			uint32* temp = NULL;

			if(takes_params)
			{
				Memory::WriteMemory(
					hs_func_pool[hs_eval_func], 
					hs_eval_func_has_param, 
					sizeof(hs_eval_func_has_param));

				temp = CAST_PTR(uint32*, &(hs_func_pool[hs_eval_func][HS_EVAL_INDEX_FUNC_WITH_PARAM]) );
				*temp = CAST_PTR(uint32, &(hs_eval_func_ptrs[hs_eval_func]) );
			}
			else
			{
				Memory::WriteMemory(
					hs_func_pool[hs_eval_func], 
					hs_eval_func_no_param, 
					sizeof(hs_eval_func_no_param));

				temp = CAST_PTR(uint32*, &(hs_func_pool[hs_eval_func][HS_EVAL_INDEX_FUNC_NO_PARAM]) );
				*temp = CAST_PTR(uint32, &(hs_eval_func_ptrs[hs_eval_func]) );
			}

			evaluate = &(*hs_func_pool[hs_eval_func]);
			hs_eval_func++; // go to the next free func ptr
			return evaluate;
		}
#pragma warning( pop )
		#pragma endregion

		void InitializeScriptFunction(Enums::hs_function_enumeration function, hs_yelo_function_proc proc)
		{
			if(function > NONE && function < Enums::k_hs_function_enumeration_count)
			{
				ScriptFunctionSetEvaluteProc( *hs_yelo_functions[function], 
					CAST_PTR(hs_evaluate_proc, CreateScriptFunction(proc, false)) );
			}
		}

		void InitializeScriptFunctionWithParams(Enums::hs_function_enumeration function, hs_yelo_function_with_params_proc proc)
		{
			if(function > NONE && function < Enums::k_hs_function_enumeration_count)
			{
				ScriptFunctionWithParamsSetEvaluteProc( *hs_yelo_functions[function], 
					CAST_PTR(hs_evaluate_proc, CreateScriptFunction(proc, true)) );
			}
		}

		static bool DefinitionsNameMatch(cstring name, const tag_string definition_name[2])
		{
			size_t length = strlen(name);
			bool result = !strncmp(name, definition_name[0], Enums::k_tag_string_length);

			// if the scripting name is longer than 31 characters, compare the other characters too
			if(length > Enums::k_tag_string_length)
				result &= !strncmp(	name+(Enums::k_tag_string_length+1), 
									definition_name[1], 
									Enums::k_tag_string_length);

			return result;
		}

		bool DefinitionsMatch(const TagGroups::scripting_block& data)
		{
			bool is_valid = false;

			for(int32 x = 0; x < data.new_functions.Count; x++)
			{
				const TagGroups::script_function_block& def = 
					data.new_functions[x];

				is_valid = false;
				// continue checking each EXPORTED function with the yelo script definitions
				for(int32 f = 0; f < K_HS_YELO_FUNCTION_COUNT && !is_valid; f++)
				{
					if( DefinitionsNameMatch(hs_yelo_functions[f]->name, def.name) &&		// check if names match...
						def.parameters.Count == hs_yelo_functions[f]->paramc &&				// check if we have the same amount of parameters
						(is_valid = def.return_type == hs_yelo_functions[f]->return_type)	// check if we have the same return type
						)
					{
						const _enum* params = def.parameters.Definitions;					// check if the parameter types are the same
						for(int32 p = 0; 
							p < def.parameters.Count && is_valid; // continue checking while things compare as a match
							p++)
							is_valid = params[p] == hs_yelo_functions[f]->params[p];
					}
				}

				if(!is_valid)
					return false;
			}

			for(int32 x = 0; x < data.new_globals.Count; x++)
			{
				const TagGroups::script_global_block& def = 
					data.new_globals[x];

				is_valid = false;
				for(int32 g = 0; g < K_HS_YELO_GLOBALS_COUNT && !is_valid; g++)
				{
					is_valid = 
						DefinitionsNameMatch(hs_yelo_globals[g]->name, def.name) &&// check if names match...
						def.type == hs_yelo_globals[g]->type;						// check if the global types match too
				}

				if(!is_valid)
					return false;
			}

			return true;
		}

		void UpdateTypeHolderFromPtrToData(TypeHolder& data, const Enums::hs_type type)
		{
			if(data.pointer != NULL)
			{
				TypeHolder copy; copy.pointer = data.pointer;
				data.pointer = NULL;
				switch(type)
				{
				// also for byte
				case HS_TYPE(bool):	data.byte = *copy.ptr.byte;		break;
				case HS_TYPE(short):data.uint16 = *copy.ptr.uint16;	break;
				case HS_TYPE(real):
				case HS_TYPE(long):	data.uint32 = *copy.ptr.uint32;	break;
					break;
				}
			}
			else // the pointer is null, so default to NONE
			{
				data.pointer = NULL;

				switch(type)
				{
				// also for byte
				case HS_TYPE(bool):	data.byte = NONE;	break;
				case HS_TYPE(short):data.int16 = NONE;	break;
				case HS_TYPE(real): data.real = -1.0f;	break;
				case HS_TYPE(long):	data.int32 = NONE;	break;
					break;
				}
			}
		}
		void UpdateTypeHolderDataFromPtr(const TypeHolder& data, const Enums::hs_type type, void* ptr)
		{
			if(ptr != NULL)
			{
				switch(type)
				{
				// also for byte
				case HS_TYPE(bool):	*data.ptr.byte = *CAST_PTR(byte*, ptr);		break;
				case HS_TYPE(short):*data.ptr.uint16 = *CAST_PTR(uint16*, ptr);	break;
				case HS_TYPE(real):
				case HS_TYPE(long):	*data.ptr.uint32 = *CAST_PTR(uint32*, ptr);	break;
					break;
				}
			}
		}
	};
};