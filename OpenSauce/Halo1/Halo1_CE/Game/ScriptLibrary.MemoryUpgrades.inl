/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
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

#include "Game/ScriptLibrary.Fixups.inl"

static void MemoryUpgradesInitialize()
{
	InitializeLibraryFixups();

	//////////////////////////////////////////////////////////////////////////
	// Add the game's functions/globals to our upgraded memory
	static const rsize_t K_HS_FUNCTION_TABLE_COUNT = 
#if PLATFORM_IS_DEDI
		0x209;
#else
		0x211;
#endif

	static const rsize_t K_HS_EXTERNAL_GLOBALS_COUNT = 0x1EC
		- 1 // NOTE: we don't copy the 'rasterizer_frame_drop_ms' definition as its not defined in the tools
		;

	for(rsize_t x = 0, index = 0; 
		x < K_HS_FUNCTION_TABLE_COUNT; 
		index++)
	{
		if(_upgrade_globals.functions.table[index] == nullptr)
		{
			_upgrade_globals.functions.table[index] = hs_function_table[x++];
			_upgrade_globals.functions.count++;
		}
	}

	for(rsize_t x = 0, index = 0; 
		x < K_HS_EXTERNAL_GLOBALS_COUNT; 
		index++)
	{
		if(_upgrade_globals.globals.table[index] == nullptr)
		{
			hs_global_definition& glob = 
				*(_upgrade_globals.globals.table[index] = hs_external_globals[x++]);
			_upgrade_globals.globals.count++;

			// Is this the global who we want to override for exposing opensauce status?
			if(strcmp(glob.name, k_external_global_opensauce_override_name) == 0)
				InitializeExternalGlobalOpenSauceOverride(glob);
		}
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Add Yelo's functions/globals to our upgraded memory
	{
		_upgrade_globals.functions.yelo_start_index = _upgrade_globals.functions.count;
		for(size_t x = 0; x < K_HS_YELO_FUNCTION_COUNT; x++,
			_upgrade_globals.functions.count++)
			_upgrade_globals.functions.table[_upgrade_globals.functions.count] = 
				hs_yelo_functions[x];

		_upgrade_globals.globals.yelo_start_index = _upgrade_globals.globals.count;
		for(size_t x = 0; x < K_HS_YELO_GLOBALS_COUNT; x++,
			_upgrade_globals.globals.count++)
			_upgrade_globals.globals.table[_upgrade_globals.globals.count] = 
				hs_yelo_globals[x];
	}
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// Update the game code to use OUR function/global definition tables
	{
		hs_function_definition**** definitions = CAST_PTR(hs_function_definition****, K_HS_FUNCTION_TABLE_REFERENCES);
		const size_t k_count = NUMBEROF(K_HS_FUNCTION_TABLE_REFERENCES);

		for(size_t x = 0; x < k_count; x++)
			*definitions[x] = &_upgrade_globals.functions.table[0];
	}

	{
		hs_global_definition**** definitions = CAST_PTR(hs_global_definition****, K_HS_EXTERNAL_GLOBALS_REFERENCES);
		const size_t k_count = NUMBEROF(K_HS_EXTERNAL_GLOBALS_REFERENCES);

		for(size_t x = 0; x < k_count; x++)
			*definitions[x] = &_upgrade_globals.globals.table[0];
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Update the game code to use OUR function/global definition table's counts
	{
		int32 count = _upgrade_globals.functions.count;
		for(auto ptr : K_HS_FUNCTION_TABLE_COUNT_REFERENCES_16bit)
			*ptr = CAST(int16, count);
		for(auto ptr : K_HS_FUNCTION_TABLE_COUNT_REFERENCES_32bit)
			*ptr = count;

		count = _upgrade_globals.globals.count;
		for(auto ptr : K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCES_16bit)
			*ptr = CAST(int16, count);
		for(auto ptr : K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCES_32bit)
			*ptr = count;
	}
	//////////////////////////////////////////////////////////////////////////
}