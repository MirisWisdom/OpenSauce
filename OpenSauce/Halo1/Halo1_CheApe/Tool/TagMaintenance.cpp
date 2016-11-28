/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/TagMaintenance.hpp"
#if PLATFORM_TYPE == PLATFORM_TOOL

#include <blamlib/Halo1/tag_files/tag_field_scanner.hpp>
#include <blamlib/Halo1/tag_files/tag_files.hpp>
#include <blamlib/Halo1/tag_files/tag_group_loading.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <YeloLib/Halo1/open_sauce/settings/yelo_shared_settings.hpp>
#include <YeloLib/Halo1/tag_files/string_id_yelo.hpp>
#include <YeloLib/Halo1/tag_files/tag_group_memory.hpp>
#include <YeloLib/cseries/value_conversion.hpp>

#include "Common/StringEditing.hpp"
#include "Engine/EngineFunctions.hpp"
#include "TagGroups/TagGroups.hpp"
#include "Tool/Console.hpp"

namespace Yelo
{
	namespace Tool
	{
		static struct {
		private:
			typedef std::unordered_set<datum_index, datum_index::std_hash> datum_index_set_t;

			datum_index_set_t* resolved_tag_indexes;
			datum_index_set_t* to_resolve_tag_indexes;

			FILE* output_file;

			void OpenOutputFile()
			{
				output_file = Settings::CreateReport("tag_maintenance.txt", false, true);
			}
			void CloseOutputFile()
			{
				if (output_file != nullptr)
				{
					fclose(output_file);
					output_file = nullptr;
				}
			}

		public:
			void Initialize()
			{
				resolved_tag_indexes = YELO_NEW_CTOR(datum_index_set_t);
				to_resolve_tag_indexes = YELO_NEW_CTOR(datum_index_set_t);
			}
			void Dispose()
			{
				CloseOutputFile();

				YELO_DELETE(resolved_tag_indexes);
				YELO_DELETE(to_resolve_tag_indexes);
			}

			void MarkAsResolved(datum_index tag_index)
			{
				auto pair = resolved_tag_indexes->insert(tag_index);
				size_t erase_count = to_resolve_tag_indexes->erase(tag_index);
				if (!pair.second)
				{
					auto* group = blam::tag_group_get(blam::tag_get_group_tag(tag_index));

					YELO_WARN("maintenance failed to mark a tag as resolved? %s.%s",
						blam::tag_get_name(tag_index), group->name);
				}
			}

			bool ChildrenNeedResolving() const
			{
				return to_resolve_tag_indexes->empty() == false;
			}
			datum_index NextChildNeedingResolving() const
			{
				return *to_resolve_tag_indexes->begin();
			}
			void MarkForChildResolving(datum_index tag_index)
			{
				if (resolved_tag_indexes->find(tag_index) == resolved_tag_indexes->end() &&
					to_resolve_tag_indexes->find(tag_index) == to_resolve_tag_indexes->end())
				{
					auto* group = blam::tag_group_get(blam::tag_get_group_tag(tag_index));

					if (group->GetHeaderRuntimeInfo()->flags.group_has_tag_reference)
					{
						auto pair = to_resolve_tag_indexes->insert(tag_index);
						if (!pair.second)
						{
							YELO_WARN("maintenance failed to mark a tag for resolving? %s.%s",
								blam::tag_get_name(tag_index), group->name);
						}
					}
				}
			}

			template<bool k_flush = true>
			void Output(cstring format, ...)
			{
				if (output_file != nullptr)
				{
					va_list argptr;
					va_start(argptr, format);
					vfprintf_s(output_file, format, argptr);
					va_end(argptr);

					if (k_flush)
						fflush(output_file);
				}
			}
			void OutputHeader()
			{
				// moved from Initialize() to here because it was opening the file even when there were no
				// errors. I'd rather we open (or ultimately create) the file when the first error is hit
				OpenOutputFile();

				tag_string time_stamp;
				GetTimeStampString(time_stamp);

				Output("%s tag maintenance\n\n", time_stamp);
			}
			void OutputTagHeader(cstring group_name, cstring tag_name)
			{
				static bool first_line = true;
				if (first_line)
				{
					first_line = false;
					OutputHeader();
				}

				Output("%s.%s problems:\n", tag_name, group_name);
			}
		}maintenance_globals;

		enum { // stolen from RuntimeCacheView.cpp
			k_color_default = Enums::_console_color_white,

			k_color_tag = Enums::_console_color_lightpurple,
			k_color_name = Enums::_console_color_lightyellow,

			k_color_bad_tag = Enums::_console_color_lightaqua,
			k_color_bad_name = Enums::_console_color_lightred,
		};
		static void PrintTagPath(cstring group_name, cstring tag_name)
		{
			Console::ColorPrintF(k_color_tag, "%s", group_name);
			Console::ColorPrintF(k_color_name, " %s", tag_name);
		}

		class c_tag_maintenance_children_controller
		{
		public:
			enum {
				_mode_fix_unresolving,
				_mode_print_unresolving,
			};
			enum {
				_options_queue_non_resolving_bit,
				_options_track_hierarchy_size_bit,
			};
		protected:
			enum {
				_child_problem_none,
				_child_problem_failed_to_load,
				_child_problem_doesnt_exist,
			};
			static cstring ProblemToString(_enum problem_type)
			{
				switch (problem_type)
				{
				case _child_problem_none:			return "no problem";
				case _child_problem_failed_to_load:	return "failed to load";
				case _child_problem_doesnt_exist:	return "doesn't exist";

				YELO_ASSERT_CASE_UNREACHABLE();
				}
			}
			struct s_queued_reference
			{
				const tag_block_definition* m_owner;
				const tag_field* m_field;
				int32 m_element_index;

				_enum m_problem_type;
				PAD16;

				tag_reference* m_reference;

				s_queued_reference(const tag_block_definition* owner, const tag_field* field, int32 element_index,
					tag_reference* reference)
					: m_owner(owner)
					, m_field(field)
					, m_element_index(element_index)
					, m_problem_type(_child_problem_none)
					, m_reference(reference)
				{
				}
				s_queued_reference()
					: s_queued_reference(nullptr, nullptr, NONE, nullptr)
				{
				}
			};

			std::vector<s_queued_reference> m_queued_references;
			datum_index m_tag_index;
			_enum m_mode;
			byte_flags m_options;
			bool m_tag_is_dirty;

			size_t m_tag_hierarchy_size;

		public:
			c_tag_maintenance_children_controller(datum_index tag_index,
				_enum mode, byte_flags options)
				: m_queued_references()
				, m_tag_index(tag_index)
				, m_mode(mode)
				, m_options(options)
				, m_tag_is_dirty(false)
				, m_tag_hierarchy_size(0)
			{
			}

			size_t GetTagHierarchySize() const { return m_tag_hierarchy_size; }

		protected:
			bool ShouldQueueReference(const tag_field* field, const tag_reference* reference)
			{
				auto* definition = field->Definition<tag_reference_definition>();

				bool queue = reference->group_tag != NONE && !is_null_or_empty(reference->name);
				queue &= !TagGroups::TagFieldIsStringId(field);

				if (queue && TEST_FLAG(definition->flags, Flags::_tag_reference_non_resolving_bit))
					queue &= TEST_FLAG(m_options, _options_queue_non_resolving_bit);

				return queue;
			}
			void QueueReferencesInBlock(tag_block* block)
			{
				auto* definition = block->definition;

				for (auto element : *block)
				{
					for (auto field : TagGroups::c_tag_field_scanner(block->definition->fields, element.address)
						.AddFieldType(Enums::_field_block)
						.AddFieldType(Enums::_field_tag_reference) )
					{
						switch (field.GetType())
						{
						case Enums::_field_block:
							QueueReferencesInBlock(field.As<tag_block>());
							break;

						case Enums::_field_tag_reference:
						{
							auto* reference = field.As<tag_reference>();
							auto* field_def = &definition->fields[field.GetIndex()];

							if (ShouldQueueReference(field_def, reference))
								m_queued_references.push_back(s_queued_reference(
									definition, field_def, element.index, reference));

						} break;

						YELO_ASSERT_CASE_UNREACHABLE();
						}
					}
				}
			}

			bool ResolveQueueEntry(s_queued_reference& entry)
			{
				bool resolved = false;
				entry.m_reference->tag_index = datum_index::null;
				tag entry_group_tag = entry.m_reference->group_tag;

				if (blam::tag_file_exists(entry_group_tag, entry.m_reference->name))
				{
					entry.m_reference->tag_index = blam::tag_load(entry_group_tag, entry.m_reference->name,
						FLAG(Flags::_tag_load_for_editor_bit) | FLAG(Flags::_tag_load_non_resolving_references_bit));
					
					if (entry.m_reference->tag_index.IsNull())
						entry.m_problem_type = _child_problem_failed_to_load;
					else
					{
						resolved = true;
						maintenance_globals.MarkForChildResolving(entry.m_reference->tag_index);

						if (TEST_FLAG(m_options, _options_track_hierarchy_size_bit))
						{
							m_tag_hierarchy_size +=
								TagGroups::c_tag_group_allocation_statistics::BuildStatsForTag(entry.m_reference->tag_index);
						}
					}
				}
				else
					entry.m_problem_type = _child_problem_doesnt_exist;

				return resolved;
			}

			void UnresolvedReferenceTryAndFix(s_queued_reference& entry)
			{
				Console::ColorPrintF(Enums::_console_color_red, "FIXING NOT IMPLEMENTED\n");
				//m_tag_is_dirty = true;
			}
			void UnresolvedReferencePrint(s_queued_reference& entry)
			{
				auto* group = blam::tag_group_get(entry.m_reference->group_tag);
				cstring name = entry.m_reference->name;
				cstring problem_desc = ProblemToString(entry.m_problem_type);

				if (m_mode == _mode_print_unresolving)
				{
					Console::ColorPrintF(k_color_bad_name, "\t%s", entry.m_reference->name);
					Console::ColorPrintF(k_color_bad_tag, ".%s", group->name);
					Console::PrintNewLine();
				}

				maintenance_globals.Output("\t%s: %s.%s\n", problem_desc, name, group->name);
				maintenance_globals.Output<false>("\t\treferenced by");
				if (entry.m_field->name != nullptr)
				{
					std::string field_name(entry.m_field->name);
					StringEditing::GetStringSegment(field_name, field_name, nullptr, "^:#*");
					maintenance_globals.Output<false>(" %s", field_name.c_str());
				}
				if (!entry.m_owner->GetRuntimeInfo()->flags.is_group_header)
				{
					maintenance_globals.Output<false>(" in %s,#%d", entry.m_owner->name, entry.m_element_index);
				}
				maintenance_globals.Output("\n");
			}
			void HandleUnresolvedReference(s_queued_reference& entry)
			{
				UnresolvedReferencePrint(entry);

				if (m_mode == _mode_fix_unresolving)
					UnresolvedReferenceTryAndFix(entry);
			}

			void PrintUnresolvingHeader()
			{
				auto* group = TagGroups::TagGetGroup(m_tag_index);
				cstring name = blam::tag_get_name(m_tag_index);

				if (m_mode == _mode_print_unresolving)
				{
					Console::ColorPrint (k_color_default, "problem tags for ");
					PrintTagPath(group->name, name);
					Console::PrintNewLine();
				}

				maintenance_globals.OutputTagHeader(group->name, name);
			}
		public:
			void QueueReferences()
			{
				auto* instance = TagGroups::TagInstances()[m_tag_index];
				QueueReferencesInBlock(&instance->root_block);
			}

			void ResolveQueue()
			{
				bool printed_header = false;
				for (auto& entry : m_queued_references)
				{
					assert(entry.m_reference);

					if (!ResolveQueueEntry(entry))
					{
						if (!printed_header)
						{
							PrintUnresolvingHeader();
							printed_header = true;
						}

						HandleUnresolvedReference(entry);
					}
				}
			}
		};

		static bool UserWantsToContinue(datum_index tag_index)
		{
			auto result = Console::EnterCommand("\n;q", nullptr, "press enter to continue or \"q\" to quit");
			Console::PrintNewLine();

			return result == 0;
		}

		void PLATFORM_API tag_load_maintenance(char* arguments[])
		{
			struct s_arguments {
				cstring tag_name;
				cstring group_name;
				cstring prompt_to_continue;
				// cstring prompt_to_fix_unresolved;
				cstring load_non_resolving;
				cstring print_size;
				cstring verbose;
			}*args = CAST_PTR(s_arguments*, arguments);

			tag_group* group = TagGroups::FindTagGroupByName(args->group_name);
			if (group == nullptr)
			{
				Console::ColorPrint (k_color_default, "no such group: ");
				Console::ColorPrintF(k_color_bad_name, args->group_name);
				return;
			}

			if (!blam::tag_file_exists(group->group_tag, args->tag_name))
			{
				Console::ColorPrint (k_color_default, "file doesn't exist: ");
				Console::ColorPrintF(k_color_bad_name, "%s.%s", args->tag_name, args->group_name);
				return;
			}

			bool prompt_to_continue = true;
			// Disabled until implemented
			bool prompt_to_fix_unresolved = false;
			bool load_non_resolving = true;
			bool print_size = true;
			bool verbose = true;

			ValueConversion::FromString(args->prompt_to_continue, prompt_to_continue);
			//ValueConversion::FromString(args->prompt_to_fix_unresolved, prompt_to_fix_unresolved);
			ValueConversion::FromString(args->load_non_resolving, load_non_resolving);
			ValueConversion::FromString(args->print_size, print_size);
			ValueConversion::FromString(args->verbose, verbose);

			_enum mode = prompt_to_fix_unresolved
				? c_tag_maintenance_children_controller::_mode_fix_unresolving
				: c_tag_maintenance_children_controller::_mode_print_unresolving;
			byte_flags options;

			SET_FLAG(options, c_tag_maintenance_children_controller::_options_queue_non_resolving_bit, load_non_resolving);
			SET_FLAG(options, c_tag_maintenance_children_controller::_options_track_hierarchy_size_bit, print_size);

			maintenance_globals.Initialize();
			if (print_size)
				TagGroups::c_tag_group_allocation_statistics::Initialize();

			int debug_count = 0;
			bool is_root_tag = true;
			datum_index tag_index = datum_index::null;
			size_t tag_hierarchy_size = 0;
			bool cancelled = false;
			do
			{
				if (is_root_tag)
				{
					is_root_tag = false;
					tag_index = blam::tag_load(group->group_tag, args->tag_name, 
						FLAG(Flags::_tag_load_for_editor_bit) | FLAG(Flags::_tag_load_non_resolving_references_bit));

					if (tag_index.IsNull())
					{
						Console::ColorPrint (k_color_default, "failed to load root: ");
						Console::ColorPrintF(k_color_bad_name, "%s.%s", args->tag_name, args->group_name);
						break;
					}
				}
				else
				{
					if (prompt_to_continue)
					{
						cancelled = !UserWantsToContinue(tag_index);

						if(cancelled)
						{
							break;
						}
					}

					tag_index = maintenance_globals.NextChildNeedingResolving();
				}

				if (print_size)
				{
					if (verbose)
					{
						Console::ColorPrintF(k_color_default, "%d ", debug_count++);
						PrintTagPath(blam::tag_group_get(blam::tag_get_group_tag(tag_index))->name, 
							blam::tag_get_name(tag_index));
					}

					tag_hierarchy_size +=
						TagGroups::c_tag_group_allocation_statistics::BuildStatsForTag(tag_index);
				}

				Console::PrintNewLine();

				c_tag_maintenance_children_controller controller(tag_index, mode, options);
				controller.QueueReferences();
				controller.ResolveQueue();
				tag_hierarchy_size += controller.GetTagHierarchySize();

				maintenance_globals.MarkAsResolved(tag_index);
			} while (maintenance_globals.ChildrenNeedResolving());

			if(cancelled)
			{
				Console::ColorPrint(k_color_default, "\ncancelled");
			}
			else
			{
				Console::ColorPrint(k_color_default, "\nfinished");
			}

			if(!cancelled)
			{
				if (print_size)
				{
					TagGroups::c_tag_group_allocation_statistics::DumpToFile();
					Console::ColorPrintF(k_color_default, ". total size: %d bytes", tag_hierarchy_size);
				}

				Console::PrintNewLine();

				if (print_size)
				{
					TagGroups::c_tag_group_allocation_statistics::Dispose();
				}
			}

			maintenance_globals.Dispose();
		}
	};
};
#endif