/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

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

		s_hs_runtime_globals*		HsRuntimeGlobals();
	};

	namespace blam
	{
		void hs_runtime_update();

		void hs_return(datum_index thread_index, TypeHolder value);

		bool hs_can_cast(Enums::hs_type actual_type, Enums::hs_type desired_type);

		TypeHolder hs_cast(datum_index thread_index, Enums::hs_type actual_type, Enums::hs_type desired_type, TypeHolder value);


		Scripting::s_hs_thread_datum* hs_thread_get(datum_index thread_index);
	};
};