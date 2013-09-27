/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include "YeloLib/Halo1/tag_files/tag_group_memory.hpp"

#include <blamlib/memory/byte_swapping_base.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <YeloLib/Halo1/tag_files/string_id_yelo.hpp>
#include "Engine/EngineFunctions.hpp"

namespace Yelo
{
	namespace TagGroups
	{
		static struct {
			bool field_set_runtime_info_enabled;
			bool allocation_headers_enabled;
			bool string_id_yelo_runtime_is_condensed;	// string_id_yelo's runtime value doesn't contain tag_reference data
		}g_tag_group_memory;

		//////////////////////////////////////////////////////////////////////////
		// s_tag_field_set_runtime_data
		void s_tag_field_set_runtime_data::Initialize(const tag_block_definition* definition, bool is_group_header)
		{
			SET_FLAG(flags, Flags::_tag_field_set_runtime_is_group_header_bit, is_group_header);

			BuildInfo(definition);
			BuildByteComparisonCodes(definition);
		}
		void s_tag_field_set_runtime_data::Dispose()
		{
			DestroyByteComparisonCodes();
		}

		void s_tag_field_set_runtime_data::DecrementRuntimeSize(size_t amount)
		{
			SET_FLAG(flags, Flags::_tag_field_set_runtime_has_runtime_size_bit, true);

			assert(amount < runtime_size ); // validate it wouldn't cause an overflow
			runtime_size += amount;
		}
		void s_tag_field_set_runtime_data::IncrementDirectReferenceCount()
		{
			assert(counts.references < k_max_direct_references);
			counts.references++;
		}
		void s_tag_field_set_runtime_data::IncrementReferenceCountFromBlockField()
		{
			IncrementDirectReferenceCount();
		}
		void s_tag_field_set_runtime_data::IncrementReferenceCountFromBlockIndexField()
		{
			SET_FLAG(flags, Flags::_tag_field_set_runtime_is_block_indexed_bit, true);

			IncrementDirectReferenceCount();
		}
		void s_tag_field_set_runtime_data::IncrementTagReferenceFieldCount()
		{
			SET_FLAG(flags, Flags::_tag_field_set_runtime_has_tag_reference_bit, true);

			assert(counts.tag_reference_fields < k_max_tag_reference_fields);
			counts.tag_reference_fields++;
		}
		void s_tag_field_set_runtime_data::IncrementBlockFieldCount()
		{
			SET_FLAG(flags, Flags::_tag_field_set_runtime_has_tag_block_bit, true);

			assert(counts.block_fields < k_max_block_fields);
			counts.block_fields++;
		}
		void s_tag_field_set_runtime_data::IncrementBlockIndexFieldCount()
		{
			SET_FLAG(flags, Flags::_tag_field_set_runtime_has_block_index_bit, true);

			assert(counts.block_index_fields < k_max_block_index_fields);
			counts.block_index_fields++;
		}
		void s_tag_field_set_runtime_data::IncrementDataFieldCount()
		{
			SET_FLAG(flags, Flags::_tag_field_set_runtime_has_tag_data_bit, true);

			assert(counts.data_fields < k_max_data_fields);
			counts.data_fields++;
		}
		void s_tag_field_set_runtime_data::IncrementTotalPaddingSize(size_t size)
		{
			counts.padding_amount += size;
			assert(counts.padding_amount <= k_max_padding_amount);
		}

		void s_tag_field_set_runtime_data::BuildInfo(const tag_block_definition* definition)
		{
			runtime_size = definition->element_size;

			for(auto field : TagGroups::c_tag_field_scanner(definition->fields, nullptr)
				.AddFieldType(Enums::_field_string)
				.AddFieldType(Enums::_field_tag_reference)
				.AddFieldType(Enums::_field_block)
				.AddFieldType(Enums::_field_short_block_index)
				.AddFieldType(Enums::_field_long_block_index)
				.AddFieldType(Enums::_field_data)
				.AddFieldType(Enums::_field_pad) )
			{
				switch(field.GetType())
				{
				case Enums::_field_string:
					if(field.IsOldStringId())
						SET_FLAG(flags, Flags::_tag_field_set_runtime_has_old_string_id_bit, true);
					break;

				case Enums::_field_tag_reference:
					if(field.IsStringId())
					{
						SET_FLAG(flags, Flags::_tag_field_set_runtime_has_string_id_bit, true);

						if(g_tag_group_memory.string_id_yelo_runtime_is_condensed)
							DecrementRuntimeSize(string_id_yelo::k_debug_data_size);
					}
					else // NOTE: we don't currently count the string_id_yelo tag_reference field
						IncrementDirectReferenceCount();
					break;

				case Enums::_field_block:
					IncrementBlockFieldCount();

					// TODO: IncrementReferenceCountFromBlockField on the field's definition
					break;

				case Enums::_field_short_block_index:
				case Enums::_field_long_block_index:
					IncrementBlockIndexFieldCount();

					// TODO: IncrementReferenceCountFromBlockIndexField on the field's definition
					break;

				case Enums::_field_data:
					IncrementDataFieldCount();
					break;

				case Enums::_field_pad:
					IncrementTotalPaddingSize(field.GetSize());
					break;

				YELO_ASSERT_CASE_UNREACHABLE();
				}
			}
		}

		void s_tag_field_set_runtime_data::BuildByteComparisonCodes(const tag_block_definition* definition)
		{
			struct s_build_state
			{
				std::array<int16, 512> codes;
				size_t count;
				bool last_field_was_positive;
				PAD24;

				inline void PrepareCodesForNewPositive()
				{
					if(!last_field_was_positive)
						codes[++count] = 0;
				}
				inline void Finish()
				{
					count++;
				}
				// Return a reference to the current code (for incrementing)
				inline int16& IncCode()
				{
					return codes[count  ];
				}
				// Return a reference to the current code (for incrementing) and move the cursor to the next code
				// Proceeding IncCode() method should be an '=', NOT '+='
				inline int16& IncCodeWithAnew()
				{
					return codes[count++];
				}
				inline void SetLastFieldWasPositive(bool answer)
				{
					last_field_was_positive = answer;
				}
			}state;

			for(auto field : TagGroups::c_tag_field_scanner(definition->fields)
				.AddAllFieldTypes() )
			{
				assert( state.count < state.codes.size() );

				switch(field.GetType())
				{
				case Enums::_field_tag_reference:
					if(g_tag_group_memory.string_id_yelo_runtime_is_condensed && field.IsStringId())
						break;

					state.PrepareCodesForNewPositive();

					state.IncCodeWithAnew() +=	sizeof(tag);			// group_tag
					state.IncCode() =			Enums::_bs_code_4byte;	// name reference
					state.IncCodeWithAnew() +=	Enums::_bs_code_4byte;	// name length
					state.IncCode() =			sizeof(datum_index);	// tag_index
					state.SetLastFieldWasPositive(true);
					break;

				case Enums::_field_block:
					state.PrepareCodesForNewPositive();

					state.IncCodeWithAnew() +=	sizeof(int32);			// count
					state.IncCode()  =			Enums::_bs_code_4byte;	// address
					state.IncCode() +=			Enums::_bs_code_4byte;	// definition
					state.SetLastFieldWasPositive(false);
					break;

				case Enums::_field_data:
					state.PrepareCodesForNewPositive();

					state.IncCodeWithAnew() +=	sizeof(int32);			// size
					state.IncCode()  =			Enums::_bs_code_4byte;	// flags
					state.IncCode() +=			Enums::_bs_code_4byte;	// stream_position
					state.IncCode() +=			Enums::_bs_code_4byte;	// address
					state.IncCode() +=			Enums::_bs_code_4byte;	// definition
					state.SetLastFieldWasPositive(false);
					break;

				default:
					state.PrepareCodesForNewPositive();

					state.IncCode() =			CAST(int16, field.GetSize());
					state.SetLastFieldWasPositive(true);
					break;
				}
			}
			state.Finish();

			comparison_codes = YELO_NEW_ARRAY(int16, state.count);
			// NOTE: ignore the warning C4996
			std::copy(state.codes.cbegin(), state.codes.cend(),
				comparison_codes);
		}

		void s_tag_field_set_runtime_data::DestroyByteComparisonCodes()
		{
			if(comparison_codes != nullptr)
			{
				YELO_DELETE_ARRAY(comparison_codes);
				comparison_codes = nullptr;
			}
		}


		//////////////////////////////////////////////////////////////////////////
		// s_tag_allocation_header
		const s_tag_allocation_header* s_tag_allocation_header::Get(const tag_block* instance)
		{
			s_tag_allocation_header* header = nullptr;

			if(g_tag_group_memory.allocation_headers_enabled && instance->address != nullptr)
			{
				header = CAST_PTR(s_tag_allocation_header*, instance->address) - 1;
				if(header->signature != k_signature)
					header = nullptr;
			}

			return header;
		}
		const s_tag_allocation_header* s_tag_allocation_header::Get(const tag_data* instance)
		{
			s_tag_allocation_header* header = nullptr;

			if(g_tag_group_memory.allocation_headers_enabled && instance->address != nullptr)
			{
				header = CAST_PTR(s_tag_allocation_header*, instance->address) - 1;
				if(header->signature != k_signature)
					header = nullptr;
			}

			return header;
		}


		//////////////////////////////////////////////////////////////////////////
		// s_tag_allocation_statistics
		void s_tag_allocation_statistics::Initialize(const tag_block_definition* definition)
		{
			block_definition = definition;

			block.count = block.elements = block.size = block.padding = 0;
		}
		void s_tag_allocation_statistics::Initialize(const tag_data_definition* definition)
		{
			data_definition = definition;

			data.count = data.size = 0;
		}

		void s_tag_allocation_statistics::Update(const tag_block* instance)
		{
			assert(instance->definition == block_definition);

			block.count++;
			block.elements += instance->count;

			if(g_tag_group_memory.field_set_runtime_info_enabled)
			{
				const auto* info = block_definition->get_runtime_info();

				block.size += info->runtime_size * instance->count;
				block.padding = info->counts.padding_amount;
			}
			else
			{
				block.size += block_definition->element_size * instance->count;
				block.padding = NONE;
			}
		}
		void s_tag_allocation_statistics::Update(const tag_data* instance)
		{
			assert(instance->definition == data_definition);

			data.count++;
			data.size += instance->size;
		}
	};

	size_t tag_block_definition::get_alignment_bit() const
	{
		assert( this );
		return TagGroups::g_tag_group_memory.field_set_runtime_info_enabled
			? CAST_PTR(size_t, this->unused0)
			: Flags::_alignment_32_bit; // TODO: or should we just stick with 8bit?
	}

	TagGroups::s_tag_field_set_runtime_data* tag_block_definition::get_runtime_info() const
	{
		assert( this );
		return TagGroups::g_tag_group_memory.field_set_runtime_info_enabled
			? CAST_PTR(TagGroups::s_tag_field_set_runtime_data*, this->unused1)
			: nullptr;
	}
};