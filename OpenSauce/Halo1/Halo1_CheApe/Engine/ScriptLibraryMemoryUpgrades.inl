/*
    Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

static struct s_upgrade_globals {
	struct {
		int32 count;
		int32 capacity;
		hs_function_definition* table[Enums::k_hs_script_functions_count_upgrade];
		int32 yelo_start_index; // starting index of our custom 'yelo' functions
	}functions;

	struct {
		int32 count;
		int32 capacity;
		hs_global_definition* table[Enums::k_hs_external_globals_count_upgrade];
		int32 yelo_start_index; // starting index of our custom 'yelo' globals
	}globals;
}_upgrade_globals = {
	{0, Enums::k_hs_script_functions_count_upgrade},
	{0, Enums::k_hs_external_globals_count_upgrade}
};

// If [use_upgrades] is false, the code will revert the HEK modifications back to stock values
static void MemoryUpgradesSyntaxInitialize(bool use_upgrades = true)
{
	static bool memory_is_upgraded = false;
	if(memory_is_upgraded == use_upgrades) return;
	memory_is_upgraded = use_upgrades;

	//////////////////////////////////////////////////////////////////////////
	// Addresses we'll be updating
	static uint32* K_MAX_HS_SYNTAX_NODES_PER_SCENARIO_UPGRADE_ADDRESS_LIST[] = {
		CAST_PTR(uint32*, PLATFORM_VALUE(0x4F130C, 0x4C1AEC, 0x5834EC)),
	};

	static uint32* K_TOTAL_SCENARIO_HS_SYNTAX_DATA_UPGRADE_ADDRESS_LIST[] = {
		//CAST_PTR(uint32*, PLATFORM_VALUE(0x4F12FF, 0x4C1ADF, 0x5834DF)), // don't modify this one, modify the size check using the address below
		CAST_PTR(uint32*, PLATFORM_VALUE(0x4F1367, 0x4C1B47, 0x583547)),
		CAST_PTR(uint32*, PLATFORM_VALUE(0xA132A4, 0x722EDC, 0xA84B74)),
	};
	static byte* K_ADDRESS_OF_SCENARIO_HS_SYNTAX_DATA_SIZE_CHECK = CAST_PTR(byte*, PLATFORM_VALUE(0x4F1303, 0x4C1AE3, 0x5834E3));
	//////////////////////////////////////////////////////////////////////////


	for(int32 x = 0; x < NUMBEROF(K_MAX_HS_SYNTAX_NODES_PER_SCENARIO_UPGRADE_ADDRESS_LIST); x++)
		*K_MAX_HS_SYNTAX_NODES_PER_SCENARIO_UPGRADE_ADDRESS_LIST[x] = use_upgrades ? 
			Enums::k_maximum_hs_syntax_nodes_per_scenario_upgrade : Enums::k_maximum_hs_syntax_nodes_per_scenario;
	
	for(int32 x = 0; x < NUMBEROF(K_TOTAL_SCENARIO_HS_SYNTAX_DATA_UPGRADE_ADDRESS_LIST); x++)
		*K_TOTAL_SCENARIO_HS_SYNTAX_DATA_UPGRADE_ADDRESS_LIST[x] = use_upgrades ? 
		Enums::k_total_scenario_hs_syntax_data_upgrade : Enums::k_total_scenario_hs_syntax_data;

	// change from 'jz' (0x0F 0x84) to 'jge' (0x0F 0x8D)
	// This allows us to support scenarios with original script nodes, or with
	// Yelo based script nodes, which are larger (because of memory upgrades, duh)
	*(K_ADDRESS_OF_SCENARIO_HS_SYNTAX_DATA_SIZE_CHECK+1) = 0x8D;
}
static void MemoryUpgradesSyntaxStringDataInitialize()
{
	//////////////////////////////////////////////////////////////////////////
	// Addresses we'll be updating
	static uint32* K_TOTAL_SCENARIO_HS_STRING_DATA_UPGRADE_ADDRESS_LIST[] = {
		CAST_PTR(uint32*, PLATFORM_VALUE(0xA132B4, 0x722EEC, 0xA84B84)),
	};
	//////////////////////////////////////////////////////////////////////////

	for(int32 x = 0; x < NUMBEROF(K_TOTAL_SCENARIO_HS_STRING_DATA_UPGRADE_ADDRESS_LIST); x++)
		*K_TOTAL_SCENARIO_HS_STRING_DATA_UPGRADE_ADDRESS_LIST[x] = Enums::k_maximum_hs_string_data_per_scenario_upgrade;
}
static void MemoryUpgradesExternalGlobalsInitialize()
{
	//////////////////////////////////////////////////////////////////////////
	// Addresses we'll be updating
	static uint16* K_ADDRESS_OF_HS_EXTERNAL_GLOBALS_COUNT_CHECK_MOD = CAST_PTR(uint16*, PLATFORM_VALUE(0x4FA24F, 0x50705F, 0x62100F));
	static uint32* K_ADDRESS_OF_HS_EXTERNAL_GLOBALS_COUNT_CHECK_VALUE = CAST_PTR(uint32*, PLATFORM_VALUE(0x4FA253, 0x507063, 0x62101F));
	//////////////////////////////////////////////////////////////////////////

	// change from 'shl REG, 1' to 'nop' x2
	// This seems to used to limit the external globals to half of the total "hs globals" data array amount
	*K_ADDRESS_OF_HS_EXTERNAL_GLOBALS_COUNT_CHECK_MOD = 0x9090;
	// change from 'cmp REG, 400h'
	// Checks the external count with the real maximum amount of external globals (read: without scenario globals count)
	*K_ADDRESS_OF_HS_EXTERNAL_GLOBALS_COUNT_CHECK_VALUE = Enums::k_hs_external_globals_count_upgrade;
}

static void MemoryUpgradesInitialize()
{
	MemoryUpgradesSyntaxInitialize();
	MemoryUpgradesSyntaxStringDataInitialize();
	MemoryUpgradesExternalGlobalsInitialize();

	//////////////////////////////////////////////////////////////////////////
	// Add the game's functions/globals to our upgraded memory
	static const int32 K_HS_FUNCTION_TABLE_COUNT = 0x21F;
	static const int32 K_HS_EXTERNAL_GLOBALS_COUNT = 0x1EF;

	for(int32 x = 0, index = 0; 
		x < K_HS_FUNCTION_TABLE_COUNT; 
		index++)
	{
		if(_upgrade_globals.functions.table[index] == NULL)
		{
			_upgrade_globals.functions.table[index] = hs_function_table[x++];
			_upgrade_globals.functions.count++;
		}
	}

	for(int32 x = 0, index = 0; 
		x < K_HS_EXTERNAL_GLOBALS_COUNT; 
		index++)
	{
		if(_upgrade_globals.globals.table[index] == NULL)
		{
			_upgrade_globals.globals.table[index] = hs_external_globals[x++];
			_upgrade_globals.globals.count++;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Add Yelo's functions/globals to our upgraded memory
	{
		CheApe::s_cache_header& header = CheApe::GlobalCacheHeader();

		{
			hs_function_definition** hs_yelo_functions = 
				CAST_PTR(hs_function_definition**, header.Scripting.Functions.Address);

			_upgrade_globals.functions.yelo_start_index = _upgrade_globals.functions.count;
			for(int32 x = 0; x < header.Scripting.Functions.Count; x++,
				_upgrade_globals.functions.count++)
			{
				_upgrade_globals.functions.table[_upgrade_globals.functions.count] = 
					hs_yelo_functions[x];

				if(hs_yelo_functions[x]->paramc > 0)
					NullifyScriptFunctionWithParams(*hs_yelo_functions[x]);
				else
					NullifyScriptFunction(*hs_yelo_functions[x]);
			}
		}

		{
			hs_global_definition** hs_yelo_globals = 
				CAST_PTR(hs_global_definition**, header.Scripting.Globals.Address);

			_upgrade_globals.globals.yelo_start_index = _upgrade_globals.globals.count;
			for(int32 x = 0; x < header.Scripting.Globals.Count; x++,
				_upgrade_globals.globals.count++)
				_upgrade_globals.globals.table[_upgrade_globals.globals.count] = 
					hs_yelo_globals[x];
		}
	}
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// Update the game code to use OUR function/global definition tables
	static void* K_HS_FUNCTION_TABLE_REFERENCES[] = {
		CAST_PTR(void*, PLATFORM_VALUE(0x4F15C5, 0x4C1C35, 0x5837A5)),
		CAST_PTR(void*, PLATFORM_VALUE(0x4F15D4, 0x4C1C44, 0x5837B4)),
		CAST_PTR(void*, PLATFORM_VALUE(0x4F1898, 0x4C1EA8, 0x583A78)),
		CAST_PTR(void*, PLATFORM_VALUE(0x4F1B26, 0x4C20A6, 0x583D06)),
		CAST_PTR(void*, PLATFORM_VALUE(0x4F2496, 0x4C25F6, 0x584676)),
		CAST_PTR(void*, PLATFORM_VALUE(0x4F2572, 0x4C2A58, 0x584752)),
		CAST_PTR(void*, PLATFORM_VALUE(0x4F2948, 0x4C2B0D, 0x584B28)),
#if PLATFORM_ID != PLATFORM_TOOL
		CAST_PTR(void*, PLATFORM_VALUE(0x4F29FD, NULL, 0x584BDD)),
#endif
	};

	static void* K_HS_EXTERNAL_GLOBALS_REFERENCES[] = {
		CAST_PTR(void*, PLATFORM_VALUE(0x4F16D7, 0x4C1CE7, 0x5838B7)),
		CAST_PTR(void*, PLATFORM_VALUE(0x4F16E6, 0x4C1CF6, 0x5838C6)),
		CAST_PTR(void*, PLATFORM_VALUE(0x4F17B3, 0x4C1DC3, 0x583993)),
		CAST_PTR(void*, PLATFORM_VALUE(0x4F1A44, 0x4C1FF4, 0x583C24)),
		CAST_PTR(void*, PLATFORM_VALUE(0x4F1C50, 0x4C21D0, 0x583E30)),
	};
	
	{
		hs_function_definition**** definitions = CAST_PTR(hs_function_definition****, K_HS_FUNCTION_TABLE_REFERENCES);
		const int32 k_count = NUMBEROF(K_HS_FUNCTION_TABLE_REFERENCES);

		for(int32 x = 0; x < k_count; x++)
			*definitions[x] = &_upgrade_globals.functions.table[0];
	}

	{
		hs_global_definition**** definitions = CAST_PTR(hs_global_definition****, K_HS_EXTERNAL_GLOBALS_REFERENCES);
		const int32 k_count = NUMBEROF(K_HS_EXTERNAL_GLOBALS_REFERENCES);

		for(int32 x = 0; x < k_count; x++)
			*definitions[x] = &_upgrade_globals.globals.table[0];
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Update the game code to use OUR function/global definition table's counts
	static int16* K_HS_FUNCTION_TABLE_COUNT_REFERENCES_16bit[] = {
		CAST_PTR(int16*, PLATFORM_VALUE(0x4F159E, 0x4C1C0E, 0x58377E)),
		CAST_PTR(int16*, PLATFORM_VALUE(0x4F18B1, 0x4C1EC1, 0x583A91)),
		CAST_PTR(int16*, PLATFORM_VALUE(0x4F1B38, 0x4C20B8, 0x583D18)),
		CAST_PTR(int16*, PLATFORM_VALUE(0x4F1B64, 0x4C20E4, 0x583D44)),
		CAST_PTR(int16*, PLATFORM_VALUE(0x4F1B99, 0x4C2119, 0x583D79)),
		CAST_PTR(int16*, PLATFORM_VALUE(0x4F246C, 0x4C25CC, 0x58464C)),
#if PLATFORM_ID != PLATFORM_TOOL
		CAST_PTR(int16*, PLATFORM_VALUE(0x4F2548, NULL, 0x584728)),
#endif
		CAST_PTR(int16*, PLATFORM_VALUE(0x4F291E, 0x4C2A2E, 0x584AFE)),
		CAST_PTR(int16*, PLATFORM_VALUE(0x4F2A09, 0x4C2B19, 0x584BE9)),
		CAST_PTR(int16*, PLATFORM_VALUE(0x4F2A51, 0x4C2B61, 0x584C31)),
		CAST_PTR(int16*, PLATFORM_VALUE(0x4F2AA0, 0x4C2BB0, 0x584C80)),
	};

	static int16* K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCES_16bit[] = {
		CAST_PTR(int16*, PLATFORM_VALUE(0x936A00, 0x6881D0, 0x99E818)),
	};

	{
		int32 count = _upgrade_globals.functions.count;
		for(int32 x = 0; x < NUMBEROF(K_HS_FUNCTION_TABLE_COUNT_REFERENCES_16bit); x++)
			*K_HS_FUNCTION_TABLE_COUNT_REFERENCES_16bit[x] = CAST(int16, count);


		count = _upgrade_globals.globals.count;
		for(int32 x = 0; x < NUMBEROF(K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCES_16bit); x++)
			*K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCES_16bit[x] = CAST(int16, count);
	}
	//////////////////////////////////////////////////////////////////////////
}