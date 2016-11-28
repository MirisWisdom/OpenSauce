/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/hs/hs.hpp>

#include <blamlib/Halo1/hs/hs_runtime.hpp>
#include <blamlib/Halo1/hs/hs_structures.hpp>
#include <blamlib/Halo1/scenario/scenario.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <YeloLib/Halo1/hs/hs_library_definitions_yelo.hpp>

namespace Yelo
{
	namespace Scripting
	{
		s_hs_script_container_datum_index s_hs_script_container_datum_index::CreateIndex(int16 index,
			int16 script_container_index)
		{
			return { 
				index,
				//script_container_index // TODO: uncomment when containers come online
			};
		}

		const s_hs_script_container_datum_index s_hs_script_container_datum_index::k_null = {
			NONE,
			//NONE // TODO: uncomment when containers come online
		};

		const s_hs_value_union k_null_value_union = { NULL };
		const s_hs_value_union k_none_value_union = { NONE };
	};

	namespace blam
	{
		using namespace Scripting;

		void hs_nodes_garbage_collect()
		{
			// NOTE: engine seems to use a for(;;) using data_next_index() on syntax nodes
			for (auto node : HsSyntax())
			{
				if (node->IsGarbageCollectable())
					continue;

				HsSyntax().Delete(node.index);
			}
		}

		hs_function_definition* hs_function_get(int16 function_index)
		{
			return c_hs_library::GetFunction(function_index);
		}
		hs_global_definition* hs_global_external_get(int16 global_index)
		{
			return c_hs_library::GetGlobal(global_index);
		}

		Enums::hs_type hs_global_get_type(int16 global_index)
		{
			YELO_ASSERT(global_index != NONE); // engine doesn't do this

			bool is_internal = (global_index & Enums::_hs_global_index_is_external_mask) == 0;
			global_index &= Enums::_hs_global_index_mask;

			if (is_internal)
			{
				return global_scenario_get()->globals[global_index].global.type;
			}

			return hs_global_external_get(global_index)->type;
		}
		cstring hs_global_get_name(int16 global_index)
		{
			YELO_ASSERT(global_index != NONE); // engine doesn't do this

			bool is_internal = (global_index & Enums::_hs_global_index_is_external_mask) == 0;
			global_index &= Enums::_hs_global_index_mask;

			if (is_internal)
			{
				return global_scenario_get()->globals[global_index].name;
			}

			return hs_global_external_get(global_index)->name;
		}

		int16 hs_find_global_by_name(cstring name)
		{
			// TODO: update code when containers come online

			// search the globals external from the scenario first
			int32 global_index = 0;
			for (auto* external_global : c_hs_library::GetExternalGlobals())
			{
				if (!_stricmp(name, external_global->name))
				{
					global_index |= Enums::_hs_global_index_is_external_mask;
					// engine casts down as well, so we will too
					return CAST(int16, global_index);
				}

				global_index++;
			}

			auto* scenario = global_scenario_try_and_get();
			// if NULL, a scenario isn't currently loaded. engine actually checks that global_scenario_index isn't NONE
			if (scenario == nullptr)
				return NONE;

			// now search the globals internal to the scenario
			global_index = 0;
			for (auto& internal_global : scenario->globals)
			{
				if (!_stricmp(name, internal_global.name))
				{
					// engine casts down as well, so we will too
					return CAST(int16, global_index);
				}

				global_index++;
			}

			// no global by the given name was found
			return NONE;
		}

		int16 hs_find_function_by_name(cstring name)
		{
			int32 function_index = 0;
			for (auto* function : c_hs_library::GetFunctionsTable())
			{
				if (!_stricmp(name, function->name))
				{
					// engine casts down as well, so we will too
					return CAST(int16, function_index);
				}

				function_index++;
			}

			// no function by the given name was found
			return NONE;
		}

		int16 hs_find_tag_reference_by_index(datum_index tag_index)
		{
			// TODO: update code when containers come online
			
			auto* scenario = global_scenario_try_and_get();
			// if NULL, a scenario isn't currently loaded. engine actually checks that global_scenario_index isn't NONE
			if (scenario == nullptr)
				return NONE;

			int32 reference_index = 0;
			for (auto& reference : scenario->references)
			{
				if (reference.reference.tag_index == tag_index)
				{
					// engine casts down as well, so we will too
					return CAST(int16, reference_index);
				}

				reference_index++;
			}

			// no reference with the given tag_index was found
			return NONE;
		}

		hs_script_index_t hs_find_script_by_name(cstring name)
		{
			// TODO: update code when containers come online
			
			auto* scenario = global_scenario_try_and_get();
			// if NULL, a scenario isn't currently loaded. engine actually checks that global_scenario_index isn't NONE
			if (scenario == nullptr)
				return hs_script_index_t::k_null;

			int32 script_index = 0;
			for (auto& script : scenario->scripts)
			{
				if (!_stricmp(name, script.name))
				{
					return hs_script_index_t::CreateIndex(CAST(int16, script_index));
				}

				script_index++;
			}

			// no script by the given name was found
			return hs_script_index_t::k_null;
		}

		bool hs_evaluate_by_script_name(cstring name)
		{
			// TODO: update code when containers come online
			
			auto script = hs_find_script_by_name(name);
			if (script.IsNull())
				return false;

			auto* scenario = global_scenario_get();

			hs_runtime_evaluate(scenario->scripts[script.script_index].script.root_expression_index);
			return true;
		}


		hs_syntax_node* hs_syntax_get(datum_index expression_index)
		{
			return HsSyntax()[expression_index];
		}
	};
};