/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/TagMaintenance.hpp"
#if PLATFORM_ID == PLATFORM_TOOL

#include <blamlib/Halo1/tag_files/tag_files.hpp>
#include <blamlib/Halo1/tag_files/tag_group_loading.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <YeloLib/Halo1/open_sauce/settings/yelo_shared_settings.hpp>
#include <YeloLib/Halo1/tag_files/string_id_yelo.hpp>
#include <YeloLib/Halo1/tag_files/tag_group_memory.hpp>

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

			tag_group* model_group, * gbxmodel_group;
			FILE* output_file;

			void OpenOutputFile()
			{
				output_file = Settings::CreateReport("tag_maintenance.txt", false, true);
			}
			void CloseOutputFile()
			{
				if (output_file != nullptr)
					fclose(output_file);
			}

		public:
			void Initialize()
			{
				OpenOutputFile();

				resolved_tag_indexes = YELO_NEW_CTOR(datum_index_set_t);
				to_resolve_tag_indexes = YELO_NEW_CTOR(datum_index_set_t);

				model_group = blam::tag_group_get('mode');
				gbxmodel_group = blam::tag_group_get('mod2');
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
				if (resolved_tag_indexes->count(tag_index) == 0 && to_resolve_tag_indexes->count(tag_index) == 0)
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

			tag_group* TagGroupGet(tag group_tag)
			{
				tag_group* group = blam::tag_group_get(group_tag);

				return group == model_group 
					? gbxmodel_group 
					: group;
			}
			tag TagReferenceGetGroupTag(const tag_reference* reference)
			{
				return reference->group_tag == model_group->group_tag 
					? gbxmodel_group->group_tag 
					: reference->group_tag;
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

			k_color_bad_tag = Enums::_console_color_lightred,
			k_color_bad_name = Enums::_console_color_red,
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
				case _child_problem_none: return "no problem";
				case _child_problem_failed_to_load: return "failed to load";
				case _child_problem_doesnt_exist: return "doesn't exist";

				YELO_ASSERT_CASE_UNREACHABLE();
				}
			}
			struct s_queued_reference
			{
				const tag_block_definition* m_owner;
				tag_field* m_field;
				int32 m_element_index;

				_enum m_problem_type;
				PAD16;

				tag_reference* m_reference;

				s_queued_reference(const tag_block_definition* owner, tag_field* field, int32 element_index,
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
			bool m_tag_is_dirty;
			bool m_queue_non_resolving;

		public:
			c_tag_maintenance_children_controller(datum_index tag_index, bool queue_non_resolving,
				_enum mode)
				: m_queued_references()
				, m_tag_index(tag_index)
				, m_mode(mode)
				, m_tag_is_dirty(false)
				, m_queue_non_resolving(queue_non_resolving)
			{
			}

		protected:
			bool ShouldQueueReference(const tag_field* field, const tag_reference* reference)
			{
				auto* definition = field->Definition<tag_reference_definition>();

				bool queue = reference->group_tag != NONE && strlen(reference->name) != 0;
				queue &= !TagGroups::TagFieldIsStringId(field);

				if (queue && TEST_FLAG(definition->flags, Flags::_tag_reference_non_resolving_bit))
					queue &= m_queue_non_resolving;

				return queue;
			}
			void QueueReferencesInBlock(tag_block* block)
			{
				auto* definition = block->definition;

				for (int x = 0; x < block->count; x++)
				{
					void* element = CAST_PTR(byte*, block->address) +
						(definition->element_size * x);

					for (auto field : TagGroups::c_tag_field_scanner(block->definition->fields, element)
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
									definition, field_def, x, reference));

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
				tag entry_group_tag = maintenance_globals.TagReferenceGetGroupTag(entry.m_reference);

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
				auto* group = maintenance_globals.TagGroupGet(entry.m_reference->group_tag);
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
				auto* group = maintenance_globals.TagGroupGet(blam::tag_get_group_tag(m_tag_index));
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
			auto* group = maintenance_globals.TagGroupGet(blam::tag_get_group_tag(tag_index));
			cstring tag_name = blam::tag_get_name(tag_index);

			Console::ColorPrint(k_color_default, "press enter to continue, anything else to stop\n\t");
			PrintTagPath(group->name, tag_name);
			Console::PrintNewLine();

			return getc(stdin) != '\n';
		}
		void PLATFORM_API tag_load_maintenance(void** arguments)
		{
			struct s_arguments {
				cstring tag_name;
				cstring group_name;
				cstring prompt_to_continue;
				cstring prompt_to_fix_unresolved;
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

			bool prompt_to_continue = Settings::ParseBoolean(args->prompt_to_continue);
			bool prompt_to_fix_unresolved = Settings::ParseBoolean(args->prompt_to_fix_unresolved);
			bool load_non_resolving = Settings::ParseBoolean(args->load_non_resolving);
			bool print_size = Settings::ParseBoolean(args->print_size);
			bool verbose = Settings::ParseBoolean(args->verbose);
			_enum mode = prompt_to_fix_unresolved
				? c_tag_maintenance_children_controller::_mode_fix_unresolving
				: c_tag_maintenance_children_controller::_mode_print_unresolving;

			maintenance_globals.Initialize();
			if (print_size)
				TagGroups::c_tag_group_allocation_statistics::Initialize();

			int debug_count = 0;
			bool is_root_tag = true;
			datum_index tag_index = datum_index::null;
			size_t tag_hierarchy_size = 0;
			do {
				if (verbose)
					Console::ColorPrintF(k_color_default, "%d\n", debug_count++);

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
					tag_index = maintenance_globals.NextChildNeedingResolving();

					if (prompt_to_continue && !UserWantsToContinue(tag_index))
						break;
				}

				if (print_size)
				{
					tag_hierarchy_size +=
						TagGroups::c_tag_group_allocation_statistics::BuildStatsForTag(tag_index);
				}

				c_tag_maintenance_children_controller controller(tag_index, load_non_resolving, mode);
				controller.QueueReferences();
				controller.ResolveQueue();

				maintenance_globals.MarkAsResolved(tag_index);
			} while (maintenance_globals.ChildrenNeedResolving());

			Console::ColorPrint(k_color_default, "\nfinished");
			if (print_size)
			{
				TagGroups::c_tag_group_allocation_statistics::DumpToFile();
				Console::ColorPrintF(k_color_default, ". total size: %d bytes", tag_hierarchy_size);
			}
			Console::PrintNewLine();

			if (print_size)
				TagGroups::c_tag_group_allocation_statistics::Dispose();
			maintenance_globals.Dispose();
		}
	};
};
#endif