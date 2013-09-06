/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/tag_files/tag_group_loading.hpp>

#include <blamlib/Halo1/memory/byte_swapping.hpp>
#include <blamlib/Halo1/tag_files/tag_files.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>

#include "Engine/EngineFunctions.hpp"

namespace Yelo
{
	namespace blam
	{
		bool PLATFORM_API tag_data_load(void* block_element, tag_data* data, void* address)
		{
			YELO_ASSERT( block_element );
			YELO_ASSERT( data && data->definition );
			auto* data_definition = data->definition;

			if(address == nullptr)
			{
				YELO_ASSERT( data->address==nullptr );
				address = data->address = YELO_MALLOC(data->size, false);

				if(address == nullptr)
				{
					YELO_ERROR(_error_message_priority_warning, "couldn't allocate memory for #%d bytes %s data (load)",
						data->size, data_definition->name); // NOTE: added this warning

					return false;
				}
			}

			//YELO_ASSERT( data_definition ); // engine does this, but I don't see why

			if( !tag_file_read(data->stream_position, data->size, data->address) )
			{
				YELO_ERROR(_error_message_priority_warning, "couldn't read #%d bytes for %s data",
					data->size, data_definition->name); // NOTE: added this warning

				return false;
			}

			data_definition->byte_swap_proc(block_element, data->address, data->size);

			return true;
		}

		void PLATFORM_API tag_data_unload(void* block_element, tag_data* data)
		{
			YELO_ASSERT( data && data->definition );
			YELO_ASSERT( data->address );

			YELO_FREE( data->address );
			data->address = nullptr;
		}

		static bool PLATFORM_API tag_data_read_recursive(tag_data_definition* data_definition, void* block_element, tag_data* data, 
			int32 *position_reference, long_flags read_flags)
		{
			YELO_ASSERT( data_definition );
			void* data_address = nullptr;
			bool success = false;

			if( TEST_FLAG(data_definition->flags, Flags::_tag_data_never_streamed_bit)==0 ||
				TEST_FLAG(read_flags, Flags::_tag_load_for_editor_bit))
			{
				int size = data->size;
				if(size < 0 || size > data_definition->maximum_size)
				{
					YELO_ERROR(_error_message_priority_warning, "tag data '%s' too large. #%d not in [0,#%d]",
						data_definition->name, data->size, data_definition->maximum_size); // NOTE: added bounds info to warning
				}
				else if( !(data_address = YELO_MALLOC(size, false)) )
				{
					YELO_ERROR(_error_message_priority_warning, "couldn't allocate #%d tag data for '%s'",
						size, data_definition->name); // NOTE: added size info to warning
				}
				else if( !tag_data_load(block_element, data, data->address = data_address) )
				{
					// tag_data_load provides warnings, so we don't provide any here
					YELO_FREE( data->address );
					data_address = nullptr;
				}
				else
					success = true;
			}
			else
				success = true;

			*position_reference += data->size;
			data->address = data_address;
			return success;
		}

		static bool PLATFORM_API tag_reference_read_recursive(tag_reference_definition* definition, tag_reference* reference,
			int32 *position_reference, long_flags read_flags)
		{
			// NOTE: engine doesn't ASSERT anything

			if(reference->group_tag == 0)
				reference->group_tag = definition->group_tag;

			reference->tag_index = datum_index::null;

			if( !(reference->name = CAST_PTR(char*,YELO_MALLOC(Enums::k_max_tag_name_length+1, false))) )
			{
				YELO_ERROR(_error_message_priority_warning, "couldn't allocate name memory for tag_reference @%p",
					definition); // NOTE: added this warning

				return false;
			}

			reference->name[0] = '\0';
			int name_length = reference->name_length;
			if(name_length == 0)
			{
				return true;
			}
			else if(name_length < 0 || name_length > Enums::k_max_tag_name_length)
			{
				YELO_ERROR(_error_message_priority_warning, "tag reference name too large #%d (this tag is corrupted).",
					name_length); // NOTE: added length info to warning

				return false;
			}
			else if( !tag_file_read(*position_reference, name_length+1, reference->name) )
			{
				YELO_ERROR(_error_message_priority_warning, "couldn't read #%d characters for @%p tag reference name",
					name_length+1, definition); // NOTE: added this warning

				return false;
			}
			else if(strlen(reference->name) != name_length)
			{
				// NOTE: engine uses the 'too large' warning for this case
				YELO_ERROR(_error_message_priority_warning, "tag reference name @%X doesn't match length #%d (this tag is corrupted).",
					*position_reference, name_length); // NOTE: added length info to warning

				return false;
			}

			_strlwr(reference->name);
			*position_reference += name_length+1;
			return true;
		}

		bool PLATFORM_API tag_block_read_recursive(const tag_block_definition* definition, tag_block* block,
			int32 *position_reference, long_flags read_flags);
		bool PLATFORM_API tag_block_read_children_recursive(const tag_block_definition *definition, void *address, int32 count, 
			int32 *position_reference, long_flags read_flags)
		{
			bool success = true;
			if(count == 0)
				return success;

			for(int x = 0; x < count; x++)
			{
				TagGroups::c_tag_field_scanner scanner(definition->fields, 
					CAST_PTR(byte*,address) + (definition->element_size * x) );

				scanner	.AddFieldType(Enums::_field_block)
						.AddFieldType(Enums::_field_data)
						.AddFieldType(Enums::_field_tag_reference)
					;

				while(scanner.Scan())
				{
					bool read_result;

					switch(scanner.GetTagFieldType())
					{
					case Enums::_field_data: read_result = 
						tag_data_read_recursive(scanner.GetTagFieldDefinition<tag_data_definition>(), scanner.GetAddress(),
							scanner.GetFieldAs<tag_data>(), 
							position_reference, read_flags);
						break;

					case Enums::_field_block: read_result = 
						tag_block_read_recursive(scanner.GetTagFieldDefinition<tag_block_definition>(),
							scanner.GetFieldAs<tag_block>(), 
							position_reference, read_flags);
						break;

					case Enums::_field_tag_reference: read_result = 
						tag_reference_read_recursive(scanner.GetTagFieldDefinition<tag_reference_definition>(),
							scanner.GetFieldAs<tag_reference>(), 
							position_reference, read_flags);
						break;

					YELO_ASSERT_CASE_UNREACHABLE();
					}

					if(!read_result)
						success = read_result;
				}
			}

			return success;
		}

		static bool PLATFORM_API tag_block_postprocess(const tag_block_definition* definition, tag_block* block, 
			Enums::tag_postprocess_mode mode)
		{
			auto proc = definition->postprocess_proc;
			bool success = true;

			if(proc != nullptr)
			{
				for(int x = 0; x < block->count; x++)
				{
					void* element = tag_block_get_element(block, x);
					if(!proc(element, mode))
					{
						YELO_ERROR(_error_message_priority_warning, "failed to postprocess element #%d element for %s block",
							x, definition->name); // NOTE: added this warning message

						success = false;
					}
				}
			}

			return success;
		}
		static bool PLATFORM_API tag_block_read_recursive(const tag_block_definition* definition, tag_block* block,
			int32 *position_reference, long_flags read_flags)
		{
			int count = block->count;
			if(count < 0 || count > definition->maximum_element_count)
			{
				YELO_ERROR(_error_message_priority_warning, "%s block has invalid element count: #%d not in [0,#%d]",
					definition->name, count, definition->maximum_element_count);

				block->count = 0;
				return false;
			}
			else if(count == 0)
				return true;

			size_t elements_size = definition->element_size * count;
			if( !(block->address = YELO_MALLOC(elements_size, false)) )
			{
				YELO_ERROR(_error_message_priority_warning, "couldn't allocate memory for #%d element %s block",
					block->count, definition->name);

				return false;
			}

			if(!tag_file_read(*position_reference, elements_size, block->address))
			{
				YELO_ERROR(_error_message_priority_warning, "couldn't read #%d elements for %s block",
					block->count, definition->name); // NOTE: added count info to error

				return false;
			}

			byte_swap_data_explicit(definition->name, definition->element_size, definition->byte_swap_codes,
				block->count, block->address);

			*position_reference += elements_size;

			if( TEST_FLAG(definition->flags, Flags::_tag_block_dont_read_children_bit)==0 )
			{
				if(!tag_block_read_children_recursive(definition, block->address, block->count, 
					position_reference, read_flags))
					return false;
			}

			// engine doesn't do this logic, but they also don't use an enum for 'postprocess_mode'
			auto postprocess_mode = TEST_FLAG(read_flags, Flags::_tag_load_for_editor_bit) 
				? Enums::_tag_postprocess_mode_for_editor
				: Enums::_tag_postprocess_mode_for_runtime;
			if( !tag_block_postprocess(definition, block, postprocess_mode) )
				return false; // postprocess warns about failures, so we don't YELO_ERROR here

			return true;
		}
	};
};