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
		enum {
			k_maximum_number_of_hs_globals = 1024,
		};
	};

	namespace Scripting
	{
		struct s_hs_thread_datum;
		struct s_hs_globals_datum;

		typedef Memory::DataArray<	s_hs_thread_datum, 
									256>
			hs_thread_data_t;

		typedef Memory::DataArray<	s_hs_globals_datum, 
									Enums::k_maximum_number_of_hs_globals> 
			hs_globals_data_t;
	};

	namespace blam
	{
		Scripting::s_hs_thread_datum* hs_thread_get(datum_index thread_index);
	};
};