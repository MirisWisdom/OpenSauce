/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/


//////////////////////////////////////////////////////////////////////////
// GrenadeTypesUpgrade.cpp
#if __EL_INCLUDE_FILE_ID == __EL_OBJECTS_GRENADE_TYPES_UPGRADE
	FUNC_PTR(GAME_GLOBALS_POSTPROCESS_GRENADE_COUNT_MOD, 0x491751, 0x48D561, 0x5A7951);
	ENGINE_PTR(string_list, global_grenade_type_enum, 0x9D1728, 0x6DE850, 0xA496A8);
	ENGINE_PTR(tag_block_definition, grenades_block, 0x9D237C, 0x6DF4A4, 0xA4A2FC);

	#if PLATFORM_TYPE == PLATFORM_SAPIEN
		static uintptr_t K_UNIT_ZOOM_LEVEL_OFFSET_REFS[] = {
			0x5861BC+4,
			0x58F2CC+2, 0x58F327+2,
			0x58F39B+2,
			0x59265A+2,
			0x59328B+2, 0x593299+2, 0x593315+2, 0x593A45+2,
			//0x64EF78+2,
			0x65804D+2,
		};
		static uintptr_t K_UNIT_DESIRED_ZOOM_LEVEL_OFFSET_REFS[] = {
			0x4ADDF5+4,
			0x58E2B7+2,
			0x58F32E+2,
			0x592660+2,
			0x593285+2,
			0x6339C9+4,
			//0x6507D2+4,
		};

		static uintptr_t K_NUMBER_OF_UNIT_GRENADE_TYPES_REFS[] = {
			0x5295DB+1, // 32bit
			0x588AC5+3, // 8bit
			0x588E53+3, // 8bit
			0x588EB0+1, // 8bit
			0x588F79+1, // 8bit
			0x589C78+4, // 32bit
			0x5906D7+1, // 8bit memcpy
			0x5928AA+2, // 16bit
			0x593254+1, // 8bit
			0x6507F2+1, // 8bit memcpy
			0x65096E+1, // 8bit memcpy
			0x650C6C+1, // 8bit memcpy
			0x65108A+1, // 8bit memcpy
			0x65B698+1, // 32bit
			0x65BE70+1, // 32bit
			0x697CBD+1, // 8bit memcpy
			0x6CAC7D+1, // 32bit unit_get_total_grenade_count
		};
		static uintptr_t K_MAXIMUM_UNIT_GRENADE_INDEX_REFS[] = {
			0x58783F+2, // 16bit
		};

		FUNC_PTR(PLAYER_ADD_EQUIPMENT_UNIT_GRENADE_COUNT_MOD,		NULL, NULL, 0x529526);
	#endif

//////////////////////////////////////////////////////////////////////////
// .cpp
#elif __EL_INCLUDE_FILE_ID == __EL_OBJECTS_EQUIPMENT


#else
	#error Undefined engine layout include for: __EL_INCLUDE_OBJECTS
#endif