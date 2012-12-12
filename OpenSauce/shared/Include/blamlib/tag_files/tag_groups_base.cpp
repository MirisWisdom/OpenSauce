/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/tag_files/tag_groups_base.hpp>

#include "TagGroups/TagGroups.hpp" // defined in the project implementation

namespace Yelo
{
	void tag_reference::clear()
	{
		tag_reference_clear(*this);
	}

	void tag_reference::set(tag group_tag, cstring name)
	{
		tag_reference_set(*this, group_tag, name);
	}

	void* tag_block::get_element(int32 element)
	{
		return tag_block_get_element(this, element);
	}

	void tag_block::delete_element(int32 element)
	{
		tag_block_delete_element(this, element);
	}

	int32 tag_block::add_element()
	{
		return tag_block_add_element(this);
	}

	bool tag_block::resize(int32 element_count)
	{
		return tag_block_resize(this, element_count);
	}

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

		void group_tag_to_string::Terminate()	{ str[4] = '\0'; }
		void group_tag_to_string::TagSwap()		{ TagGroups::TagSwap(group); }
	};
};