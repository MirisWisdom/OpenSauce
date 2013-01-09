/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

const size_t s_unit_data::k_offset_zoom_level = FIELD_OFFSET(s_unit_datum, unit.zoom_level);
const size_t s_unit_data::k_offset_desired_zoom_level = FIELD_OFFSET(s_unit_datum, unit.desired_zoom_level);

const size_t s_unit_data::k_offset_zoom_level_yelo = FIELD_OFFSET(s_unit_datum, unit.zoom_level_yelo);
const size_t s_unit_data::k_offset_desired_zoom_level_yelo = FIELD_OFFSET(s_unit_datum, unit.desired_zoom_level_yelo);

// DEBUG: Before each memory write when we're enabling, we assert we're overwriting the expected value or bytes

static void InitializeGrenadeCounts_UnitZoomLevelRefs(bool enabled)
{
	uint32 offset = enabled ? s_unit_data::k_offset_zoom_level_yelo : s_unit_data::k_offset_zoom_level;

	for(int x = 0; x < NUMBEROF(K_UNIT_ZOOM_LEVEL_OFFSET_REFS); x++)
	{
		uint32* offset_ref = CAST_PTR(uint32*, K_UNIT_ZOOM_LEVEL_OFFSET_REFS[x]);

		DebugRunOnce( ASSERT( s_unit_data::k_offset_zoom_level == *offset_ref, "GrenadeCounts asm mismtach!" ) );
		*offset_ref = offset;
	}
}
static void InitializeGrenadeCounts_UnitDesiredZoomLevelRefs(bool enabled)
{
	uint32 offset = enabled ? s_unit_data::k_offset_desired_zoom_level_yelo : s_unit_data::k_offset_desired_zoom_level;

	for(int x = 0; x < NUMBEROF(K_UNIT_DESIRED_ZOOM_LEVEL_OFFSET_REFS); x++)
	{
		uint32* offset_ref = CAST_PTR(uint32*, K_UNIT_DESIRED_ZOOM_LEVEL_OFFSET_REFS[x]);

		DebugRunOnce( ASSERT( s_unit_data::k_offset_desired_zoom_level == *offset_ref, "GrenadeCounts asm mismtach!" ) );
		*offset_ref = offset;
	}
}
static void InitializeGrenadeCounts_NumberOfUnitGrenadeTypes(uint32 count)
{
	for(int x = 0; x < NUMBEROF(K_NUMBER_OF_UNIT_GRENADE_TYPES_REFS); x++)
	{
		uint32* count_ref = CAST_PTR(uint32*, K_NUMBER_OF_UNIT_GRENADE_TYPES_REFS[x]);

		DebugRunOnce( ASSERT( Enums::k_unit_grenade_types_count == *count_ref, "GrenadeCounts asm mismtach!" ) );
		*count_ref = count;
	}

	//////////////////////////////////////////////////////////////////////////
	// patch the maximum unit grenade index
	count--;

	for(int x = 0; x < NUMBEROF(K_MAXIMUM_UNIT_GRENADE_INDEX_REFS_32bit); x++)
	{
		uint32* count_ref = CAST_PTR(uint32*, K_MAXIMUM_UNIT_GRENADE_INDEX_REFS_32bit[x]);

		DebugRunOnce( ASSERT( Enums::k_unit_grenade_types_count-1 == *count_ref, "GrenadeCounts asm mismtach!" ) );
		*count_ref = count;
	}
	for(int x = 0; x < NUMBEROF(K_MAXIMUM_UNIT_GRENADE_INDEX_REFS_8bit); x++)
	{
		byte* count_ref = CAST_PTR(byte*, K_MAXIMUM_UNIT_GRENADE_INDEX_REFS_8bit[x]);

		DebugRunOnce( ASSERT( Enums::k_unit_grenade_types_count-1 == *count_ref, "GrenadeCounts asm mismtach!" ) );
		*count_ref = CAST(byte, count);
	}
}
