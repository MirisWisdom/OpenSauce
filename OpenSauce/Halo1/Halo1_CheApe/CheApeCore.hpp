/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

namespace Yelo
{
	struct tag_group;

	namespace CheApe
	{
		struct s_custom_tag_group_datum : Memory::s_datum_base
		{
			uint16 flags;
			tag_group* definition;
		}; BOOST_STATIC_ASSERT( sizeof(s_custom_tag_group_datum) == 0x8 );
		typedef Memory::DataArray<s_custom_tag_group_datum, Enums::k_max_number_of_new_tag_groups>
			custom_tag_group_data_t;
	};
};