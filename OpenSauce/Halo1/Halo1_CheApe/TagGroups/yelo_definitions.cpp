/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "TagGroups/yelo_definitions.hpp"

#include <blamlib/Halo1/game/game_globals.hpp>
#include <blamlib/Halo1/game/game_globals_definitions.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>

#include <YeloLib/Halo1/open_sauce/project_yellow_global_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>
#include <YeloLib/Halo1/tag_files/tag_database_definitions.hpp>

#include "Engine/EngineFunctions.hpp"
#include "Engine/GrenadeTypesUpgrade.hpp"
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace TagGroups
	{
		//////////////////////////////////////////////////////////////////////////
		// s_yelo_definition_globals
		static struct s_yelo_definition_globals {
			bool initialized;
			PAD24;
			tag_group* py_globals_definition;
			tag_group* py_definition;

		private:
			bool VerifyYeloGlobalsGroup() const
			{
				return	py_globals_definition->version == project_yellow_globals::k_version &&
						py_globals_definition->header_block_definition->element_size == sizeof(project_yellow_globals);
			}
			bool VerifyYeloScenarioGroup() const
			{
				return	py_definition->version == project_yellow::k_version &&
						py_definition->header_block_definition->element_size == sizeof(project_yellow);
			}
			bool VerifyGroupDefinitions() const;

			// Populate the group references with their function pointers, etc
			void InitializeGroupDefinitions();
			void InitializeGroupReferences();
		public:
			void Initialize();
			void Dispose();
		}_yelo_definition_globals = {
			false,
			nullptr, nullptr,
		};

		static void VerifyGroupDefinitionHandler(bool& valid_definitions,
			const tag_group* group,
			bool (s_yelo_definition_globals::* verify_group_proc)() const, // pointer to member function grossness
			cstring group_name)
		{
			if( valid_definitions=(group != nullptr) )
			{
				valid_definitions = (_yelo_definition_globals.*verify_group_proc)();

				if (!valid_definitions)
					YELO_ERROR_CRITICAL("CheApe: %s group definition doesn't match code!", group_name);
			}
			else
				YELO_ERROR_CRITICAL("CheApe: %s group not found!", group_name);
		}
		bool s_yelo_definition_globals::VerifyGroupDefinitions() const
		{
			bool valid_definitions = true;

			VerifyGroupDefinitionHandler(valid_definitions,
				py_globals_definition, &s_yelo_definition_globals::VerifyYeloGlobalsGroup,
				"project_yellow_globals");

			VerifyGroupDefinitionHandler(valid_definitions,
				py_definition, &s_yelo_definition_globals::VerifyYeloScenarioGroup,
				"project_yellow");

			return valid_definitions;
		}

		void s_yelo_definition_globals::InitializeGroupDefinitions()
		{
			int32 field_index = NONE;
			
			//////////////////////////////////////////////////////////////////////////
			{// project_yellow_globals
				py_globals_definition->postprocess_proc = &TagGroups::project_yellow_globals::GroupPostprocess;

#if FALSE
				//////////////////////////////////////////////////////////////////////////
				{// preprocess_block
					field_index = py_globals_definition->header_block_definition->FindFieldIndex(Enums::_field_block, "preprocess");
					YELO_ASSERT_DISPLAY(field_index != NONE, 
						"CheApe: preprocess_block not found!");

					auto* preprocess_block_def = py_globals_definition->definition->fields[field_index].Definition<tag_block_definition>();
					field_index = preprocess_block_def->FindFieldIndex(Enums::_field_block, "campaign");
					YELO_ASSERT_DISPLAY(field_index != NONE, 
						"CheApe: preprocess_maplist_block not found!");

					auto* preprocess_map_block_def = preprocess_block_def->fields[field_index].Definition<tag_block_definition>();
					preprocess_map_block_def->format_proc = &TagGroups::s_yelo_preprocess_maplist_entry::FormatBlockName;
				}
#endif


				//////////////////////////////////////////////////////////////////////////
				{// scripting_block
					// NOTE: this will also affect project_yellow's script block as it's the same definition
					field_index = py_globals_definition->header_block_definition->FindFieldIndex(Enums::_field_block, "yelo scripting");
					YELO_ASSERT_DISPLAY(field_index != NONE,
						"CheApe: scripting_block not found!");

					auto* scripting_block_def = py_globals_definition->header_block_definition->fields[field_index].Definition<tag_block_definition>();

					field_index = scripting_block_def->FindFieldIndex(Enums::_field_block, "new functions");
					YELO_ASSERT_DISPLAY(field_index != NONE,
						"CheApe: script_function_block not found!");

					auto* script_function_block_def = scripting_block_def->fields[field_index].Definition<tag_block_definition>();
					script_function_block_def->format_proc = &TagGroups::s_script_construct_definition::FormatBlockName;

					field_index = scripting_block_def->FindFieldIndex(Enums::_field_block, "new globals");
					YELO_ASSERT_DISPLAY(field_index != NONE,
						"CheApe: script_global_block not found!");

					auto* script_global_block_def = scripting_block_def->fields[field_index].Definition<tag_block_definition>();
					script_global_block_def->format_proc = &TagGroups::s_script_construct_definition::FormatBlockName;
				}
			}
			//////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////
			{// project_yellow
				py_definition->postprocess_proc = &TagGroups::project_yellow::GroupPostprocess;
			}
			//////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////
			{// make the first tag_reference field (which is unused) in the scenario to reference the yelo tag
				static tag_reference_definition reference_definition = {
					0,
					project_yellow::k_group_tag,
					nullptr
				};

				tag_group* scnr = blam::tag_group_get(TagGroups::scenario::k_group_tag);
				tag_field* field = &scnr->header_block_definition->fields[0];
				field->name = "project yellow definitions";
				field->definition = &reference_definition;
			}
			//////////////////////////////////////////////////////////////////////////
		}

		void s_yelo_definition_globals::InitializeGroupReferences()
		{
			py_globals_definition = blam::tag_group_get<project_yellow_globals>();
			py_definition = blam::tag_group_get<project_yellow>();
		}

		void s_yelo_definition_globals::Initialize()
		{
			InitializeGroupReferences();

			if(VerifyGroupDefinitions())
			{
				InitializeGroupDefinitions();
				initialized = true;
			}
		}
		void s_yelo_definition_globals::Dispose()
		{
			if(initialized)
			{
			}

			initialized = false;
		}
		//////////////////////////////////////////////////////////////////////////

		bool YeloDefinitionsInitialized()
		{
			return _yelo_definition_globals.initialized;
		}

		void YeloDefinitionsInitialize()
		{
			_yelo_definition_globals.Initialize();
			s_tag_database::Initialize();
			Objects::Items::GrenadeTypesUpgradeInitializeTagDefinitions();
		}

		void YeloDefinitionsDispose()
		{
			Objects::Items::GrenadeTypesUpgradeDisposeTagDefinitions();
			s_tag_database::Dispose();
			_yelo_definition_globals.Dispose();
		}
		//////////////////////////////////////////////////////////////////////////

#if PLATFORM_TYPE == PLATFORM_TOOL
		static bool GameGlobalsRequiresYeloGameStateUpgrades(datum_index game_globals_index)
		{
			auto* game_globals = blam::tag_get<TagGroups::s_game_globals>(game_globals_index);

			return false;
		}

		bool YeloToolCheckTagsForGameStateUpgradeRequirements()
		{
			bool result = false;

	// no tags currently need to be checked
	#if FALSE
			TagGroups::s_tag_iterator iter;
			blam::tag_iterator_new(iter);

			datum_index tag_index;
			while( !(tag_index = blam::tag_iterator_next(iter)).IsNull() )
			{
				switch( blam::tag_get_group_tag(tag_index) )
				{
				case TagGroups::s_game_globals::k_group_tag:
					result |= GameGlobalsRequiresYeloGameStateUpgrades(tag_index); 
				}
			}
	#endif

			return result;
		}
#endif
	};
};