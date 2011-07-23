/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\Halo2_CheApe for specific license information
*/
#pragma once

#include <Memory/Halo2/DataShared.hpp>

namespace Yelo
{
	struct tag_group_definition;

	namespace CheApe
	{
		struct s_custom_tag_group_datum : Memory::s_datum_base
		{
			uint16 flags;
			tag_group_definition* definition;
		}; BOOST_STATIC_ASSERT( sizeof(s_custom_tag_group_datum) == 0x8 );
		typedef Memory::DataArray<s_custom_tag_group_datum, Enums::k_max_number_of_new_tag_groups> t_custom_tag_group_data;
	};
};