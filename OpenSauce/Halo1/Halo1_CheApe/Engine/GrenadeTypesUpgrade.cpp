/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Engine/GrenadeTypesUpgrade.hpp"

#include <TagGroups/TagGroupDefinitions.hpp>
#include <TagGroups/Halo1/game_globals_definitions.hpp>
#include <TagGroups/Halo1/scenario_definitions.hpp>

#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace Objects { namespace Items {

		// How many new types OS adds
		static const int k_unit_grenade_types_new_count = Enums::k_unit_grenade_types_count_yelo - Enums::k_unit_grenade_types_count;

		static bool g_grenade_types_upgrade_enabled;

		ENGINE_PTR(string_list, global_grenade_type_enum, 0x9D1728, 0x6DE850, 0xA496AC);
		ENGINE_PTR(tag_block_definition, grenades_block, 0x9D237C, 0x6DF4A4, 0xA4A2FC);

		static cstring global_grenade_type_yelo_enum_strings[Enums::k_unit_grenade_types_count_yelo] = {
			"",
			"",
			"custom 2",
			"custom 3",
		};
		static string_list global_grenade_type_yelo_enum = {
			NUMBEROF(global_grenade_type_yelo_enum_strings),
			global_grenade_type_yelo_enum_strings
		};

		static cstring k_scenario_profiles_block_unused_grenade_count_field_name = "starting <unknown> grenade count";
		static cstring k_scenario_profiles_block_unused_grenade_count_field_names_yelo[k_unit_grenade_types_new_count] = {
			"starting custom-2 grenade count",
			"starting custom-3 grenade count",
		};
		static tag_field* g_scenario_profiles_block_unused_grenade_count_fields;
		static tag_field* scenario_profiles_block_find_unused_grenade_count_fields()
		{
			tag_group_definition* scnr = Yelo::tag_group_get(TagGroups::scenario::k_group_tag);

			int32 field_index = NONE;
			field_index = TagGroups::tag_block_find_field(scnr->definition, Enums::_field_block, "player starting profile");
			if(field_index == NONE)
			{
				YELO_ERROR(_error_message_priority_warning, 
					"CheApe: scenario_profiles_block not found!");

				return NULL;
			}

			tag_block_definition* scenario_profiles_block_def = scnr->definition->fields[field_index].Definition<tag_block_definition>();
			field_index = TagGroups::tag_block_find_field(scenario_profiles_block_def, Enums::_field_char_integer, k_scenario_profiles_block_unused_grenade_count_field_name);
			if(field_index == NONE)
			{
				YELO_ERROR(_error_message_priority_warning, 
					"CheApe: scenario_profiles_block unused grenade count fields not found!");

				return NULL;
			}

			return &scenario_profiles_block_def->fields[field_index];
		}


		void GrenadeTypesUpgradeInitialize()
		{
			using namespace Enums;

			string_list* types_enum = GET_PTR2(global_grenade_type_enum);
			tag_block_definition* block_definition = GET_PTR2(grenades_block);
			assert(types_enum->count == k_unit_grenade_types_count && block_definition->max_elements == k_unit_grenade_types_count);

			global_grenade_type_yelo_enum_strings[_unit_grenade_type_frag] = types_enum->strings[_unit_grenade_type_frag];
			global_grenade_type_yelo_enum_strings[_unit_grenade_type_plasma] = types_enum->strings[_unit_grenade_type_plasma];

#if PLATFORM_ID != PLATFORM_TOOL
			GrenadeTypesUpgrade(true);
#endif
		}
		void GrenadeTypesUpgradeDispose()
		{
		}

		void GrenadeTypesUpgradeTagDefinitions(bool enabled);
		void GrenadeTypesUpgradeInitializeTagDefinitions()
		{
			g_scenario_profiles_block_unused_grenade_count_fields = scenario_profiles_block_find_unused_grenade_count_fields();

			GrenadeTypesUpgradeTagDefinitions(g_grenade_types_upgrade_enabled);
		}
		void GrenadeTypesUpgradeDisposeTagDefinitions()
		{
			if(g_grenade_types_upgrade_enabled)
				GrenadeTypesUpgradeTagDefinitions(false);
		}


		void GrenadeTypesUpgrade(bool enabled)
		{
			g_grenade_types_upgrade_enabled = enabled;

			string_list* global_grenade_type_enum = GET_PTR2(global_grenade_type_enum);
			global_grenade_type_enum->strings = global_grenade_type_yelo_enum.strings;
			global_grenade_type_enum->count = enabled ? Enums::k_unit_grenade_types_count_yelo : Enums::k_unit_grenade_types_count;

			tag_block_definition* block_definition = GET_PTR2(grenades_block);
			block_definition->max_elements = global_grenade_type_enum->count;
		}
		
		static void GrenadeTypesUpgradeTagDefinitions(bool enabled)
		{
			if(g_scenario_profiles_block_unused_grenade_count_fields == NULL) return;

			//////////////////////////////////////////////////////////////////////////
			// Fixup the editor field names for scenario_profiles_block
			for(int x = 0; x < k_unit_grenade_types_new_count; x++)
			{
				cstring field_name = NULL;
				if(enabled)
					field_name = k_scenario_profiles_block_unused_grenade_count_field_names_yelo[x];
				else
					field_name = k_scenario_profiles_block_unused_grenade_count_field_name;

				g_scenario_profiles_block_unused_grenade_count_fields[x].name = field_name;
			}
		}

	}; };
};