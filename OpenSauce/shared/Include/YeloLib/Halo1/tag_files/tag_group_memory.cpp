/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#if PLATFORM_IS_EDITOR
#include <YeloLib/Halo1/tag_files/tag_group_memory.hpp>

#include <blamlib/memory/byte_swapping_base.hpp>
#include <blamlib/Halo1/tag_files/tag_field_scanner.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <YeloLib/Halo1/open_sauce/settings/yelo_shared_settings.hpp>
#include <YeloLib/Halo1/tag_files/string_id_yelo.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		static struct {
			bool field_set_runtime_info_enabled;
#if PLATFORM_TYPE != PLATFORM_TOOL // currently only tool builds can toggle on allocation headers
			const
#endif
				bool allocation_headers_enabled;
			bool string_id_yelo_runtime_is_condensed;	// string_id_yelo's runtime value doesn't contain tag_reference data
			PAD8;

			std::unordered_set<tag_block_definition*>* block_definitions;
			std::unordered_set<tag_data_definition*>* data_definitions;

			c_tag_group_allocation_statistics::group_tag_to_stats_map_t* group_statistics;

			void Initialize()
			{
				block_definitions = YELO_NEW_CTOR(std::unordered_set<tag_block_definition*>);
				data_definitions = YELO_NEW_CTOR(std::unordered_set<tag_data_definition*>);
			}

			void Dispose()
			{
				YELO_DELETE(block_definitions);
				YELO_DELETE(data_definitions);
			}
		}g_tag_group_memory_globals = {
			true,
#if PLATFORM_TYPE != PLATFORM_TOOL // allocation_headers_enabled
			false,
#else
			false,
#endif
			true
		};

		const std::unordered_set<tag_block_definition*>& GetBlockDefinitionsSet()
		{
			assert(g_tag_group_memory_globals.field_set_runtime_info_enabled);

			return *g_tag_group_memory_globals.block_definitions;
		}
		const std::unordered_set<tag_data_definition*>& GetDataDefinitionsSet()
		{
			assert(g_tag_group_memory_globals.field_set_runtime_info_enabled);

			return *g_tag_group_memory_globals.data_definitions;
		}


		//////////////////////////////////////////////////////////////////////////
		// field_comparison_code builder util
		class c_field_comparison_codes_builder
		{
			field_comparison_code*& m_final_codes_reference;
			size_t m_count;
			size_t m_index;
			std::array<field_comparison_code, s_tag_field_set_runtime_data::k_max_comparison_codes> m_codes;

			field_comparison_code& NewCode()
			{
				if (m_count != 0)
					m_index += 1;

				m_count++;
				return m_codes[m_index];
			}

			template<_enum kCodeType>
			bool CodeAlreadyExists()
			{
				return m_count > 0 &&
					m_codes[m_index].type == kCodeType;
			}

			void AllocateFinalCodes()
			{
				NewCode() = { 0, Enums::_field_comparison_code_end, NONE };

				m_final_codes_reference = new field_comparison_code[m_count];

				std::memcpy(m_final_codes_reference, m_codes.data(), sizeof(m_codes[0]) * m_count);
			}
		public:
			c_field_comparison_codes_builder(field_comparison_code*& final_codes_reference)
				: m_final_codes_reference(final_codes_reference)
				, m_count(0)
				, m_index(0)
			{
				m_final_codes_reference = nullptr;
				m_codes[0] = {};
			}
			~c_field_comparison_codes_builder()
			{
				AllocateFinalCodes();
			}

			field_comparison_code& NewBitwiseCode()
			{
				if (CodeAlreadyExists<Enums::_field_comparison_code_bitwise>())
					return m_codes[m_index];

				field_comparison_code& code = NewCode();
				code.size = 0;
				code.type = Enums::_field_comparison_code_bitwise;
				code.field_index = NONE;
				return code;
			}

			field_comparison_code& NewSkipCode()
			{
				if (CodeAlreadyExists<Enums::_field_comparison_code_skip>())
					return m_codes[m_index];

				field_comparison_code& code = NewCode();
				code.size = 0;
				code.type = Enums::_field_comparison_code_skip;
				code.field_index = NONE;
				return code;
			}

			void NewPointerCode(int32 field_index)
			{
				NewCode() = { 0, Enums::_field_comparison_code_pointer, field_index };
			}

			void NewTagReferenceCode(int32 field_index)
			{
				NewCode() = { 0, Enums::_field_comparison_code_tag_reference, field_index };
			}

			void NewTagDataCode(int32 field_index)
			{
				NewCode() = { 0, Enums::_field_comparison_code_tag_data, field_index };
			}

			void NewBlockCode(int32 field_index)
			{
				NewCode() = { 0, Enums::_field_comparison_code_block, field_index };
			}

			void NewBlockIndexCode(int32 field_index, size_t field_size)
			{
				NewCode() = { field_size, Enums::_field_comparison_code_block_index, field_index };
			}
		};

		//////////////////////////////////////////////////////////////////////////
		// CheApe hooks
		bool BlockHasCustomAlignment(const tag_block_definition* definition, _Out_ size_t& alignment_bit);
		bool BlockHasVolatileFields(const tag_block_definition* definition);
		bool BlockHasCustomComparisonCodes(const tag_block_definition* definition, _Out_ field_comparison_code*& codes_reference);

		//////////////////////////////////////////////////////////////////////////
		// s_tag_field_set_runtime_data
		s_tag_field_set_runtime_data* BuildRuntimeInfoForBlockDefinition(const tag_block_definition* group_header_definition, 
			tag_block_definition* block_definition);

		void s_tag_field_set_runtime_data::Initialize(const tag_block_definition* group_header_definition, 
			const tag_block_definition* definition)
		{
			std::memset(this, 0, sizeof(*this)); // I like to act like I know what I'm doing. Ctors? Never heard of 'em!

			BuildInfo(group_header_definition, definition);
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
					flags.has_string_id;
		}
		void s_tag_field_set_runtime_data::SetIsGroupHeader()
		{
			flags.is_group_header = true;
		}
		void s_tag_field_set_runtime_data::DecrementRuntimeSize(size_t amount)
		{
			flags.has_runtime_size = true;

			assert(amount < runtime_size ); // validate it wouldn't cause an overflow
			runtime_size -= amount;
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
			flags.is_block_indexed = true;

			IncrementDirectReferenceCount();
		}
		void s_tag_field_set_runtime_data::IncrementTagReferenceFieldCount()
		{
			flags.has_tag_reference = true;

			assert(counts.tag_reference_fields < k_max_tag_reference_fields);
			counts.tag_reference_fields++;
		}
		void s_tag_field_set_runtime_data::IncrementBlockFieldCount()
		{
			flags.has_tag_block = true;

			assert(counts.block_fields < k_max_block_fields);
			counts.block_fields++;
		}
		void s_tag_field_set_runtime_data::IncrementBlockIndexFieldCount()
		{
			flags.has_block_index = true;

			assert(counts.block_index_fields < k_max_block_index_fields);
			counts.block_index_fields++;
		}
		void s_tag_field_set_runtime_data::IncrementDataFieldCount()
		{
			flags.has_tag_data = true;

			assert(counts.data_fields < k_max_data_fields);
			counts.data_fields++;
		}
		void s_tag_field_set_runtime_data::IncrementStringFieldCount()
		{
			assert(counts.string_fields < k_max_string_fields);
			counts.string_fields++;
		}
		void s_tag_field_set_runtime_data::IncrementStringIdFieldCount()
		{
			flags.has_string_id = true;

			assert(counts.string_id_fields < k_max_string_id_fields);
			counts.string_id_fields++;
		}
		void s_tag_field_set_runtime_data::IncrementTotalPaddingSize(size_t size)
		{
			assert((counts.padding_amount+size) <= k_max_padding_amount);
			counts.padding_amount += size;
		}
		void s_tag_field_set_runtime_data::IncrementDebugDataSize(size_t size)
		{
			counts.debug_data_amount += size;
		}

		void s_tag_field_set_runtime_data::BuildInfoHandleDataField(tag_data_definition* definition)
		{
			IncrementDataFieldCount();

			IncrementDebugDataSize(tag_data::k_debug_data_size);
			if (definition->IsConsideredDebugOnly())
				flags.has_debug_child_data = true;

			g_tag_group_memory_globals.data_definitions->insert(definition);
		}
		void s_tag_field_set_runtime_data::BuildInfo(const tag_block_definition* group_header_definition, 
			const tag_block_definition* definition)
		{
			runtime_size = definition->element_size;

			s_tag_field_set_runtime_data* group_header_info = nullptr;
			if (group_header_definition && group_header_definition != definition)
				group_header_info = group_header_definition->GetRuntimeInfo();

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
					if (field.IsOldStringId())
					{
						flags.has_old_string_id = true;

						if (group_header_info)
							group_header_info->flags.group_has_old_string_id = true;
					}

					IncrementStringFieldCount();
					break;

				case Enums::_field_tag_reference:
					if(field.IsStringId())
					{
						IncrementStringIdFieldCount();

						if (group_header_info)
							group_header_info->flags.group_has_string_id = true;

						IncrementDebugDataSize(string_id_yelo::k_debug_data_size);

						if(g_tag_group_memory_globals.string_id_yelo_runtime_is_condensed)
							DecrementRuntimeSize(string_id_yelo::k_debug_data_size);
					}
					else // NOTE: we don't currently count the string_id_yelo tag_reference field
					{
						IncrementTagReferenceFieldCount();

						if (group_header_info)
							group_header_info->flags.group_has_tag_reference = true;

						IncrementDebugDataSize(tag_reference::k_debug_data_size);
					}
					break;

				case Enums::_field_block:
					IncrementBlockFieldCount();
					if (group_header_info)
						group_header_info->flags.group_has_tag_block = true;

					IncrementDebugDataSize(tag_block::k_debug_data_size);

					BuildRuntimeInfoForBlockDefinition( group_header_definition, field.DefinitionAs<tag_block_definition>() )
						->IncrementReferenceCountFromBlockField();
					break;

				case Enums::_field_short_block_index:
				case Enums::_field_long_block_index:
					IncrementBlockIndexFieldCount();
					if (group_header_info)
						group_header_info->flags.group_has_block_index = true;

					BuildRuntimeInfoForBlockDefinition( group_header_definition, field.DefinitionAs<tag_block_definition>() )
						->IncrementReferenceCountFromBlockIndexField();
					break;

				case Enums::_field_data:
					if (group_header_info)
						group_header_info->flags.group_has_tag_data = true;

					BuildInfoHandleDataField(field.DefinitionAs<tag_data_definition>());
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
			if(flags.custom_comparison)
			{
				assert(comparison_codes);
				return;
			}

			auto codes_builder = c_field_comparison_codes_builder(comparison_codes);
			for(auto field : TagGroups::c_tag_field_scanner(definition->fields)
				.AddAllFieldTypes() )
			{
				// since we're iterating over -all- fields, might as well do this here
				if(field.IsBlockName())
					block_name_field = &definition->fields[field.GetIndex()];

				switch(field.GetType())
				{
				case Enums::_field_tag_reference:
					if(g_tag_group_memory_globals.string_id_yelo_runtime_is_condensed && field.IsStringId())
					{
						runtime_size -= string_id_yelo::k_debug_data_size;

						codes_builder.NewSkipCode().size += string_id_yelo::k_debug_data_size; // tag_reference
					}
					else
						codes_builder.NewTagReferenceCode(field.GetIndex());
					break;

				case Enums::_field_block:
					codes_builder.NewBlockCode(field.GetIndex());
					break;

				case Enums::_field_short_block_index:
					codes_builder.NewBlockIndexCode(field.GetIndex(), 
						k_tag_field_definitions[Enums::_field_short_block_index].size);
					break;
				case Enums::_field_long_block_index:
					codes_builder.NewBlockIndexCode(field.GetIndex(),
						k_tag_field_definitions[Enums::_field_long_block_index].size);
					break;

				case Enums::_field_data:
					codes_builder.NewTagDataCode(field.GetIndex());
					break;

				case Enums::_field_pad:
					// the padding that comes after a string_id tag reference is named "string_id"
					// other pad fields which are actually postprocessed fields should be named as well
					if (field.GetName() == nullptr)
					{
						codes_builder.NewSkipCode().size += field.GetSize();
						break;
					}
					// fall through: pad field has name, assume not actually padding
				default:
					codes_builder.NewBitwiseCode().size += field.GetSize();
					break;
				}
			}
		}

		void s_tag_field_set_runtime_data::DestroyByteComparisonCodes()
		{
			if(comparison_codes != nullptr && !flags.custom_comparison)
			{
				delete[] comparison_codes;
				comparison_codes = nullptr;
			}
		}

		void s_tag_field_set_runtime_data::DumpThisToFile(FILE* file, const tag_block_definition* definition) const
		{
			fprintf_s(file, "0x%X ", definition->element_size);
			if (flags.has_runtime_size)
				fprintf_s(file, "0x%X ", runtime_size);
			fprintf_s(file, "%s%s\n", definition->name, flags.is_group_header
				? " (group)"
				: "");

			if (flags.no_shared_memory)
				fprintf_s(file, "%s\n", "NO memory sharing");
			if (flags.is_block_indexed)
				fprintf_s(file, "%s\n", "IS indexed");

			if (counts.references)
				fprintf_s(file, "\t%u %s\n", counts.references, "references");
			if (counts.padding_amount)
				fprintf_s(file, "\t%u %s\n", counts.padding_amount, "padding");
			if (counts.debug_data_amount)
				fprintf_s(file, "\t%u %s\n", counts.debug_data_amount, "debug data size");
			if (counts.tag_reference_fields)
				fprintf_s(file, "\t%u %s\n", counts.tag_reference_fields, "tag reference fields");
			if (counts.block_fields)
				fprintf_s(file, "\t%u %s\n", counts.block_fields, "block fields");
			if (counts.block_index_fields)
				fprintf_s(file, "\t%u %s\n", counts.block_index_fields, "block index fields");
			if (counts.data_fields)
				fprintf_s(file, "\t%u %s\n", counts.data_fields, "data fields");
			if (counts.string_fields)
				fprintf_s(file, "\t%u %s\n", counts.string_fields, "string fields");
			if (counts.string_id_fields)
				fprintf_s(file, "\t%u %s\n", counts.string_id_fields, "string_id fields");
		}
		void s_tag_field_set_runtime_data::DumpToFile()
		{
			if (!Enabled())
				return;

			auto file = std::unique_ptr<FILE, crt_file_closer>(
				Settings::CreateReport("group_field_set_info.txt", false, true));

			for (auto definition : GetBlockDefinitionsSet())
			{
				definition->GetRuntimeInfo()->DumpThisToFile(file.get(), definition);
			}
		}


		//////////////////////////////////////////////////////////////////////////
		// system definitions
		bool s_tag_field_set_runtime_data::Enabled()
		{
			return g_tag_group_memory_globals.field_set_runtime_info_enabled;
		}

		static s_tag_field_set_runtime_data* BuildRuntimeInfoForBlockDefinition(const tag_block_definition* group_header_definition, 
			tag_block_definition* block_definition)
		{
			s_tag_field_set_runtime_data* info = block_definition->GetRuntimeInfo();
			if(info != nullptr)
				return info;

			g_tag_group_memory_globals.block_definitions->insert(block_definition);

			info = new s_tag_field_set_runtime_data;
			block_definition->SetRuntimeInfo(info);

			info->Initialize(group_header_definition, block_definition);

			return info;
		}
		void BuildGroupRuntimeInfo()
		{
			if(!g_tag_group_memory_globals.field_set_runtime_info_enabled)
				return;

			g_tag_group_memory_globals.Initialize();

			struct build_runtime_data_info_action
			{
				const tag_block_definition* m_group_header_definition;

				void operator()(tag_block_definition* block_definition) const
				{
					BuildRuntimeInfoForBlockDefinition(m_group_header_definition, block_definition);
				}
				void operator()(tag_group* group)
				{
					m_group_header_definition = group->header_block_definition;

					group->header_block_definition->DoRecursiveAction(*this);
					group->header_block_definition->GetRuntimeInfo()->SetIsGroupHeader();
				}
			};

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
				delete info;
			} void operator()(tag_group* group) const
			{
				group->header_block_definition->DoRecursiveAction(*this);
			} };

			tag_groups_do_action<destroy_runtime_data_info_action>();

			g_tag_group_memory_globals.Dispose();
		}

		bool s_tag_allocation_header::Enabled()
		{
			return g_tag_group_memory_globals.allocation_headers_enabled;
		}


		//////////////////////////////////////////////////////////////////////////
		// c_tag_group_allocation_statistics system definitions
		bool c_tag_group_allocation_statistics::Enabled()
		{
			return g_tag_group_memory_globals.group_statistics != nullptr;
		}

		void c_tag_group_allocation_statistics::Initialize()
		{
			assert( s_tag_field_set_runtime_data::Enabled() );
			assert( !Enabled() );

			g_tag_group_memory_globals.group_statistics = YELO_NEW_CTOR(c_tag_group_allocation_statistics::group_tag_to_stats_map_t);
		}
		void c_tag_group_allocation_statistics::Dispose()
		{
			assert( Enabled() );

			YELO_DELETE( g_tag_group_memory_globals.group_statistics );
		}
		void c_tag_group_allocation_statistics::Reset()
		{
			assert( Enabled() );

			g_tag_group_memory_globals.group_statistics->clear();
		}

		c_tag_group_allocation_statistics& c_tag_group_allocation_statistics::GetStats(tag group_tag)
		{
			assert( Enabled() );

			auto& group_stats_map = *g_tag_group_memory_globals.group_statistics;
			auto group_entry = group_stats_map.find(group_tag);

			if(group_entry == group_stats_map.cend())
			{
				tag_group* group = blam::tag_group_get(group_tag);
				assert( group!=nullptr );

				auto result = group_stats_map.insert( std::make_pair(group_tag, c_tag_group_allocation_statistics(group_tag, 
					*group->GetHeaderRuntimeInfo()) ));

				group_entry = result.first;
			}

			return (*group_entry).second;
		}

		const c_tag_group_allocation_statistics::group_tag_to_stats_map_t& c_tag_group_allocation_statistics::GroupStatistics()
		{
			return *g_tag_group_memory_globals.group_statistics;
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

#endif