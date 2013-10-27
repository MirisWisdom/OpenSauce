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

typedef std::unordered_map<Yelo::tag, Yelo::TagGroups::c_tag_group_allocation_statistics> 
	tag_group_allocation_statistics_map_t;

namespace Yelo
{
	namespace TagGroups
	{
		static struct {
			bool field_set_runtime_info_enabled;
			bool allocation_headers_enabled;
			bool string_id_yelo_runtime_is_condensed;	// string_id_yelo's runtime value doesn't contain tag_reference data

			tag_group_allocation_statistics_map_t* group_statistics;
		}g_tag_group_memory_globals = {
			true,
			false,
			true
		};

		//////////////////////////////////////////////////////////////////////////
		// CheApe hooks
		bool BlockHasCustomAlignment(const tag_block_definition* definition, _Out_ size_t& alignment_bit);
		bool BlockHasVolatileFields(const tag_block_definition* definition);
		bool BlockHasCustomComparisonCodes(const tag_block_definition* definition, _Out_ comparison_code_t*& codes_reference);

		//////////////////////////////////////////////////////////////////////////
		// s_tag_field_set_runtime_data
		s_tag_field_set_runtime_data* BuildRuntimeInfoForBlockDefinition(tag_block_definition* block_definition);

		void s_tag_field_set_runtime_data::Initialize(const tag_block_definition* definition)
		{
			std::memset(this, 0, sizeof(*this)); // I like to act like I know what I'm doing. Ctors? Never heard of 'em!

			BuildInfo(definition);
			CallCheApeHooks(definition);
			BuildByteComparisonCodes(definition);
		}
		void s_tag_field_set_runtime_data::Dispose()
		{
			DestroyByteComparisonCodes();
		}

		bool s_tag_field_set_runtime_data::ContainsRuntimeOptimizedFields() const
		{
			return	g_tag_group_memory_globals.string_id_yelo_runtime_is_condensed &&
					TEST_FLAG(flags, Flags::_tag_field_set_runtime_has_string_id_bit);
		}
		void s_tag_field_set_runtime_data::SetIsGroupHeader()
		{
			SET_FLAG(flags, Flags::_tag_field_set_runtime_is_group_header_bit, true);
		}
		void s_tag_field_set_runtime_data::DecrementRuntimeSize(size_t amount)
		{
			SET_FLAG(flags, Flags::_tag_field_set_runtime_has_runtime_size_bit, true);

			assert(amount < runtime_size ); // validate it wouldn't cause an overflow
			runtime_size += amount;
		}
		void s_tag_field_set_runtime_data::IncrementDirectReferenceCount()
		{
			assert(counts.references <= k_max_direct_references);
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

			assert(counts.tag_reference_fields <= k_max_tag_reference_fields);
			counts.tag_reference_fields++;
		}
		void s_tag_field_set_runtime_data::IncrementBlockFieldCount()
		{
			SET_FLAG(flags, Flags::_tag_field_set_runtime_has_tag_block_bit, true);

			assert(counts.block_fields <= k_max_block_fields);
			counts.block_fields++;
		}
		void s_tag_field_set_runtime_data::IncrementBlockIndexFieldCount()
		{
			SET_FLAG(flags, Flags::_tag_field_set_runtime_has_block_index_bit, true);

			assert(counts.block_index_fields <= k_max_block_index_fields);
			counts.block_index_fields++;
		}
		void s_tag_field_set_runtime_data::IncrementDataFieldCount()
		{
			SET_FLAG(flags, Flags::_tag_field_set_runtime_has_tag_data_bit, true);

			assert(counts.data_fields <= k_max_data_fields);
			counts.data_fields++;
		}
		void s_tag_field_set_runtime_data::IncrementStringFieldCount()
		{
			assert(counts.string_fields <= k_max_string_fields);
			counts.string_fields++;
		}
		void s_tag_field_set_runtime_data::IncrementStringIdFieldCount()
		{
			SET_FLAG(flags, Flags::_tag_field_set_runtime_has_string_id_bit, true);

			assert(counts.string_id_fields <= k_max_string_id_fields);
			counts.string_id_fields++;
		}
		void s_tag_field_set_runtime_data::IncrementTotalPaddingSize(size_t size)
		{
			assert((counts.padding_amount+size) <= k_max_padding_amount);
			counts.padding_amount += size;
		}

		void s_tag_field_set_runtime_data::BuildInfo(const tag_block_definition* definition)
		{
			runtime_size = definition->element_size;

			for(auto field : TagGroups::c_tag_field_scanner(definition->fields)
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

					IncrementStringFieldCount();
					break;

				case Enums::_field_tag_reference:
					if(field.IsStringId())
					{
						IncrementStringIdFieldCount();

						if(g_tag_group_memory_globals.string_id_yelo_runtime_is_condensed)
							DecrementRuntimeSize(string_id_yelo::k_debug_data_size);
					}
					else // NOTE: we don't currently count the string_id_yelo tag_reference field
						IncrementTagReferenceFieldCount();
					break;

				case Enums::_field_block:
					IncrementBlockFieldCount();

					BuildRuntimeInfoForBlockDefinition( field.DefinitionAs<tag_block_definition>() )
						->IncrementReferenceCountFromBlockField();
					break;

				case Enums::_field_short_block_index:
				case Enums::_field_long_block_index:
					IncrementBlockIndexFieldCount();

					BuildRuntimeInfoForBlockDefinition( field.DefinitionAs<tag_block_definition>() )
						->IncrementReferenceCountFromBlockIndexField();
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

		void s_tag_field_set_runtime_data::CallCheApeHooks(const tag_block_definition* definition)
		{
			// TODO: check for custom alignment bit
			// TODO: check for volatile memory
			// TODO: check for custom comparison codes
		}

		void s_tag_field_set_runtime_data::BuildByteComparisonCodes(const tag_block_definition* definition)
		{
			struct s_build_state
			{
				bool last_field_was_positive;
				PAD24;
				size_t count;
				std::array<comparison_code_t, Enums::k_maximum_field_byte_swap_codes / 2> codes;

				inline void PrepareCodesForNewPositive()
				{
					if(!last_field_was_positive)
						codes[++count] = 0;
				}
				inline void PrepareCodesForNewNegative()
				{
					if( last_field_was_positive)
						codes[++count] = 0;
				}
				inline void Finish()
				{
					assert( count < codes.size() );

					codes[++count] = 0;
				}
				// Return a reference to the current code (for incrementing)
				inline comparison_code_t& IncCode()
				{
					return codes[count  ];
				}
				// Return a reference to the current code (for incrementing) and move the cursor to the next code
				// Proceeding IncCode() method should be an '=', NOT '+='
				inline comparison_code_t& IncCodeWithAnew()
				{
					return codes[count++];
				}
				inline void SetLastFieldWasPositive(bool answer)
				{
					last_field_was_positive = answer;
				}
			}state = {false, 0};

			if(TEST_FLAG(flags, Flags::_tag_field_set_runtime_custom_comparison_bit))
			{
				assert(comparison_codes);
				return;
			}

			for(auto field : TagGroups::c_tag_field_scanner(definition->fields)
				.AddAllFieldTypes() )
			{
				assert( state.count < state.codes.size() );

				// since we're iterating over -all- fields, might as well do this here
				if(field.IsBlockName())
					block_name_field = &definition->fields[field.GetIndex()];

				switch(field.GetType())
				{
				case Enums::_field_tag_reference:
					if(g_tag_group_memory_globals.string_id_yelo_runtime_is_condensed && field.IsStringId())
					{
						runtime_size -= string_id_yelo::k_debug_data_size;

						state.PrepareCodesForNewNegative();
						state.IncCode() =		-string_id_yelo::k_debug_data_size; // tag_reference
						state.SetLastFieldWasPositive(true);
						break;
					}

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

			comparison_codes = YELO_NEW_ARRAY(comparison_code_t, state.count);
			std::copy_n(state.codes.cbegin(), state.count,
				stdext::checked_array_iterator<comparison_code_t*>(comparison_codes, state.count));
		}

		void s_tag_field_set_runtime_data::DestroyByteComparisonCodes()
		{
			if(comparison_codes != nullptr && !TEST_FLAG(flags, Flags::_tag_field_set_runtime_custom_comparison_bit))
			{
				YELO_DELETE_ARRAY(comparison_codes);
				comparison_codes = nullptr;
			}
		}


		//////////////////////////////////////////////////////////////////////////
		// system definitions
		bool s_tag_field_set_runtime_data::Enabled()
		{
			return g_tag_group_memory_globals.field_set_runtime_info_enabled;
		}

		static s_tag_field_set_runtime_data* BuildRuntimeInfoForBlockDefinition(tag_block_definition* block_definition)
		{
			s_tag_field_set_runtime_data* info = block_definition->GetRuntimeInfo();
			if(info != nullptr)
				return info;

			info = YELO_NEW(s_tag_field_set_runtime_data);
			block_definition->SetRuntimeInfo(info);

			info->Initialize(block_definition);

#if FALSE
			YELO_WARN( 
				"% 56s % 4X % 4X "
				"% 4d % 4d % 4d "
				"% 4d % 4d % 8X ",
				block_definition->name, info->flags, info->runtime_size,
				info->counts.references, info->counts.tag_reference_fields, info->counts.block_fields, 
				info->counts.block_index_fields, info->counts.data_fields, info->counts.padding_amount);
#endif

			return info;
		}
		void BuildGroupRuntimeInfo()
		{
			if(!g_tag_group_memory_globals.field_set_runtime_info_enabled)
				return;

			struct build_runtime_data_info_action
			{ void operator()(tag_block_definition* block_definition) const
			{
				BuildRuntimeInfoForBlockDefinition(block_definition);
			} void operator()(tag_group* group) const
			{
				group->header_block_definition->DoRecursiveAction(*this);
				group->header_block_definition->GetRuntimeInfo()->SetIsGroupHeader();
			} };

			tag_groups_do_action<build_runtime_data_info_action>();
		}

		void DestroyGroupRuntimeInfo()
		{
			if(!g_tag_group_memory_globals.field_set_runtime_info_enabled)
				return;

			struct destroy_runtime_data_info_action
			{ void operator()(tag_block_definition* block_definition) const
			{
				auto* info = block_definition->GetRuntimeInfo();
				if(info == nullptr)
					return;

				block_definition->SetRuntimeInfo(nullptr);

				info->Dispose();
				YELO_DELETE(info);
			} void operator()(tag_group* group) const
			{
				group->header_block_definition->DoRecursiveAction(*this);
			} };

			tag_groups_do_action<destroy_runtime_data_info_action>();
		}


		//////////////////////////////////////////////////////////////////////////
		// s_tag_allocation_header
		const s_tag_allocation_header* s_tag_allocation_header::Get(const tag_block* instance)
		{
			s_tag_allocation_header* header = nullptr;

			if(g_tag_group_memory_globals.allocation_headers_enabled && instance->address != nullptr)
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

			if(g_tag_group_memory_globals.allocation_headers_enabled && instance->address != nullptr)
			{
				header = CAST_PTR(s_tag_allocation_header*, instance->address) - 1;
				if(header->signature != k_signature)
					header = nullptr;
			}

			return header;
		}


		//////////////////////////////////////////////////////////////////////////
		// s_tag_allocation_statistics
		s_tag_allocation_statistics& s_tag_allocation_statistics::Initialize(const tag_block_definition* definition)
		{
			block_definition = definition;

			block.count = block.elements = block.size = block.padding = 0;

			return *this;
		}
		s_tag_allocation_statistics& s_tag_allocation_statistics::Initialize(const tag_data_definition* definition)
		{
			data_definition = definition;

			data.count = data.size = 0;

			return *this;
		}

		void s_tag_allocation_statistics::Update(const tag_block* instance)
		{
			assert(instance->definition == block_definition);

			block.count++;
			block.elements += instance->count;

			if(g_tag_group_memory_globals.field_set_runtime_info_enabled)
			{
				const auto* info = block_definition->GetRuntimeInfo();

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


		//////////////////////////////////////////////////////////////////////////
		// c_tag_group_allocation_statistics
		s_tag_allocation_statistics* c_tag_group_allocation_statistics::GetChildStats(const tag_block* instance)
		{
			s_tag_allocation_statistics* stats = nullptr;

			auto* definition = instance->definition;
			auto predicate = [definition](const s_tag_allocation_statistics& stats) { return stats.block_definition == definition; };
			auto iter = std::find_if(m_children.begin(), m_children.end(),
				predicate);

			if(iter == m_children.end())
			{
				m_children.push_back( s_tag_allocation_statistics().Initialize(instance->definition) );
				stats = &m_children[m_children.size()-1];
			}

			return stats;
		}
		s_tag_allocation_statistics* c_tag_group_allocation_statistics::GetChildStats(const tag_data* instance)
		{
			s_tag_allocation_statistics* stats = nullptr;

			auto* definition = instance->definition;
			auto predicate = [definition](const s_tag_allocation_statistics& stats) { return stats.data_definition == definition; };
			auto iter = std::find_if(m_children.begin(), m_children.end(),
				predicate);

			if(iter == m_children.end())
			{
				m_children.push_back( s_tag_allocation_statistics().Initialize(instance->definition) );
				stats = &m_children[m_children.size()-1];
			}

			return stats;
		}

		c_tag_group_allocation_statistics::c_tag_group_allocation_statistics(tag group_tag, const s_tag_field_set_runtime_data& header_info) :
			m_group_tag(group_tag),
			// doesn't take into account the child of the immediate child blocks
			m_children(header_info.counts.block_fields + header_info.counts.data_fields)
		{
		}
		//////////////////////////////////////////////////////////////////////////
		// system definitions
		bool c_tag_group_allocation_statistics::Enabled()
		{
			return g_tag_group_memory_globals.group_statistics != nullptr;
		}

		void c_tag_group_allocation_statistics::Initialize()
		{
			assert( s_tag_field_set_runtime_data::Enabled() );
			assert( !Enabled() );

			g_tag_group_memory_globals.group_statistics = YELO_NEW_CTOR(tag_group_allocation_statistics_map_t);
		}
		void c_tag_group_allocation_statistics::Dispose()
		{
			assert( Enabled() );

			YELO_DELETE( g_tag_group_memory_globals.group_statistics );
		}

		c_tag_group_allocation_statistics* c_tag_group_allocation_statistics::GetStats(tag group_tag)
		{
			assert( Enabled() );
			c_tag_group_allocation_statistics* group_stats = nullptr;

			auto& group_stats_map = *g_tag_group_memory_globals.group_statistics;
			auto group_entry = group_stats_map.find(group_tag);

			if(group_entry == group_stats_map.cend())
			{
				tag_group* group = blam::tag_group_get(group_tag);
				assert( group!=nullptr );

				group_stats_map.insert( std::make_pair(group_tag, c_tag_group_allocation_statistics(group_tag, 
					*group->GetHeaderRuntimeInfo()) ));
			}
			else
				group_stats = &(*group_entry).second;

			return group_stats;
		}

		s_tag_allocation_statistics* c_tag_group_allocation_statistics::GetBlockStats(datum_index tag_index, const tag_block* instance)
		{
			tag group_tag = blam::tag_get_group_tag(tag_index);
			auto* group_stats = GetStats(group_tag);

			return group_stats->GetChildStats(instance);
		}
		s_tag_allocation_statistics* c_tag_group_allocation_statistics::GetDataStats(datum_index tag_index, const tag_data* instance)
		{
			tag group_tag = blam::tag_get_group_tag(tag_index);
			auto* group_stats = GetStats(group_tag);

			return group_stats->GetChildStats(instance);
		}
	};

	size_t tag_block_definition::GetAlignmentBit() const
	{
		assert( this );
		return TagGroups::g_tag_group_memory_globals.field_set_runtime_info_enabled
			? CAST_PTR(size_t, this->unused0)
			: Flags::_alignment_32_bit; // TODO: or should we just stick with 8bit?
	}

	TagGroups::s_tag_field_set_runtime_data* tag_block_definition::GetRuntimeInfo() const
	{
		assert( this );
		return TagGroups::g_tag_group_memory_globals.field_set_runtime_info_enabled
			? CAST_PTR(TagGroups::s_tag_field_set_runtime_data*, this->unused1)
			: nullptr;
	}
	void tag_block_definition::SetRuntimeInfo(TagGroups::s_tag_field_set_runtime_data* info)
	{
		this->unused1 = info;
	}
};