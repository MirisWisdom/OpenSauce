/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
// NOTE: NON-STANDARD ENGINE SOURCE FILE
#include "Common/Precompile.hpp"
#if PLATFORM_IS_EDITOR
#include <blamlib/Halo1/tag_files/tag_groups.hpp>

#include <blamlib/Halo1/memory/byte_swapping.hpp>
#include <blamlib/Halo1/tag_files/tag_field_scanner.hpp>
#include <blamlib/Halo1/tag_files/tag_group_verification.hpp>
#include <YeloLib/memory/memory_interface_base.hpp>
#include <YeloLib/Halo1/tag_files/string_id_yelo.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		tag_instance_data_t** TagInstanceData(); // forward declaration, defined in CheApe

		//////////////////////////////////////////////////////////////////////////
		// tag group memory
		void BuildGroupRuntimeInfo();
		void DestroyGroupRuntimeInfo();
	};

	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// build group parents
		static void build_group_parents()
		{
			struct build_child_group_tags_action
			{ void operator()(tag_group* group) const
			{
				for(tag_group* parent_group = tag_group_get(group->parent_group_tag); parent_group; parent_group = tag_group_get(parent_group->parent_group_tag))
				{
					YELO_ASSERT( parent_group->child_count<Enums::k_maximum_children_per_tag );
					parent_group->child_group_tags[parent_group->child_count++] = group->group_tag;
				}
			} };

			TagGroups::tag_groups_do_action<build_child_group_tags_action>();

			struct build_child_groups_fields_action
			{ void operator()(tag_group* group) const
			{
				if(group->parent_group_tag == NONE || group->child_count != 0)
					return;

				// chain starts with the specific group, then increments to the last parent tag group
				struct {
					const tag_group* group;
					int32 field_count;
				}tag_chain[Enums::k_maximum_tags_per_tag_chain];
				int32 tag_chain_size = 0;
				int32 total_field_count = 0;
				size_t total_header_size = 0;

				const tag_group* parent_group = group;
				do {
					//YELO_ASSERT( parent_group->header_block_definition ); // don't assert here as this is checked in VerifyGroupFieldDefinitions

					auto scanner = TagGroups::c_tag_field_scanner(parent_group->header_block_definition->fields)
						.ScanToEnd();

					total_field_count += 
						tag_chain[tag_chain_size].field_count = scanner.GetFieldCount() - 1; // don't count the terminator
					total_header_size += 
						parent_group->header_block_definition->element_size;
					tag_chain[tag_chain_size].group = parent_group;

					tag_chain_size++;
					YELO_ASSERT( tag_chain_size<=Enums::k_maximum_tags_per_tag_chain );

				} while(parent_group = tag_group_get(parent_group->parent_group_tag));

				// +1 for our terminator
				tag_field* combined_fields = YELO_NEW_ARRAY(tag_field, total_field_count+1);
				if(combined_fields == nullptr)
					return; // TODO: this is what the engine does for out of memory...

				// copy the fields from the highest to lowest link in the chain
				tag_field* next_combined_field = combined_fields;
				for(int x = tag_chain_size-1; x >= 0; x--)
				{
					auto& link = tag_chain[x];

					std::memcpy(next_combined_field,
						link.group->header_block_definition->fields, sizeof(tag_field) * link.field_count);

					next_combined_field += link.field_count;
				}
				YELO_ASSERT( (next_combined_field-combined_fields)==total_field_count );

				memset(next_combined_field, 0, sizeof(*next_combined_field));
				next_combined_field->type = Enums::_field_terminator;

				group->header_block_definition->fields = combined_fields;
				group->header_block_definition->element_size = total_header_size;
			} };

			TagGroups::tag_groups_do_action<build_child_groups_fields_action>();
		}

		static void destroy_group_parents()
		{
			struct destroy_group_parents_action
			{ void operator()(tag_group* group) const
			{
				if(group->parent_group_tag != NONE && group->child_count == 0)
				{
					YELO_DELETE_ARRAY(group->header_block_definition->fields);
					group->header_block_definition->fields = nullptr; // NOTE: engine didn't null this
				}
			} };

			TagGroups::tag_groups_do_action<destroy_group_parents_action>();
		}


		//////////////////////////////////////////////////////////////////////////
		// build byte swap codes
		static void build_byte_swap_codes_for_block_definition(tag_block_definition* block_definition)
		{
			if(block_definition->byte_swap_codes != nullptr)
				return;

			const tag_field* fields = block_definition->fields;
			int field_index = 0;

			// NOTE: we don't assert code_index since we're using std::array
			std::array<byte_swap_code_t, Enums::k_maximum_field_byte_swap_codes> codes = 
				{Enums::_bs_code_array_start, 1};
			int code_index = 2;

			bool has_children = false;

			do {
				const tag_field& field = fields[field_index];
				YELO_ASSERT( field.type>=0 && field.type<Enums::k_number_of_tag_field_types );

				for(byte_swap_code_t* field_codes = TagGroups::k_tag_field_definitions[field.type].byte_swap_codes;
					*field_codes != 0;
					field_codes++, code_index++)
				{
					codes[code_index] = *field_codes;
				}

				switch (field.type)
				{
				case Enums::_field_string: // NOTE: non-standard; added to support variable length string fields
					codes[code_index-1] = CAST(byte_swap_code_t, TagGroups::StringFieldGetSize(&field));
					break;

				case Enums::_field_array_start:
				case Enums::_field_pad:
				case Enums::_field_skip:
					codes[code_index++] = field.DefinitionCast<int32>();
					break;

				case Enums::_field_block:
					// NOTE: this is already checked in verify_group_field_definitions
					//YELO_ASSERT_DISPLAY(field.definition, "element '%s' of block '%s' is NULL",
					//	field.name, block_definition->name);

					YELO_ASSERT(field.definition != block_definition); // a block shouldn't be able to have a child of itself, that would be infinite recursion

					build_byte_swap_codes_for_block_definition(field.Definition<tag_block_definition>());
					has_children = true;
					break;

				case Enums::_field_tag_reference:
				case Enums::_field_data:
					has_children = true;
					break;
				}

			} while(fields[field_index++].type != Enums::_field_terminator);

			if(has_children)
				SET_FLAG(block_definition->flags, Flags::_tag_block_has_children_bit, true);

			block_definition->byte_swap_codes = new byte_swap_code_t[code_index];
			if(!block_definition->byte_swap_codes)
				YELO_WARN("couldn't allocate memory for tag block definition byte swapping codes (%s)",
					block_definition->name); // NOTE: added name to info

			std::memcpy(block_definition->byte_swap_codes, 
				codes.data(), sizeof(codes[0]) * code_index);

			byte_swap_data_explicit(block_definition->name, block_definition->element_size, block_definition->byte_swap_codes);
		}
		static void build_group_byte_swap_codes()
		{
			struct build_byte_swap_codes_action
			{ void operator()(tag_block_definition* header_block_definition) const
			{
				build_byte_swap_codes_for_block_definition(header_block_definition);
			} };

			TagGroups::tag_groups_do_header_block_definition_action<build_byte_swap_codes_action>();
		}

		static void destroy_group_byte_swap_codes()
		{
			struct destroy_byte_swap_codes_action
			{ void operator()(tag_block_definition* block_definition) const
			{
				if(block_definition->byte_swap_codes == nullptr)
					return;

				delete block_definition->byte_swap_codes;
				block_definition->byte_swap_codes = nullptr;
			} void operator()(tag_group* group) const
			{
				group->header_block_definition->DoRecursiveAction(*this);
			} };

			TagGroups::tag_groups_do_action<destroy_byte_swap_codes_action>();
		}


		//////////////////////////////////////////////////////////////////////////
		// system definitions

		void PLATFORM_API tag_groups_initialize()
		{
			TagGroups::VerifyTagGroupTags();

			TagGroups::InitializeFieldSetReplacements();
			TagGroups::InitializeFixes();

			TagGroups::VerifyGroupFieldDefinitions();
			build_group_parents();
			build_group_byte_swap_codes();

			TagGroups::VerifyTagGroupsFinalChecks();
			TagGroups::CheckForUnreferencedGroups();
			TagGroups::BuildGroupRuntimeInfo();

			auto instance_data = CAST_PTR(Memory::s_data_array**, TagGroups::TagInstanceData());
			*instance_data = data_new<s_tag_instance, Enums::k_maximum_simultaneous_tag_instances_upgrade>("tag instance");
			if( *instance_data == nullptr )
			{
				tag_groups_dispose();
				YELO_ERROR_OUT_OF_MEMORY("couldn't allocate tag group globals");
				return;
			}

			 TagGroups::TagInstances().MakeValid();
		}

		void PLATFORM_API tag_groups_dispose()
		{
			if( *TagGroups::TagInstanceData() != nullptr )
			{
				for(auto kv : TagGroups::TagInstances())
					tag_unload(kv.index);

				TagGroups::TagInstances().MakeInvalid();
				TagGroups::TagInstances().Dispose();

				*TagGroups::TagInstanceData() = nullptr;
			}

			TagGroups::DestroyGroupRuntimeInfo();
			destroy_group_byte_swap_codes();
			destroy_group_parents();
		}

		void PLATFORM_API tag_groups_initialize_for_new_map()
		{
		}

		void PLATFORM_API tag_groups_dispose_from_old_map()
		{
			for (auto kv : TagGroups::TagInstances())
				tag_unload(kv.index);
		}

		uint32 PLATFORM_API tag_groups_checksum()
		{
			uint32 crc;
			crc_new(crc);

			for (auto kv : TagGroups::TagInstances())
			{
				if (kv->is_orphan ||
					!TEST_FLAG(tag_group_get(kv->group_tag)->flags, Flags::_tag_group_include_in_tags_checksum_bit))
					continue;

				crc_checksum_buffer(crc, &kv->file_checksum, sizeof(kv->file_checksum));
			}

			return crc;
		}
	};
};

#endif