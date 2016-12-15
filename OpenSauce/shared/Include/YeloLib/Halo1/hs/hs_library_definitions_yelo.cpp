/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/hs/hs_library_definitions_yelo.hpp>

namespace Yelo
{
	namespace Scripting
	{
		size_t c_hs_library::g_functions_yelo_start_index;
		size_t c_hs_library::g_functions_next_register_index;
		std::vector<hs_function_definition*> c_hs_library::g_functions(Enums::k_hs_script_functions_count_upgrade);

		size_t c_hs_library::g_external_globals_yelo_start_index;
		size_t c_hs_library::g_external_globals_next_register_index;
		std::vector<hs_global_definition*> c_hs_library::g_external_globals(Enums::k_hs_external_globals_count_upgrade);


		void c_hs_library::Initialize()
		{
		}
		void c_hs_library::Dispose()
		{
		}

		hs_function_definition* c_hs_library::GetFunction(int16 function_index)
		{
			YELO_ASSERT(function_index>=0 && (size_t)function_index<g_functions.size());

			auto* hs_function = g_functions[function_index];

			YELO_ASSERT(hs_function);
			return hs_function;
		}
		hs_global_definition* c_hs_library::GetGlobal(int16 global_index)
		{
			YELO_ASSERT(global_index>=0 && (size_t)global_index<g_external_globals.size());

			auto* hs_global = g_external_globals[global_index];

			YELO_ASSERT(hs_global);
			return hs_global;
		}

		hs_function_definition* c_hs_library::GetYeloFunction(int16 function_index)
		{
			YELO_ASSERT(function_index>=0);

			return GetFunction(g_functions_yelo_start_index+function_index);
		}
		hs_global_definition* c_hs_library::GetYeloGlobal(int16 global_index)
		{
			YELO_ASSERT(global_index>=0);

			return GetGlobal(g_external_globals_yelo_start_index+global_index);
		}

		template<typename T>
		static void RegisterLibraryElement(std::vector<T*>& vector, size_t& next_register_index, T& element)
		{
			// since we support 'fixups' that map directly to a specific index, we first have to try and find an
			// unused slot we can put the desired element at.
			if (next_register_index < vector.size())
			{
				bool added = false;
				for (auto iter = vector.begin()+next_register_index, end = vector.end();
					 !added && iter != end;
					 ++iter, ++next_register_index)
				{
					if (*iter != nullptr)
						continue;

					added = true;
					*iter = &element;
				}

				// we found an unused slot and added it there, we can return.
				// else, there are no more free slots, and have to go back to regular 'push_back' adding
				if (added)
					return;
			}

			// we should only be adding when there are no more free slots in the vector
			YELO_ASSERT(next_register_index == vector.size());

			vector.push_back(&element);
			next_register_index++;
		}
		void c_hs_library::RegisterFunction(hs_function_definition& definition)
		{
			RegisterLibraryElement(g_functions, g_functions_next_register_index,
				definition);
		}
		void c_hs_library::RegisterGlobal(hs_global_definition& definition)
		{
			YELO_ASSERT(g_external_globals.size() < Enums::k_hs_external_globals_count_upgrade);

			RegisterLibraryElement(g_external_globals, g_external_globals_next_register_index, 
				definition);
		}

		template<typename T>
		static void RegisterLibraryFixup(std::vector<T*>& vector, size_t fixup_index, T* element)
		{
			YELO_ASSERT(element);

			if (fixup_index >= vector.size())
			{
				size_t new_size = vector.size();
				new_size += (vector.size() - fixup_index) + 1;
				vector.resize(new_size);
			}

			vector[fixup_index] = element;
		}
		void c_hs_library::RegisterFunctionFixup(s_hs_library_fixup& fixup)
		{
			RegisterLibraryFixup(g_functions, fixup.index,
				fixup.function);
		}
		void c_hs_library::RegisterGlobalFixup(s_hs_library_fixup& fixup)
		{
			RegisterLibraryFixup(g_external_globals, fixup.index,
				fixup.global);
		}
	};
};