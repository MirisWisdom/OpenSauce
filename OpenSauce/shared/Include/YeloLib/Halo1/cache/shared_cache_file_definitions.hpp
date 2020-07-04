/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct s_shared_cache_file_definition
		{
#if !PLATFORM_IS_EDITOR
			const 
#endif
				int16 version;	enum { k_version = 1, k_group_tag = 'cher' };
			struct _flags {
				TAG_FLAG16(unused);
			}flags; BOOST_STATIC_ASSERT( sizeof(_flags) == sizeof(word_flags) );
			TAG_FIELD(uint32, base_address);
			TAG_FIELD(int32, starting_tag_index);
			PAD32;

			TAG_FIELD(tag_reference, explicit_references, 'tagc');
		};
	};
};