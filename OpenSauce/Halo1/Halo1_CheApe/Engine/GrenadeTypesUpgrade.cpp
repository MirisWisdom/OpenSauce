/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Engine/GrenadeTypesUpgrade.hpp"

#include <blamlib/Halo1/game/game_globals.hpp>
#include <blamlib/Halo1/game/game_globals_definitions.hpp>
#include <blamlib/Halo1/scenario/scenario.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <blamlib/Halo1/units/biped_structures.hpp>
#include <blamlib/Halo1/units/unit_structures.hpp>

#include "Engine/EngineFunctions.hpp"
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace Objects
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_OBJECTS
#define __EL_INCLUDE_FILE_ID	__EL_OBJECTS_GRENADE_TYPES_UPGRADE
#include "Memory/_EngineLayout.inl"
#if PLATFORM_TYPE == PLATFORM_SAPIEN
	#include <YeloLib/Halo1/units/units_grenade_count_upgrade.inl>
#endif
	};

	namespace Objects { namespace Items {
		// How many new types OS adds
		static const int k_unit_grenade_types_new_count = Enums::k_unit_grenade_types_count_yelo - Enums::k_unit_grenade_types_count;

		static bool g_grenade_types_upgrade_enabled;

		//////////////////////////////////////////////////////////////////////////
		// new grenade_type enum strings
		static cstring global_grenade_type_yelo_enum_strings[Enums::k_unit_grenade_types_count_yelo] = {
			"",
			"",
			"custom 2",
			"custom 3",
		};
		//////////////////////////////////////////////////////////////////////////
		// Scenario modifications
		static cstring k_scenario_profiles_block_unused_grenade_count_field_name = "starting <unknown> grenade count";
		static cstring k_scenario_profiles_block_unused_grenade_count_field_names_yelo[k_unit_grenade_types_new_count] = {
			"starting custom-2 grenade count",
			"starting custom-3 grenade count",
		};
		static tag_field* g_scenario_profiles_block_unused_grenade_count_fields;
		static tag_field* scenario_profiles_block_find_unused_grenade_count_fields()
		{
			tag_group* scnr = blam::tag_group_get<TagGroups::scenario>();

			int32 field_index = NONE;
			field_index = scnr->header_block_definition->FindFieldIndex(Enums::_field_block, "player starting profile");
			if(field_index == NONE)
			{
				YELO_WARN("CheApe: scenario_profiles_block not found!");

				return nullptr;
			}

			auto* scenario_profiles_block_def = scnr->header_block_definition->fields[field_index].Definition<tag_block_definition>();
			field_index = scenario_profiles_block_def->FindFieldIndex(Enums::_field_char_integer, k_scenario_profiles_block_unused_grenade_count_field_name);
			if(field_index == NONE)
			{
				YELO_WARN("CheApe: scenario_profiles_block unused grenade count fields not found!");

				return nullptr;
			}

			return &scenario_profiles_block_def->fields[field_index];
		}
		//////////////////////////////////////////////////////////////////////////


		void GrenadeTypesUpgradeInitialize()
		{
			using namespace Enums;

			string_list* types_enum = GET_PTR2(global_grenade_type_enum);
			tag_block_definition* block_definition = GET_PTR2(grenades_block);
			assert(types_enum->count == k_unit_grenade_types_count && block_definition->maximum_element_count == k_unit_grenade_types_count);

			global_grenade_type_yelo_enum_strings[_unit_grenade_type_frag] = types_enum->strings[_unit_grenade_type_frag];
			global_grenade_type_yelo_enum_strings[_unit_grenade_type_plasma] = types_enum->strings[_unit_grenade_type_plasma];

			GrenadeTypesUpgrade(true);
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

			//////////////////////////////////////////////////////////////////////////
			// Make the postprocess not error out on grenade_types.count != 2
			// TODO: Ideally, we should hook this part of the code to do our own checks with our own error, but bigger fish to fry right now
			// cmp     dword ptr [esi+128h], 2
			// modify the jz to be jge
			// jz      short 48D579
			byte* game_globals_postprocess_jmp_mod = CAST_PTR(byte*, GET_FUNC_VPTR(GAME_GLOBALS_POSTPROCESS_GRENADE_COUNT_MOD));
			// jge : jz
			*game_globals_postprocess_jmp_mod = enabled
				? Enums::_x86_opcode_jge_short
				: Enums::_x86_opcode_jz_short;
			//////////////////////////////////////////////////////////////////////////

			string_list* global_grenade_type_enum = GET_PTR2(global_grenade_type_enum);
			global_grenade_type_enum->strings = global_grenade_type_yelo_enum_strings;
			global_grenade_type_enum->count = enabled ? Enums::k_unit_grenade_types_count_yelo : Enums::k_unit_grenade_types_count;

			tag_block_definition* block_definition = GET_PTR2(grenades_block);
			block_definition->maximum_element_count = global_grenade_type_enum->count;

#if PLATFORM_TYPE == PLATFORM_SAPIEN
			InitializeGrenadeCounts_UnitZoomLevelRefs(enabled);
			InitializeGrenadeCounts_UnitDesiredZoomLevelRefs(enabled);
			InitializeGrenadeCounts_NumberOfUnitGrenadeTypes(global_grenade_type_enum->count);
#endif
		}
		
		static void GrenadeTypesUpgradeTagDefinitions(bool enabled)
		{
			if(g_scenario_profiles_block_unused_grenade_count_fields == nullptr) return;

			//////////////////////////////////////////////////////////////////////////
			// Fixup the editor field names for scenario_profiles_block
			for(int x = 0; x < k_unit_grenade_types_new_count; x++)
			{
				cstring field_name = nullptr;
				if(enabled)
					field_name = k_scenario_profiles_block_unused_grenade_count_field_names_yelo[x];
				else
					field_name = k_scenario_profiles_block_unused_grenade_count_field_name;

				g_scenario_profiles_block_unused_grenade_count_fields[x].name = field_name;
			}
		}

#if PLATFORM_TYPE == PLATFORM_SAPIEN
		void InitializeGrenadesForNewMap()
		{
			Items::GrenadeTypesUpgrade(GameState::GlobalGameGlobals()->grenades.Count > 2);
		}
#endif
	}; };
};