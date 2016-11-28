/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#if PLATFORM_IS_EDITOR
#include <YeloLib/Halo1/tag_files/tag_group_memory.hpp>

#include <blamlib/Halo1/tag_files/tag_field_scanner.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <YeloLib/Halo1/open_sauce/settings/yelo_shared_settings.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		//////////////////////////////////////////////////////////////////////////
		// s_tag_allocation_statistics
		s_tag_allocation_statistics& s_tag_allocation_statistics::Initialize(const tag_block_definition* definition)
		{
			block_definition = definition;
			data_definition = nullptr;

			std::memset(&block, 0, sizeof(block));

			return *this;
		}
		s_tag_allocation_statistics& s_tag_allocation_statistics::Initialize(const tag_data_definition* definition)
		{
			block_definition = nullptr;
			data_definition = definition;

			std::memset(&block, 0, sizeof(block));

			return *this;
		}

		void s_tag_allocation_statistics::Update(const tag_block* instance)
		{
			assert(instance->definition == block_definition);
			size_t num_of_elements = CAST(size_t, instance->count);

			block.count++;
			block.elements += num_of_elements;

			if (s_tag_field_set_runtime_data::Enabled())
			{
				const auto* info = block_definition->GetRuntimeInfo();

				block.size +=				num_of_elements * info->runtime_size;
				block.padding +=			num_of_elements * info->counts.padding_amount;
				block.debug_bytes_size +=	num_of_elements * info->counts.debug_data_amount;
			}
			else
			{
				block.size +=				num_of_elements * block_definition->element_size;
				block.padding = 0;
				block.debug_bytes_size = 0;
			}
		}
		void s_tag_allocation_statistics::Update(const tag_data* instance)
		{
			assert(instance->definition == data_definition);

			data.count++;
			size_t total_size = CAST(size_t, instance->size);

			if (data_definition->IsConsideredDebugOnly())
			{
				data.debug_bytes_size += total_size;
				data.size = 0;
			}
			else
			{
				data.debug_bytes_size = 0;
				data.size += total_size;
			}
		}

		static cstring k_tag_allocation_statistics_dump_format = 
			"% 60s % 9d % 9d % 9d % 9d % 11d\r\n";
		static cstring k_tag_allocation_statistics_dump_header_format = 
			"%-60s % 9s % 9s % 9s % 9s % 11s\r\n";
		void s_tag_allocation_statistics::DumpBlockInfoToFile(FILE* file, s_block_totals& group_totals) const
		{
			group_totals.debug_bytes_size += block.debug_bytes_size;
			group_totals.count += block.count;
			group_totals.elements += block.elements;
			group_totals.size += block.size;
			group_totals.padding += block.padding;

			if (file != nullptr)
				fprintf_s(file, k_tag_allocation_statistics_dump_format,
					block_definition->name,
					block.count, block.elements, block.size, block.padding, block.debug_bytes_size);
		}
		void s_tag_allocation_statistics::DumpDataInfoToFile(FILE* file, s_block_totals& group_totals) const
		{
			group_totals.count += data.count;

			int32 size = NONE;
			int32 debug_size = NONE;
			if (data_definition->IsConsideredDebugOnly())
			{
				group_totals.debug_bytes_size += data.debug_bytes_size;
				debug_size = CAST(int32, data.debug_bytes_size);
			}
			else
			{
				size = CAST(int32, data.size);
				group_totals.size += data.size;
			}

			if (file != nullptr)
				fprintf_s(file, k_tag_allocation_statistics_dump_format,
					data_definition->name,
					data.count, NONE, size, NONE, debug_size);
		}


		//////////////////////////////////////////////////////////////////////////
		// c_tag_group_allocation_statistics
		s_tag_allocation_statistics& c_tag_group_allocation_statistics::GetChildStats(const tag_block* instance)
		{
			auto* definition = instance->definition;
			auto predicate = [definition](const s_tag_allocation_statistics& stats) { return stats.block_definition == definition; };
			auto iter = std::find_if(m_children.begin(), m_children.end(),
				predicate);

			if(iter == m_children.end())
			{
				m_children.push_back( s_tag_allocation_statistics().Initialize(instance->definition) );
				return m_children[m_children.size()-1];
			}

			return *iter;
		}
		s_tag_allocation_statistics& c_tag_group_allocation_statistics::GetChildStats(const tag_data* instance)
		{
			auto* definition = instance->definition;
			auto predicate = [definition](const s_tag_allocation_statistics& stats) { return stats.data_definition == definition; };
			auto iter = std::find_if(m_children.begin(), m_children.end(),
				predicate);

			if(iter == m_children.end())
			{
				m_children.push_back( s_tag_allocation_statistics().Initialize(instance->definition) );
				return m_children[m_children.size()-1];
			}

			return *iter;
		}

		c_tag_group_allocation_statistics::c_tag_group_allocation_statistics(tag group_tag, const s_tag_field_set_runtime_data& header_info) :
			m_group_tag(group_tag),
			// doesn't take into account the child of the immediate child blocks
			m_children(header_info.counts.block_fields + header_info.counts.data_fields)
		{
		}

		s_tag_allocation_statistics& c_tag_group_allocation_statistics::GetBlockStats(datum_index tag_index, const tag_block* instance)
		{
			tag group_tag = blam::tag_get_group_tag(tag_index);
			auto& group_stats = GetStats(group_tag);

			return group_stats.GetChildStats(instance);
		}
		s_tag_allocation_statistics& c_tag_group_allocation_statistics::GetDataStats(datum_index tag_index, const tag_data* instance)
		{
			tag group_tag = blam::tag_get_group_tag(tag_index);
			auto& group_stats = GetStats(group_tag);

			return group_stats.GetChildStats(instance);
		}

		void c_tag_group_allocation_statistics::DumpToFile()
		{
			auto file = std::unique_ptr<FILE, crt_file_closer>(
				Settings::CreateReport("group_memory.txt", false, true));

			size_t total_padding = 0, total_size = 0, total_debug_size = 0;

			for (auto pair : GroupStatistics())
			{
				fprintf_s(file.get(), k_tag_allocation_statistics_dump_header_format,
					blam::tag_group_get(pair.first)->name,
					"count", "elements", "size", "padding", "debug size");

				s_tag_allocation_statistics::s_block_totals totals;
				std::memset(&totals, 0, sizeof(totals));

				auto& stats = pair.second;
				for (auto& child_stats : stats.m_children)
				{
					if (child_stats.IsBlock())
					{
						child_stats.DumpBlockInfoToFile(file.get(), totals);
					}
					else if (child_stats.IsData())
					{
						child_stats.DumpDataInfoToFile(file.get(), totals);
					}
				}

				fprintf_s(file.get(), k_tag_allocation_statistics_dump_format,
					"totals:",
					totals.count, totals.elements, totals.size, totals.padding, totals.debug_bytes_size);
				fprintf_s(file.get(), "\r\n");

				total_debug_size += totals.debug_bytes_size;
				total_padding += totals.padding;
				total_size += totals.size;
			}

			fprintf_s(file.get(), "\r\ntotal padding: %u", total_padding);
			fprintf_s(file.get(), "\r\ntotal size: %u", total_size);
			fprintf_s(file.get(), "\r\ntotal debug bytes: %u", total_debug_size);
		}

		size_t c_tag_group_allocation_statistics::BuildStatsForTagChildBlockRecursive(c_tag_group_allocation_statistics& group_stats,
			tag_block* instance)
		{
			if (instance->count == 0)
				return 0;

			size_t element_size = instance->get_element_size();
			if (s_tag_field_set_runtime_data::Enabled())
				element_size = instance->definition->GetRuntimeInfo()->runtime_size;

			// our rough estimation of the size at runtime
			size_t block_size = CAST(size_t, instance->count) * element_size;
			group_stats.GetChildStats(instance).Update(instance);

			for (auto element : *instance)
			{
				for (auto field : TagGroups::c_tag_field_scanner(instance->definition->fields, element.address)
					.AddFieldType(Enums::_field_block)
					.AddFieldType(Enums::_field_data))
				{
					switch(field.GetType())
					{
					case Enums::_field_block: 
						block_size +=
							BuildStatsForTagChildBlockRecursive(group_stats, field.As<tag_block>());
						break;

					case Enums::_field_data:
					{
						auto* data_instance = field.As<const tag_data>();
						if (data_instance->size > 0)
						{
							auto& data_stats = group_stats.GetChildStats(data_instance);
							data_stats.Update(data_instance);

							size_t instance_size = CAST(size_t, data_instance->size);

							// don't update the result block_size if the data field is debug-only
							if (!data_stats.data_definition->IsConsideredDebugOnly())
								block_size += instance_size;
						}
					} break;

					YELO_ASSERT_CASE_UNREACHABLE();
					}
				}
			}

			return block_size;
		}
		size_t c_tag_group_allocation_statistics::BuildStatsForTag(datum_index tag_index)
		{
			tag group_tag = blam::tag_get_group_tag(tag_index);
			tag_block* root_block = blam::tag_get_root_block(tag_index);

			auto& group_stats = GetStats(group_tag);
			return BuildStatsForTagChildBlockRecursive(group_stats, root_block);
		}
	};
};

#endif