/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include <blam/blam.h>
#include <blam/tag_files/tag_groups_base.hpp>

#include <blam/tag_files/tag_block.h>

namespace Yelo
{

	namespace TagGroups
	{
		// just an endian swap
		void TagSwap(tag& x)
		{
			x = (x>>24) | 
				((x<<8) & 0x00FF0000) |
				((x>>8) & 0x0000FF00) |
				(x<<24);
		}

		tag string_to_group_tag(cstring name)
		{
			const uint32 _group = *((const uint32*)name);
			return (tag) ( (_group>>24) | ((_group>>8)& 0xFF00) | (((_group<<8)&0xFF0000) | ((_group<<24)&0xFF000000)) );
		}

		group_tag_to_string& group_tag_to_string::Terminate()	{ str[4] = '\0';			return *this; }
		group_tag_to_string& group_tag_to_string::TagSwap()		{ TagGroups::TagSwap(group);return *this; }
	};

	namespace blam
	{
#if PLATFORM_IS_EDITOR
		void* tag_block_add_and_get_element(tag_block* block)
		{
			int32 index = tag_block_add_element(block);
			return tag_block_get_element(block, index);
		}
#endif
	};
};