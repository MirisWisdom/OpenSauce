/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>
#include <YeloLib/Halo1/tag_files/string_id_yelo.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum tag_allocation_type : _enum;
	};

	namespace TagGroups
	{
		class c_cache_file_memory_layout_table;

		class c_cache_file_memory_gestalt
		{
		public:
			enum { k_group_tag = 'memg' };

			struct s_tag_allocation_definition
			{
				typedef int16 block_index_t;

				TAG_ENUM(type, Enums::tag_allocation_type);
				PAD16;
				TAG_FIELD(size_t, name_offset);

				void* definition_code_address;
			};
			struct s_tag_allocation
			{
				PAD32;
				byte checksum_buffer[20];
			};

			struct s_tag_instance
			{
				TAG_FIELD(size_t, name_offset);
				TAG_FIELD(tag, group_tag);
				TAG_FIELD(uint32, tag_file_checksum);
				TAG_FIELD(uint32, modified_date);
			};

			TAG_FIELD(tag_reference, layout_reference);
			TAG_FIELD(tag_reference, scenario_reference);

			TAG_FIELD(tag_string, cache_name);
			TAG_FIELD(uint32, cache_build_date);
			PAD32; PAD32; PAD32;

			TAG_FIELD(TagData<char>, definition_names);
			TAG_FIELD(TagData<char>, tag_names);

			TAG_TBLOCK(allocation_definitions, s_tag_allocation_definition);
			TAG_TBLOCK(allocations, s_tag_allocation);

			TAG_TBLOCK(tag_instances, s_tag_instance);
		};
	};
};