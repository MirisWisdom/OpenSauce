/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "TagGroups/yelo_definitions.hpp"

#include <blamlib/Halo1/game/game_globals.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <blamlib/Halo1/units/unit_definitions.hpp>

#include <YeloLib/Halo1/open_sauce/project_yellow_global_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_global_cv_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>
#include <YeloLib/Halo1/tag_files/tag_database_definitions.hpp>

#include "Engine/EngineFunctions.hpp"
#include "Engine/GrenadeTypesUpgrade.hpp"
#include "Engine/Scripting.hpp"
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace TagGroups
	{
		//////////////////////////////////////////////////////////////////////////
		// unit_external_upgrades_block
		static int UnitGetSeatIndexFromLabel(s_unit_definition* unit_def, cstring seat_label)
		{
			for (int i = 0; i < unit_def->unit.seats.Count; i++)
			{
				if (strcmp(seat_label, unit_def->unit.seats[i].label) == 0)
					return i;
			}
			return NONE;
		}

		static void UnitExternalUpgradesBlockPostprocess(TAG_TBLOCK(& unit_external_upgrades_def, TagGroups::s_unit_external_upgrades), 
			Enums::tag_postprocess_mode mode)
		{
			if (mode != Enums::_tag_postprocess_mode_for_runtime)
				return;

			for(auto& upgrade : unit_external_upgrades_def)
			{
				datum_index tag_index = blam::tag_load(upgrade.unit.group_tag, upgrade.unit.name, 
					FLAG(Flags::_tag_load_non_resolving_references_bit));

				if (!tag_index.IsNull())
				{
					auto* unit_def = blam::tag_get<s_unit_definition>(tag_index);

					for(auto& seat : upgrade.boarding_seats)
					{
						cstring seat_label = seat.seat_label;
						cstring target_seat_label = seat.target_seat_label;

						seat.seat_index = UnitGetSeatIndexFromLabel(unit_def, seat_label);
						seat.target_seat_index = UnitGetSeatIndexFromLabel(unit_def, target_seat_label);
					}
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// project_yellow_globals_cv
		static bool PLATFORM_API py_globals_cv_group_postprocess(Yelo::datum_index tag_index, Enums::tag_postprocess_mode mode)
		{
			auto* def = blam::tag_get<project_yellow_globals_cv>(tag_index);

			def->version = project_yellow_globals_cv::k_version;
			UnitExternalUpgradesBlockPostprocess(def->unit_external_upgrades, mode);

			return true;
		}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// scripting_block
		static cstring PLATFORM_API scripting_block_construct_format(datum_index tag_index, tag_block* block, int32 element, char formatted_buffer[Enums::k_tag_block_format_buffer_size])
		{
			auto* elem = CAST_PTR(s_script_construct_definition*, blam::tag_block_get_element(block, element));

			if(elem->name[0][0] != '\0')
				strncpy_s(formatted_buffer, Enums::k_tag_block_format_buffer_size, elem->name[0], Enums::k_tag_string_length);
			if(elem->name[1][0] != '\0')
				strncat_s(formatted_buffer, Enums::k_tag_block_format_buffer_size, elem->name[1], Enums::k_tag_string_length);

			return formatted_buffer;
		}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// project_yellow_globals
		static bool PLATFORM_API py_globals_group_postprocess(Yelo::datum_index tag_index, Enums::tag_postprocess_mode mode)
		{
			auto* def = blam::tag_get<project_yellow_globals>(tag_index);

			def->version = project_yellow_globals::k_version;

			Scripting::ScriptingBlockClear(def->yelo_scripting);
			if(mode == Enums::_tag_postprocess_mode_for_runtime)
			{
				Scripting::ScriptingBlockAddDefinitions(def->yelo_scripting, true);
			}

			return true;
		}

#if FALSE
		static cstring PLATFORM_API py_globals_preprocess_maplist_format(datum_index tag_index, tag_block* block, int32 element, char formatted_buffer[Enums::k_tag_block_format_buffer_size])
		{
			auto* elem =  CAST_PTR(s_yelo_preprocess_maplist_entry*, blam::tag_block_get_element(block, element));

			cstring value = elem->name;
			if( !strcmp(value, "") )
			{
				if( elem->scenario.name_length > 0 && elem->scenario.name != nullptr)
					value = elem->scenario.name;
				else
					return formatted_buffer; // if name is empty and there is no scenario reference, no formatting is done
			}

			strncpy_s(formatted_buffer, Enums::k_tag_block_format_buffer_size, value, Enums::k_tag_string_length);

			return formatted_buffer;
		}
#endif
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// project_yellow
		static bool PLATFORM_API py_group_postprocess(datum_index tag_index, Enums::tag_postprocess_mode mode)
		{
			auto* def = blam::tag_get<project_yellow>(tag_index);

			def->version = project_yellow::k_version;

			Scripting::ScriptingBlockClear(def->user_scripting);
			if(mode == Enums::_tag_postprocess_mode_for_runtime)
			{
				if(!def->physics.IsGravityScaleValid() || def->physics.gravity_scale == 0.0f)			def->physics.ResetGravityScale();
				if(!def->physics.IsPlayerSpeedScaleValid() || def->physics.player_speed_scale == 0.0f)	def->physics.ResetPlayerSpeedScale();

				Scripting::ScriptingBlockAddDefinitions(def->user_scripting, false);
			}

			return true;
		}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// s_yelo_definition_globals
		static struct s_yelo_definition_globals {
			bool initialized;
			PAD24;
			tag_group* py_globals_cv_definition;
			tag_group* py_globals_definition;
			tag_group* py_definition;

		private:
			bool VerifyYeloGlobalsCvGroup() const
			{
				return	py_globals_cv_definition->version == project_yellow_globals_cv::k_version &&
						py_globals_cv_definition->header_block_definition->element_size == sizeof(project_yellow_globals_cv);
			}
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
			nullptr, nullptr, nullptr,
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
					YELO_ERROR(_error_message_priority_none, 
						"CheApe: %s group definition doesn't match code!", group_name);
			}
			else YELO_ERROR(_error_message_priority_none, 
					"CheApe: %s group not found!", group_name);
		}
		bool s_yelo_definition_globals::VerifyGroupDefinitions() const
		{
			bool valid_definitions = true;

			VerifyGroupDefinitionHandler(valid_definitions,
				py_globals_cv_definition, &s_yelo_definition_globals::VerifyYeloGlobalsCvGroup,
				"project_yellow_globals_cv");

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
			{// project_yellow_globals_cv
				py_globals_cv_definition->postprocess_proc = &TagGroups::py_globals_cv_group_postprocess;
			}
			//////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////
			{// project_yellow_globals
				py_globals_definition->postprocess_proc = &TagGroups::py_globals_group_postprocess;

#if FALSE
				//////////////////////////////////////////////////////////////////////////
				{// preprocess_block
					field_index = py_globals_definition->header_block_definition->FindFieldIndex(Enums::_field_block, "preprocess");
					if(field_index == NONE)
					{
						YELO_ERROR(_error_message_priority_assert, 
							"CheApe: preprocess_block not found!");
					}

					auto* preprocess_block_def = py_globals_definition->definition->fields[field_index].Definition<tag_block_definition>();
					field_index = preprocess_block_def->FindFieldIndex(Enums::_field_block, "campaign");
					if(field_index == NONE)
					{
						YELO_ERROR(_error_message_priority_assert, 
							"CheApe: preprocess_maplist_block not found!");
					}

					auto* preprocess_map_block_def = preprocess_block_def->fields[field_index].Definition<tag_block_definition>();
					preprocess_map_block_def->format_proc = &TagGroups::py_globals_preprocess_maplist_format;
				}
#endif


				//////////////////////////////////////////////////////////////////////////
				{// scripting_block
					// NOTE: this will also affect project_yellow's script block as it's the same definition
					field_index = py_globals_definition->header_block_definition->FindFieldIndex(Enums::_field_block, "yelo scripting");
					if(field_index == NONE)
					{
						YELO_ERROR(_error_message_priority_assert, 
							"CheApe: scripting_block not found!");
					}

					auto* scripting_block_def = py_globals_definition->header_block_definition->fields[field_index].Definition<tag_block_definition>();

					field_index = scripting_block_def->FindFieldIndex(Enums::_field_block, "new functions");
					if(field_index == NONE)
					{
						YELO_ERROR(_error_message_priority_assert, 
							"CheApe: script_function_block not found!");
					}
					auto* script_function_block_def = scripting_block_def->fields[field_index].Definition<tag_block_definition>();
					script_function_block_def->format_proc = &TagGroups::scripting_block_construct_format;

					field_index = scripting_block_def->FindFieldIndex(Enums::_field_block, "new globals");
					if(field_index == NONE)
					{
						YELO_ERROR(_error_message_priority_assert, 
							"CheApe: script_global_block not found!");
					}
					auto* script_global_block_def = scripting_block_def->fields[field_index].Definition<tag_block_definition>();
					script_global_block_def->format_proc = &TagGroups::scripting_block_construct_format;
				}
			}
			//////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////
			{// project_yellow
				py_definition->postprocess_proc = &TagGroups::py_group_postprocess;
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
			py_globals_cv_definition = blam::tag_group_get<project_yellow_globals_cv>();
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

#if PLATFORM_ID == PLATFORM_TOOL
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