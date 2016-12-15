/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/hs/hs.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum hs_type : _enum;

		enum {
			k_maximum_number_of_hs_globals = 1024,

			k_hs_inspect_buffer_size = 1024,
		};
	};

	namespace Scripting
	{
		struct s_hs_thread_datum;
		struct s_hs_globals_datum;
		struct s_hs_runtime_globals;

		typedef Memory::DataArray<	s_hs_thread_datum, 
									256>
			hs_thread_data_t;

		typedef Memory::DataArray<	s_hs_globals_datum, 
									Enums::k_maximum_number_of_hs_globals> 
			hs_globals_data_t;


		hs_thread_data_t&			HsThreads();
		hs_globals_data_t&			HsGlobals();
		s_hs_runtime_globals*		HsRuntimeGlobals();
	};

	namespace blam
	{
		void hs_evaluate(datum_index thread_index, datum_index expression_index, Scripting::s_hs_value_union* destination);
		void hs_call_evaluate(datum_index thread_index);


		void hs_runtime_update();

		Scripting::s_hs_value_union* hs_macro_function_evaluate(int16 function_index, datum_index thread_index, bool initialize_stack);

		void hs_return(datum_index thread_index, Scripting::s_hs_value_union value);

		bool hs_can_cast(Enums::hs_type actual_type, Enums::hs_type desired_type);

		Scripting::s_hs_value_union hs_cast(datum_index thread_index, Enums::hs_type actual_type, Enums::hs_type desired_type, Scripting::s_hs_value_union value);

		Scripting::s_hs_value_union hs_runtime_evaluate(datum_index expression_index,
			datum_index script_container_index = datum_index::null); // TODO: remove default parameter when containers come online

		datum_index hs_find_thread_by_script(Scripting::hs_script_index_t script);


		Scripting::s_hs_thread_datum* hs_thread_get(datum_index thread_index);
	};
};