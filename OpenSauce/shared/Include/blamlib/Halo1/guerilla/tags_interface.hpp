/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <blamlib/Halo1/tag_files/tag_groups_structures.hpp>

namespace Yelo
{
	class field_information;

	class tag_information
	{
	public:
		void* owner;
		datum_index tag_index;
		field_information* fields;
		tag_block* root_block;
		tag group_tag;
	}; BOOST_STATIC_ASSERT( sizeof(tag_information) == 0x14 );

	class field_information
	{
		void** vftable;
	public:
		UNUSED_TYPE(int32);
		tag_field* field;
		void* address;
		tag_information* tag_info;
		field_information* next_field;
		field_information* parent_field;
	}; BOOST_STATIC_ASSERT( sizeof(field_information) == 0x1C );
	class block_information : public field_information
	{
	public:
		int32 block_index;
		field_information* first_child_field;
		field_information* block_name_field;
	}; BOOST_STATIC_ASSERT( sizeof(block_information) == 0x28 );
};