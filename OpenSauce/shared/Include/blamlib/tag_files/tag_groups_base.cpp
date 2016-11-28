/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/tag_files/tag_groups_base.hpp>

namespace Yelo
{
#if PLATFORM_IS_EDITOR
	void tag_reference::clear()
	{
		blam::tag_reference_clear(*this);
	}

	void tag_reference::set(tag group_tag, cstring name)
	{
		blam::tag_reference_set(*this, group_tag, name);
	}

	void* tag_block::get_element(int32 element)
	{
		return blam::tag_block_get_element(this, element);
	}

	void tag_block::delete_element(int32 element)
	{
		blam::tag_block_delete_element(this, element);
	}

	int32 tag_block::add_element()
	{
		return blam::tag_block_add_element(this);
	}

	bool tag_block::resize(int32 element_count)
	{
		return blam::tag_block_resize(this, element_count);
	}

	void* tag_block::add_and_get_element()
	{
		return blam::tag_block_add_and_get_element(this);
	}

	bool tag_data::resize(int32 new_size)
	{
		return blam::tag_data_resize(this, new_size);
	}
#endif

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