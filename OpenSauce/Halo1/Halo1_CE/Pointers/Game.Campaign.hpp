/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Pointers/1.10/HaloCE_110_Runtime.Game.inl"
#include "Pointers/1.10/HaloCE_110_Runtime_Manual.Game.inl"
#else
#include "Pointers/1.10/HaloCE_110_Dedi.Game.inl"
#include "Pointers/1.10/HaloCE_110_Dedi_Manual.Game.inl"
#endif

#include <YeloLib/memory/memory_interface_base.hpp>
#include <Memory/MemoryInterface.hpp>

namespace Yelo
{
	namespace Campaign
	{
#if YELO_CAMPAIGN_UPGRADES_ENABLED
#pragma region Scenario Paths
		static cstring** ScenarioPathsReferences[] = {
			CAST_PTR(cstring**, K_SCENARIO_PATHS_REFERENCE_0),
			CAST_PTR(cstring**, K_SCENARIO_PATHS_REFERENCE_1),
			CAST_PTR(cstring**, K_SCENARIO_PATHS_REFERENCE_2),
			CAST_PTR(cstring**, K_SCENARIO_PATHS_REFERENCE_3),
			CAST_PTR(cstring**, K_SCENARIO_PATHS_REFERENCE_4),
			//CAST_PTR(cstring**, K_SCENARIO_PATHS_REFERENCE_5),
		};

		static cstring** ScenarioPathsReferences2[] = {
			CAST_PTR(cstring**, K_SCENARIO_PATHS_REFERENCE2_0),
			CAST_PTR(cstring**, K_SCENARIO_PATHS_REFERENCE2_1),
			CAST_PTR(cstring**, K_SCENARIO_PATHS_REFERENCE2_2),
			CAST_PTR(cstring**, K_SCENARIO_PATHS_REFERENCE2_3),
		};
#pragma endregion

#pragma region Scenario Help
		FUNC_PTR(DISPLAY_SCENARIO_HELP_HOOK, K_DISPLAY_SCENARIO_HELP_HOOK, FUNC_PTR_NULL);
		FUNC_PTR(DISPLAY_SCENARIO_HELP_SUCCESS, K_DISPLAY_SCENARIO_HELP_SUCCESS, FUNC_PTR_NULL);
		FUNC_PTR(DISPLAY_SCENARIO_HELP_FAIL, K_DISPLAY_SCENARIO_HELP_FAIL, FUNC_PTR_NULL);
#pragma endregion

#pragma region Campaign Level Data
		static void** CampaignLevelDataReferences[] = {
			CAST_PTR(void**, K_CAMPAIGN_LEVEL_DATA_REFERENCE_0),
			CAST_PTR(void**, K_CAMPAIGN_LEVEL_DATA_REFERENCE_1),
			CAST_PTR(void**, K_CAMPAIGN_LEVEL_DATA_REFERENCE_2),
			CAST_PTR(void**, K_CAMPAIGN_LEVEL_DATA_REFERENCE_3),
			//CAST_PTR(void**, K_CAMPAIGN_LEVEL_DATA_REFERENCE_4),
			//CAST_PTR(void**, K_CAMPAIGN_LEVEL_DATA_REFERENCE_5),
			//CAST_PTR(void**, K_CAMPAIGN_LEVEL_DATA_REFERENCE_6),
		};

		static size_t* CampaignLevelDataSizeReferences[] = {
			CAST_PTR(size_t*, K_CAMPAIGN_LEVEL_DATA_SIZE_REFERENCE_0),
			CAST_PTR(size_t*, K_CAMPAIGN_LEVEL_DATA_SIZE_REFERENCE_1),
			CAST_PTR(size_t*, K_CAMPAIGN_LEVEL_DATA_SIZE_REFERENCE_2),
		};
#pragma endregion

#pragma region Campaign Level Count
		static byte* CampaignLevelCountReferences_8bit[] = {
			CAST_PTR(byte*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE_8BIT_0),
			//CAST_PTR(byte*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE_8BIT_1),
		};

		static int16* CampaignLevelCountReferences_16bit[] = {
			CAST_PTR(int16*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE_16BIT_0),
		};

		static int32* CampaignLevelCountReferences_32bit[] = {
			CAST_PTR(int32*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE_32BIT_0),
			//CAST_PTR(int32*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE_32BIT_1),
		};

		//////////////////////////////////////////////////////////////////////////

		static byte* CampaignLevelCountReferences2_8bit[] = {
			CAST_PTR(byte*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_0),
			CAST_PTR(byte*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_1),
			CAST_PTR(byte*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_2),
			CAST_PTR(byte*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_3),
			CAST_PTR(byte*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_4),
			CAST_PTR(byte*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_5),
			//CAST_PTR(byte*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_6),
		};

		static int16* CampaignLevelCountReferences2_16bit[] = {
			CAST_PTR(int16*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_16BIT_0),
			CAST_PTR(int16*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_16BIT_1),
			//CAST_PTR(int16*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_16BIT_2),
		};
#pragma endregion
#endif
	}
}
