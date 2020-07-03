/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/Campaign.hpp"
#if !PLATFORM_IS_DEDI

#include "Memory/MemoryInterface.hpp"

// Note: Only use 'Campaign.inl' (in the "Impl" virtual folder) for declaring campaign levels,
// all other definitions should be automated via 'DEFINE_CAMPAIGN_LEVEL'

namespace Yelo
{
	namespace Enums
	{
		enum campaign_level
		{
#define DEFINE_CAMPAIGN_LEVEL(name, path) _campaign_level_##name,

#include "Game/Campaign.inl" // Create an enumeration of our campaign levels
			_campaign_level,

#undef DEFINE_CAMPAIGN_LEVEL


			// Original amount of campaign levels the engine expected
			k_campaign_level_base_count = 10,

			// How many campaign levels we've added to the original count
			k_campaign_level_new_count = _campaign_level - k_campaign_level_base_count,

			// How many campaign levels we've defined
// 			k_campaign_level_total_count = k_campaign_level_new_count >= 0 ?
// 				k_campaign_level_base_count + k_campaign_level_new_count
// 				:
// 				_campaign_level,
		};

		// Make sure the user heeded our warning and didn't remove existing campaign 
		// levels
		BOOST_STATIC_ASSERT(_campaign_level >= k_campaign_level_base_count);
	};

	namespace Campaign
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_CAMPAIGN
#include "Memory/_EngineLayout.inl"

#pragma region Scenario Paths
#if YELO_CAMPAIGN_UPGRADES_ENABLED

	#define DEFINE_CAMPAIGN_LEVEL(name, path) path,
		static cstring ScenarioPaths[] = { // this is needed for displaying the title\bitmap in the campaign selection in the UI
			
	#include "Game/Campaign.inl" // create a path list of our campaign levels

		};
	#undef DEFINE_CAMPAIGN_LEVEL


		static void ScenarioPathsInitialize()
		{
			for(auto ptr : ScenarioPathsReferences)
				*ptr = ScenarioPaths;

			for(auto ptr : ScenarioPathsReferences2)
				*ptr = ScenarioPaths;
		}
#endif
#pragma endregion


#pragma region Scenario Help
#if YELO_CAMPAIGN_UPGRADES_ENABLED
	#define DEFINE_CAMPAIGN_LEVEL(name, path) {#name, "ui\\shell\\solo_game\\player_help\\player_help_screen_" #name},
		static struct s_scenario_help_mapping {
			cstring name;
			cstring widget_definition;
		}ScenarioHelpMappings[] = { // this is needed for displaying the title\bitmap in the campaign selection in the UI

	#include "Game/Campaign.inl" // create a path list of our campaign levels

		};
	#undef DEFINE_CAMPAIGN_LEVEL

	static cstring ScenarioHelpMappingGetWidgetName(char* scenario_name)
	{
		for(auto& mapping : ScenarioHelpMappings)
			if( strstr(scenario_name, mapping.name) )
				return mapping.widget_definition;

		return nullptr;
	}

	API_FUNC_NAKED static void PLATFORM_API DisplayScenarioHelpHack()
	{
		static const uintptr_t SUCCESS_CALL_ADDR = GET_FUNC_PTR(DISPLAY_SCENARIO_HELP_SUCCESS);

		static const uintptr_t FAIL_CALL_ADDR = GET_FUNC_PTR(DISPLAY_SCENARIO_HELP_FAIL);

		__asm {
			// edx = char scenario_name[256]
			// Note: The code we jump back to in display_scenario_help doesn't expect any 
			// registers to have a specific value (save for 'eax', which we return) 
			// so we don't have to worry about the compiler overwriting ecx or esi 
			// without preserving them on the stack in 'ScenarioHelpMappingGetWidgetName's code

			push	edx
			call	ScenarioHelpMappingGetWidgetName
			test	eax, eax
			jz		mapping_not_found
			jmp		SUCCESS_CALL_ADDR

mapping_not_found:
			jmp		FAIL_CALL_ADDR
		}
	}

	static void DisplayScenarioHelpInitialize()
	{
		Memory::WriteRelativeJmp(Campaign::DisplayScenarioHelpHack, GET_FUNC_VPTR(DISPLAY_SCENARIO_HELP_HOOK), true);
	}
#endif
#pragma endregion


#pragma region Campaign Level Data
#if YELO_CAMPAIGN_UPGRADES_ENABLED
		struct {
			cstring ScenarioPath;
			PAD8; // bools
			PAD8;
			PAD8;
			PAD8;
		}CampaignLevelData[Enums::_campaign_level];

		static void CampaignLevelDataInitialize()
		{
			for(auto ptr : CampaignLevelDataReferences)
				*ptr = CampaignLevelData;

			for(auto ptr : CampaignLevelDataSizeReferences)
				*ptr = sizeof(CampaignLevelData) / 4;
		}
#endif
#pragma endregion


#pragma region Campaign Level Count
#if YELO_CAMPAIGN_UPGRADES_ENABLED
		static void CampaignLevelCountInitialize()
		{
			for(auto ptr : CampaignLevelCountReferences_8bit)
				*ptr = Enums::_campaign_level-1;
			for(auto ptr : CampaignLevelCountReferences_16bit)
				*ptr = Enums::_campaign_level-1;
			for(auto ptr : CampaignLevelCountReferences_32bit)
				*ptr = Enums::_campaign_level-1;

			for(auto ptr : CampaignLevelCountReferences2_8bit)
				*ptr = Enums::_campaign_level;
			for(auto ptr : CampaignLevelCountReferences2_16bit)
				*ptr = Enums::_campaign_level;
		}
#endif
#pragma endregion


		void Initialize()
		{
#if YELO_CAMPAIGN_UPGRADES_ENABLED
			if(Enums::_campaign_level > Enums::k_campaign_level_base_count) // only change the game if we're greater than the normal level count
			{
				ScenarioPathsInitialize();
				DisplayScenarioHelpInitialize();
				CampaignLevelDataInitialize();
				CampaignLevelCountInitialize();
			}
#endif
		}

		void Dispose()
		{
#if YELO_CAMPAIGN_UPGRADES_ENABLED
// 			if(Enums::_campaign_level > Enums::k_campaign_level_base_count) // only change the game if we're greater than the normal level count
// 			{
// 			}
#endif
		}
	};
};

#endif