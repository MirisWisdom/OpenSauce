/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

namespace Yelo
{
	namespace TagGroups
	{
		struct s_tag_reference
		{
			TAG_FIELD(tag_reference, tag);
		}; ASSERT_SIZE(s_tag_reference, 0x10); // max count: 200

		struct s_tag_collection_definition
		{
			enum { k_group_tag = 'tagc' };

			TAG_TBLOCK(tag_references, s_tag_reference);
		}; ASSERT_SIZE(s_tag_collection_definition, 0xC); // max count: 1
	};
};
