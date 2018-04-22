/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blam/hs/hs.hpp>

namespace Yelo
{
	namespace Scripting
	{
		typedef s_hs_value_union (API_FUNC* proc_hs_typecast)(s_hs_value_union value);
		typedef bool (API_FUNC* proc_hs_type_parse)(Enums::hs_type type, datum_index expression_index);
		typedef void (API_FUNC* proc_hs_type_inspect)(Enums::hs_type type, s_hs_value_union value, char* buffer, size_t buffer_size);

		extern proc_hs_typecast k_hs_typecasting_procedures[Enums::k_number_of_hs_types][Enums::k_number_of_hs_types];
		void HsTypesInitialize();
		void HsTypesDispose();
	};
};