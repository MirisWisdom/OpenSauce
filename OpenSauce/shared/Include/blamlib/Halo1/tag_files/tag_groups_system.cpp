/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
// NOTE: NON-STANDARD ENGINE SOURCE FILE
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/tag_files/tag_groups.hpp>

#include <blamlib/Halo1/memory/byte_swapping.hpp>
#include <YeloLib/Halo1/tag_files/string_id_yelo.hpp>

#include "Engine/EngineFunctions.hpp"

namespace Yelo
{
	namespace TagGroups
	{
		tag_instance_data_t** TagInstanceData(); // forward declaration, defined in CheApe

		//////////////////////////////////////////////////////////////////////////
		// tag group memory
		void build_group_runtime_info();
		void destroy_group_runtime_info();
	};

	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// verify definitions
		static void verify_tag_group_tags()
		{
			struct verify_no_duplicate_group_tags_action
			{
				TagGroups::group_tag_to_string m_group_string;

				verify_no_duplicate_group_tags_action()
				{
					m_group_string.Terminate();
				}

				void operator()(tag_group* group)
				{
					m_group_string.group = group->group_tag;
					YELO_ASSERT_DISPLAY(group==tag_group_get(group->group_tag), "there are two groups using the tag '%s'.",
						m_group_string.TagSwap().str);
				}
			};

			TagGroups::tag_groups_do_action<verify_no_duplicate_group_tags_action>();
		}

		static void verify_string_list_definition(const string_list* definition,
			const tag_block_definition* block_definition, cstring field_type_name)
		{
			YELO_ASSERT_DISPLAY(definition, "no definition specified for %s field in block %s.",
				field_type_name, block_definition->name); // NOTE: added owner block name to info
			YELO_ASSERT( definition->count>=0 );

			for(int x = 0; x < definition->count; x++)
			{
				if(definition->strings[x] == nullptr)
				{
					YELO_ERROR(_error_message_priority_assert, "%s field in block %s doesn't have enough strings",
						field_type_name, block_definition->name);
				}
			}
		}
		template<typename TEnum>
		static void verify_enum_field_definition(const tag_field& field,
			const tag_block_definition* block_definition)
		{
			auto* definition = field.Definition<string_list>();
			verify_string_list_definition(definition, block_definition, "enum");
		}
		template<typename TFlags>
		static void verify_flags_field_definition(const tag_field& field,
			const tag_block_definition* block_definition)
		{
			auto* definition = field.Definition<string_list>();
			verify_string_list_definition(definition, block_definition, "flags");

			YELO_ASSERT( definition->count<=BIT_COUNT(TFlags) );
		}
		static void verify_tag_reference_field_definition(const tag_field& field,
			const tag_block_definition* block_definition)
		{
			auto* definition = field.Definition<tag_reference_definition>();
			YELO_ASSERT_DISPLAY(definition, "no definition specified for tag reference field in block %s.",
				block_definition->name); // NOTE: added owner block name to info
			YELO_ASSERT( VALID_FLAGS(definition->flags, Flags::k_number_of_tag_group_tag_reference_flags) );

			if(definition->group_tag != NONE)
			{
				YELO_ASSERT( tag_group_get(definition->group_tag) );
				YELO_ASSERT( definition->group_tags==nullptr );
			}
			else if(definition->group_tags != nullptr)
			{
				for(auto group_tag : *definition)
				{
					YELO_ASSERT( tag_group_get(group_tag) );
				}
			}
		}
		static void verify_data_field_definition(const tag_field& field,
			const tag_block_definition* block_definition)
		{
			auto* definition = field.Definition<tag_data_definition>();
			YELO_ASSERT_DISPLAY(definition, "no definition specified for tag_data field in block %s.",
				block_definition->name); // NOTE: added owner block name to info
			YELO_ASSERT( definition->name );
			YELO_ASSERT( VALID_FLAGS(definition->flags, Flags::k_number_of_tag_data_definition_flags) );
			YELO_ASSERT( definition->maximum_size>0 );
		}
		static void verify_block_field_definitions(tag_block_definition* block)
		{
			YELO_ASSERT( block );
			YELO_ASSERT( block->name );
			YELO_ASSERT( VALID_FLAGS(block->flags, Flags::k_number_of_tag_block_definition_flags) );
			YELO_ASSERT( block->maximum_element_count>=0 );
			YELO_ASSERT( CAST(int,block->element_size)>=0 );
			YELO_ASSERT( block->fields );

			struct verify_tag_field_action
			{ void operator()(const tag_block_definition* block, const tag_field& field) const
			{
				switch(field.type)
				{
				case Enums::_field_string:
					{
						uintptr_t definition = CAST_PTR(uintptr_t, field.definition);

						YELO_ASSERT( definition==0 || definition <= Enums::k_long_string_length || 
							TagGroups::TagFieldIsOldStringId(&field) );
					}
					break;

				case Enums::_field_enum:
					verify_enum_field_definition<int16>(field, block);
					break;

				case Enums::_field_long_flags:	verify_flags_field_definition<long_flags>(field, block); break;
				case Enums::_field_word_flags:	verify_flags_field_definition<word_flags>(field, block); break;
				case Enums::_field_byte_flags:	verify_flags_field_definition<byte_flags>(field, block); break;

				case Enums::_field_tag_reference:
					verify_tag_reference_field_definition(field, block);
					break;

				case Enums::_field_block:
					{
						auto* definition = field.Definition<tag_block_definition>();
						YELO_ASSERT_DISPLAY(definition, "no definition specified for block field in block %s.",
							block->name); // NOTE: added owner block name to info

						verify_block_field_definitions(definition);
					}
					break;

				case Enums::_field_short_block_index:
				case Enums::_field_long_block_index:
					{
						auto* definition = field.Definition<tag_block_definition>();
						YELO_ASSERT_DISPLAY(definition, "no definition specified for block index field in block %s.",
							block->name); // NOTE: added owner block name to info
					}
					break;

				case Enums::_field_array_start:
					YELO_ASSERT( CAST_PTR(int32,field.definition)>0 );
					break;

				case Enums::_field_explanation:
					{
						cstring definition = field.Definition<const char>();
						YELO_ASSERT_DISPLAY(definition, "no definition specified for explanation field in block %s.",
							block->name); // NOTE: added owner block name to info
					}
					break;
				}
			} };

			block->FieldsDoAction<verify_tag_field_action, true>();
		}
		static void verify_group_field_definitions()
		{
			struct verify_group_field_definitions_action
			{
				bool m_found_group_missing_header_definition;

				verify_group_field_definitions_action() : m_found_group_missing_header_definition(false) { }

				void operator()(const tag_group* group)
				{
					if(group->header_block_definition != nullptr)
						verify_block_field_definitions(group->header_block_definition);
					else
					{	// NOTE: added this warning
						YELO_WARN("tag group '%s' doesn't have a definition", group->name);
						m_found_group_missing_header_definition = true;
					}
				}
			};

			auto action = verify_group_field_definitions_action();
			TagGroups::tag_groups_do_action(action);

			if(action.m_found_group_missing_header_definition)
			{
				YELO_ASSERT( !"fix your goddamn tag groups" );
			}
		}


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
					//YELO_ASSERT( parent_group->header_block_definition ); // don't assert here as this is checked

					auto scanner = TagGroups::c_tag_field_scanner(parent_group->header_block_definition->fields)
						.ScanToEnd();

					total_field_count += 
						tag_chain[tag_chain_size].field_count = scanner.GetFieldIndex() - 1; // don't count the terminator
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

			block_definition->byte_swap_codes = YELO_NEW_ARRAY(byte_swap_code_t, code_index);
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

				YELO_DELETE_ARRAY(block_definition->byte_swap_codes);
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
			verify_tag_group_tags();
			TagGroups::InitializeFieldSetReplacements();
			verify_group_field_definitions();
			TagGroups::InitializeFixes();
			build_group_parents();
			build_group_byte_swap_codes();
			TagGroups::build_group_runtime_info();

			auto instance_data = CAST_PTR(Memory::s_data_array**, TagGroups::TagInstanceData());
			*instance_data = data_new<s_tag_instance, Enums::k_maximum_simultaneous_tag_instances_upgrade>("tag instance");
			if( *instance_data == nullptr )
			{
				tag_groups_dispose();
				YELO_ERROR(_error_message_priority_none, "couldn't allocate tag group globals");
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

			TagGroups::destroy_group_runtime_info();
			destroy_group_byte_swap_codes();
			destroy_group_parents();
		}
	};
};