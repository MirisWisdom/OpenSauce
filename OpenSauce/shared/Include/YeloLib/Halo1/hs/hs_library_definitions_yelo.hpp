/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Scripting
	{
		struct hs_function_definition;
		struct hs_global_definition;

		struct s_hs_library_fixup
		{
			size_t index;
			union {
				void* address;

				hs_function_definition* function;
				hs_global_definition* global;
			};
		};

		class c_hs_library
		{
			static size_t g_functions_yelo_start_index; ///< Starting index of OS-defined functions
			static size_t g_functions_next_register_index;
			static std::vector<hs_function_definition*> g_functions;

			static size_t g_external_globals_yelo_start_index; ///< Starting index of OS-defined globals
			static size_t g_external_globals_next_register_index;
			static std::vector<hs_global_definition*> g_external_globals;

		public:
			static void Initialize();
			static void Dispose();

			static void FinishRegisteringBlamFunctions()
			{
				g_functions_yelo_start_index = g_functions.size();
			}
			static void FinishRegisteringBlamGlobals()
			{
				g_external_globals_yelo_start_index = g_external_globals.size();
			}

			static size_t GetYeloFunctionsCount()
			{
				return g_functions.size() + g_functions_yelo_start_index;
			}
			static size_t GetYeloGlobalsCount()
			{
				return g_external_globals.size() + g_external_globals_yelo_start_index;
			}

			static const std::vector<hs_function_definition*>& GetFunctionsTable(size_t* yelo_start_index = nullptr)
			{
				if (yelo_start_index)
					*yelo_start_index = g_functions_yelo_start_index;

				return g_functions;
			}
			static const std::vector<hs_global_definition*>& GetExternalGlobals(size_t* yelo_start_index = nullptr)
			{
				if (yelo_start_index)
					*yelo_start_index = g_external_globals_yelo_start_index;

				return g_external_globals;
			}

			static hs_function_definition* GetFunction(int16 function_index);
			static hs_global_definition* GetGlobal(int16 global_index);

			static hs_function_definition* GetYeloFunction(int16 function_index);
			static hs_global_definition* GetYeloGlobal(int16 function_index);

			static void RegisterFunction(hs_function_definition& definition);
			static void RegisterGlobal(hs_global_definition& definition);

		private:
			static void RegisterFunctionFixup(s_hs_library_fixup& fixup);
			static void RegisterGlobalFixup(s_hs_library_fixup& fixup);
		public:
			template<rsize_t k_array_size>
			static void RegisterFunctionFixups(s_hs_library_fixup (&fixups)[k_array_size])
			{
				for (rsize_t x = 0; x < count; x++)
				{
					RegisterFunctionFixup(fixups[x]);
				}
			}
			template<rsize_t k_array_size>
			static void RegisterGlobalFixups(s_hs_library_fixup (&fixups)[k_array_size])
			{
				for (rsize_t x = 0; x < count; x++)
				{
					RegisterGlobalFixup(fixups[x]);
				}
			}
		};
	};
};