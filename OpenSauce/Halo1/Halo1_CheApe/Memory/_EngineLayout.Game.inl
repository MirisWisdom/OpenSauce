/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/


//////////////////////////////////////////////////////////////////////////
// EngineFunctions.cpp
#if __EL_INCLUDE_FILE_ID == __EL_GAME_ENGINE_FUNCTIONS
	namespace GameState
	{
		ENGINE_PTR(s_main_globals, main_globals,						0xAA6D40, 0xFC9B40, 0xBD7E28);

		ENGINE_PTR(s_physical_memory_map_globals, physical_memory_globals,	0xCBF510, 0xFC9010, 0xDB4688);
		
		ENGINE_PTR(GameState::s_game_globals*, game_globals,			PTR_NULL, PTR_NULL, 0xDF3694);
		ENGINE_PTR(s_game_state_globals, game_state_globals,			0xCB2888, 0xFC9038, 0xBD7D24);
		ENGINE_PTR(TagGroups::s_game_globals*, global_game_globals,		PTR_NULL, 0x12E577C, 0x107EDC8);
		

		s_main_globals* MainGlobals()									PTR_IMP_GET2(main_globals);
		s_physical_memory_map_globals* PhysicalMemoryMapGlobals()		PTR_IMP_GET2(physical_memory_globals);

		s_game_state_globals* GameStateGlobals()						PTR_IMP_GET2(game_state_globals);
		TagGroups::s_game_globals* GlobalGameGlobals()					PTR_IMP_GET(global_game_globals);
		
		GameState::s_game_globals* GameGlobals()						PTR_IMP_GET(game_globals);

		TagGroups::s_game_globals** GlobalGameGlobalsReference()		PTR_IMP_GET2(global_game_globals);
	};

	#if PLATFORM_TYPE == PLATFORM_SAPIEN
	namespace AI
	{
		ENGINE_DPTR(actor_data_t, actor_data,							PTR_NULL, PTR_NULL, 0xE459DC);
		ENGINE_DPTR(s_ai_globals_data, ai_globals,						PTR_NULL, PTR_NULL, 0x01057384);
		
		actor_data_t& Actors()											DPTR_IMP_GET_BYREF(actor_data)
		s_ai_globals_data* AIGlobals()									DPTR_IMP_GET(ai_globals);
	};

	namespace Objects
	{
		ENGINE_DPTR(object_header_data_t, object_header,				PTR_NULL, PTR_NULL, 0x01056E4C);
		ENGINE_PTR(s_object_name_list_data*, object_name_list,			PTR_NULL, PTR_NULL, 0xDB44D0);

		object_header_data_t& ObjectHeader()							DPTR_IMP_GET_BYREF(object_header);
		s_object_name_list_data* ObjectNameList()						PTR_IMP_GET(object_name_list);
	};
	#endif

	namespace GameUI
	{
		ENGINE_DPTR(s_first_person_weapons, first_person_weapons, PTR_NULL, 0x10C8EAC, 0xE00D44);
		s_first_person_weapons* FirstPersonWeapons()		DPTR_IMP_GET(first_person_weapons);
	};

	namespace Scenario
	{
		ENGINE_DPTR(s_scenario_globals, scenario_globals,				PTR_NULL, 0x12E576C, 0x107EDB8);

		ENGINE_PTR(TagGroups::scenario*, global_scenario,				PTR_NULL, 0x12E5764, 0x107EDA0);
		ENGINE_PTR(TagGroups::collision_bsp*, global_bsp3d,				PTR_NULL, 0x12E5768, 0x107EDB4);
		ENGINE_PTR(TagGroups::collision_bsp*, global_collision_bsp,		PTR_NULL, 0x12E5770, 0x107EDBC);
		ENGINE_PTR(TagGroups::structure_bsp*, global_structure_bsp,		PTR_NULL, 0x12E5778, 0x107EDC4);

		ENGINE_PTR(datum_index, global_scenario_index,					PTR_NULL, 0x6B9430, 0xA2A900);
		ENGINE_PTR(int16, structure_bsp_index,							PTR_NULL, 0x6B9434, 0xA2A904);


		s_scenario_globals* ScenarioGlobals()						DPTR_IMP_GET(scenario_globals);

		TagGroups::scenario* Scenario()								PTR_IMP_GET(global_scenario);
		TagGroups::collision_bsp* Bsp3d()							PTR_IMP_GET(global_bsp3d);
		TagGroups::collision_bsp* CollisionBsp()					PTR_IMP_GET(global_collision_bsp);
		TagGroups::structure_bsp* StructureBsp()					PTR_IMP_GET(global_structure_bsp);

		datum_index ScenarioIndex()									PTR_IMP_GET(global_scenario_index);
		int16 StructureBspIndex()									PTR_IMP_GET(structure_bsp_index);

		datum_index* GlobalScenarioIndexReference()					PTR_IMP_GET2(global_scenario_index);
		TagGroups::scenario** GlobalScenarioReference()				PTR_IMP_GET2(global_scenario);
		int16* GlobalStructureBspIndexReference()					PTR_IMP_GET2(structure_bsp_index);
		TagGroups::structure_bsp** GlobalStructureBspReference()	PTR_IMP_GET2(global_structure_bsp);
		TagGroups::collision_bsp** GlobalBspReference()				PTR_IMP_GET2(global_bsp3d);
		TagGroups::collision_bsp** GlobalCollisionBspReference()	PTR_IMP_GET2(global_collision_bsp);
	};

	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// ai/actors
		FUNC_PTR(ACTOR_DELETE,					FUNC_PTR_NULL, FUNC_PTR_NULL, 0x697770);
		FUNC_PTR(ACTOR_CUSTOMIZE_UNIT,			FUNC_PTR_NULL, FUNC_PTR_NULL, 0x697310);
		FUNC_PTR(ACTOR_BRAINDEAD,				FUNC_PTR_NULL, FUNC_PTR_NULL, 0x697E60);
		FUNC_PTR(ACTOR_CREATE_FOR_UNIT,			FUNC_PTR_NULL, FUNC_PTR_NULL, 0x699750);
		//////////////////////////////////////////////////////////////////////////
		// cseries/cseries
		FUNC_PTR(DISPLAY_ASSERT,	0x42C840, 0x435F10, 0x4F7000);
		//////////////////////////////////////////////////////////////////////////
		// cseries/debug_memory
		FUNC_PTR(DEBUG_DUMP_MEMORY, 0x446CE0, 0x43EA20, 0x5022C0);
		FUNC_PTR(DEBUG_MALLOC,		0x446970, 0x43E6B0, 0x501F50);
		FUNC_PTR(DEBUG_FREE,		0x446A80, 0x43E7C0, 0x502060);
		FUNC_PTR(DEBUG_REALLOC,		0x446B50, 0x43E890, 0x502130);
		//////////////////////////////////////////////////////////////////////////
		// cseries/errors
		proc_error error = PLATFORM_PTR(proc_error, 0x4206B0, 0x42CB90, 0x417500);
		FUNC_PTR(ERRORS_HANDLE,					0x4209F0, 0x42CE40, 0x417850);
		//////////////////////////////////////////////////////////////////////////
		// cseries/profile
		ENGINE_PTR(bool, g_profiling_enabled,	0xC81CB1, 0xFC9009, 0xDB2D21);
		FUNC_PTR(PROFILE_ENTER_PRIVATE,			0x475F30, 0x452970, 0x535490);
		FUNC_PTR(PROFILE_EXIT_PRIVATE,			0x475FB0, 0x4529F0, 0x535510);		
		//////////////////////////////////////////////////////////////////////////
		// game/game_allegiance
		FUNC_PTR(GAME_TEAM_IS_ENEMY,			FUNC_PTR_NULL, FUNC_PTR_NULL, 0x6773C0);
		//////////////////////////////////////////////////////////////////////////
		// hs/effects
		FUNC_PTR(HS_EFFECT_NEW_FROM_OBJECT_MARKER,	FUNC_PTR_NULL, FUNC_PTR_NULL, 0x6C9B90);
		//////////////////////////////////////////////////////////////////////////
		// hs/hs_scenario_definitions
		FUNC_PTR(HS_SCENARIO_POSTPROCESS,		0x4F26B0, 0x4C27C0, 0x584890);
		//////////////////////////////////////////////////////////////////////////
		// interface/ui_widget_group
		FUNC_PTR(UI_LOAD_TAGS_FOR_SCENARIO,		0x48ADA0, 0x488960, 0x5A37E0);
		//////////////////////////////////////////////////////////////////////////
		// items/weapons
		FUNC_PTR(WEAPON_PREVENTS_MELEE_ATTACK,				FUNC_PTR_NULL, FUNC_PTR_NULL, 0x669440);
		FUNC_PTR(WEAPON_PREVENTS_GRENADE_THROWING,			FUNC_PTR_NULL, FUNC_PTR_NULL, 0x6664D0);
		FUNC_PTR(WEAPON_STOP_RELOAD,						FUNC_PTR_NULL, FUNC_PTR_NULL, 0x66B520);
		FUNC_PTR(FIRST_PERSON_WEAPON_MESSAGE_FROM_UNIT,		FUNC_PTR_NULL, FUNC_PTR_NULL, 0x57B1C0);
		FUNC_PTR(WEAPON_GET_FIRST_PERSON_ANIMATION_TIME,	FUNC_PTR_NULL, FUNC_PTR_NULL, 0x666520);
		//////////////////////////////////////////////////////////////////////////
		// main/console
		FUNC_PTR(CONSOLE_PRINTF,				FUNC_PTR_NULL, 0x4885A0, 0x547C80);
		FUNC_PTR(CONSOLE_WARNING,				FUNC_PTR_NULL, 0x4886E0, 0x547DC0);
		//////////////////////////////////////////////////////////////////////////
		// math/periodic_functions
		FUNC_PTR(PERIODIC_FUNCTION_EVALUATE,	0x4DC780, 0x490250, 0x61A620);
		FUNC_PTR(TRANSITION_FUNCTION_EVALUATE,	0x4DC8C0, 0x490390, 0x61A760);
		//////////////////////////////////////////////////////////////////////////
		// memory/byte_swapping
		FUNC_PTR(BYTE_SWAP_DATA,				0x496820, 0x48FF90, 0x5ABC20);
		FUNC_PTR(BYTE_SWAP_DATA_EXPLICIT,		0x4968F0, 0x490060, 0x5ABCF0);
		//////////////////////////////////////////////////////////////////////////
		// memory/data
		FUNC_PTR(DATUM_TRY_AND_GET,				0x45CF40, 0x4759D0, 0x51CFC0);
		FUNC_PTR(DATUM_GET,						0x45CFF0, 0x475A80, 0x51D070);
		FUNC_PTR(DATA_VERIFY,					0x45D0C0, 0x475B50, 0x51D140);
		FUNC_PTR(DATA_NEW,						0x45D1A0, 0x475C30, 0x51D220);
		FUNC_PTR(DATA_DISPOSE,					0x45D1F0, 0x475C80, 0x51D270);
		FUNC_PTR(DATUM_NEW_AT_INDEX,			0x45D240, 0x475CD0, 0x51D2C0);
		FUNC_PTR(DATUM_NEW,						0x45D3A0, 0x475D80, 0x51D420);
		FUNC_PTR(DATUM_DELETE,					0x45D460, 0x475E40, 0x51D4E0);
		FUNC_PTR(DATA_DELETE_ALL,				0x45D4B0, 0x475E90, 0x51D530);
		FUNC_PTR(DATA_ITERATOR_NEXT,			0x45D5A0, 0x475F80, 0x51D620);
		FUNC_PTR(DATA_NEXT_INDEX,				0x45D670, 0x476050, 0x51D6F0);
		FUNC_PTR(DATA_MAKE_VALID,				0x45D890, 0x476170, 0x51D910);
		//////////////////////////////////////////////////////////////////////////
		// memory/memory_pool
		FUNC_PTR(MEMORY_POOL_INITIALIZE,		0x5E3AD0, 0x4EBA70, 0x6183F0);
		FUNC_PTR(MEMORY_POOL_VERIFY,			0x5E3BC0, 0x4EBB00, 0x6184E0);
		FUNC_PTR(MEMORY_POOL_NEW,				0x5E3DE0, 0x4EBD20, 0x618700);
		FUNC_PTR(MEMORY_POOL_DELETE,			0x5E3E20, FUNC_PTR_NULL, 0x618740);
		FUNC_PTR(MEMORY_POOL_BLOCK_ALLOCATE,	0x5E3E50, 0x4EBD60, 0x618770);
		FUNC_PTR(MEMORY_POOL_BLOCK_FREE,		0x5E3F30, 0x4EBE40, 0x618850);
		FUNC_PTR(MEMORY_POOL_DEFRAGMENT,		0x5E3FD0, 0x4EBEE0, 0x6188F0);
		FUNC_PTR(MEMORY_POOL_BLOCK_REALLOCATE,	0x5E4030, 0x4EBF40, 0x618950);
		//////////////////////////////////////////////////////////////////////////
		// models/models
		FUNC_PTR(MODEL_FIND_MARKER,								FUNC_PTR_NULL, FUNC_PTR_NULL, 0x55C910);
		FUNC_PTR(ANIMATION_CHOOSE_RANDOM_PERMUTATION_INTERNAL,	FUNC_PTR_NULL, FUNC_PTR_NULL, 0x59D380);
		//////////////////////////////////////////////////////////////////////////
		// objects/objects
		FUNC_PTR(OBJECT_NEW,					FUNC_PTR_NULL, FUNC_PTR_NULL, 0x568E50);
		FUNC_PTR(OBJECT_DELETE,					FUNC_PTR_NULL, FUNC_PTR_NULL, 0x564C80);
		FUNC_PTR(OBJECT_DETACH,					FUNC_PTR_NULL, FUNC_PTR_NULL, 0x5651E0);
		FUNC_PTR(OBJECT_ATTACH_TO_MARKER,		FUNC_PTR_NULL, FUNC_PTR_NULL, 0x568EA0);
		FUNC_PTR(OBJECTS_UPDATE,				FUNC_PTR_NULL, FUNC_PTR_NULL, 0x5699F0);
		FUNC_PTR(OBJECT_PLACEMENT_DATA_NEW,		FUNC_PTR_NULL, FUNC_PTR_NULL, 0x563AF0);
		FUNC_PTR(OBJECT_ITERATOR_NEXT,			FUNC_PTR_NULL, FUNC_PTR_NULL, 0x561360);
		FUNC_PTR(OBJECT_GET_ORIGIN,				FUNC_PTR_NULL, FUNC_PTR_NULL, 0x565320);
		FUNC_PTR(OBJECT_CAUSE_DAMAGE,			FUNC_PTR_NULL, FUNC_PTR_NULL, 0x64B5A0);
		FUNC_PTR(OBJECT_START_INTERPOLATION,	FUNC_PTR_NULL, FUNC_PTR_NULL, 0x564250);
		FUNC_PTR(OBJECT_GET_MARKER_BY_NAME,		FUNC_PTR_NULL, FUNC_PTR_NULL, 0x5650D0);
		FUNC_PTR(OBJECT_DESTROY,				FUNC_PTR_NULL, FUNC_PTR_NULL, 0x64AE40);
		//////////////////////////////////////////////////////////////////////////
		// saved_games/game_state
		FUNC_PTR(GAME_STATE_MALLOC,				FUNC_PTR_NULL, FUNC_PTR_NULL, 0x50B510);
		//////////////////////////////////////////////////////////////////////////
		// scenario/scenario
		FUNC_PTR(SCENARIO_SWITCH_STRUCTURE_BSP, 0x45FD30, 0x443A50, 0x516D40);
		FUNC_PTR(SCENARIO_LOAD,					0x4604D0, 0x443C50, 0x5174E0);
		FUNC_PTR(SCENARIO_UNLOAD,				0x45F820, 0x463840, 0x516830);
		FUNC_PTR(SCENARIO_TAGS_LOAD,			0x45F720, 0x4434A0, 0x516730);
		//////////////////////////////////////////////////////////////////////////
		// shell/shell_windows
		FUNC_PTR(SHELL_GET_COMMAND_LINE_ARGUMENT, 0x421ED0, 0x42DC70, 0x492E60);
		//////////////////////////////////////////////////////////////////////////
		// tag_files/files
		FUNC_PTR(FILE_REFERENCE_CREATE,			0x443D10, 0x4366C0, 0x4FF300);
		FUNC_PTR(FIND_FILES,					0x443D90, 0x436740, 0x4FF380);
		FUNC_PTR(FILE_REFERENCE_ADD_DIRECTORY,	0x444090, 0x436930, 0x4FF680);
		FUNC_PTR(FILE_REFERENCE_SET_NAME,		0x444110, 0x4369B0, 0x4FF700);
		FUNC_PTR(FILE_REFERENCE_GET_NAME,		0x4441A0, 0x436A20, 0x4FF790);
		//////////////////////////////////////////////////////////////////////////
		// tag_files/files_windows
		FUNC_PTR(FILE_ERROR,					0x445520, 0x44B4F0, 0x500B10);
		FUNC_PTR(FILE_CREATE,					0x4455B0, 0x44B580, 0x500BA0);
		FUNC_PTR(FILE_DELETE,					0x445680, 0x44B650, 0x500C70);
		FUNC_PTR(FILE_EXISTS,					0x445760, 0x44B730, 0x500D50);
		FUNC_PTR(FILE_OPEN,						0x445900, 0x44B7E0, 0x500EF0);
		FUNC_PTR(FILE_CLOSE,					0x445AA0, 0x44B980, 0x501090);
		FUNC_PTR(FILE_GET_POSITION,				0x445AF0, FUNC_PTR_NULL, 0x5010E0);
		FUNC_PTR(FILE_SET_POSITION,				0x445B30, 0x44B9D0, 0x501120);
		FUNC_PTR(FILE_GET_EOF,					0x445B80, 0x44BA20, 0x501170);
		FUNC_PTR(FILE_SET_EOF,					0x445BC0, FUNC_PTR_NULL, 0x5011B0);
		FUNC_PTR(FILE_READ,						0x445C40, 0x44BA60, 0x501230);
		FUNC_PTR(FILE_WRITE,					0x445CD0, 0x44BAF0, 0x5012C0);
		FUNC_PTR(FILE_READ_FROM_POSITION,		0x445D50, 0x44BB70, 0x501340);
		FUNC_PTR(FILE_WRITE_TO_POSITION,		0x445DB0, 0x44BBD0, 0x5013A0);
		FUNC_PTR(FILE_READ_ONLY_,				0x445030, 0x44B480, 0x500620);
		//////////////////////////////////////////////////////////////////////////
		// units/units
		FUNC_PTR(UNIT_KILL,							FUNC_PTR_NULL, FUNC_PTR_NULL, 0x585160);
		FUNC_PTR(UNIT_GET_CUSTOM_ANIMATION_TIME,	FUNC_PTR_NULL, FUNC_PTR_NULL, 0x58A220);
		FUNC_PTR(UNIT_START_USER_ANIMATION,			FUNC_PTR_NULL, FUNC_PTR_NULL, 0x58A2C0);
		FUNC_PTR(UNIT_DROP_CURRENT_WEAPON,			FUNC_PTR_NULL, FUNC_PTR_NULL, 0x58CA40);
		FUNC_PTR(UNIT_DAMAGE_AFTERMATH,				FUNC_PTR_NULL, FUNC_PTR_NULL, 0x594590);
		FUNC_PTR(UNIT_ENTER_SEAT,					FUNC_PTR_NULL, FUNC_PTR_NULL, 0x590830);
		FUNC_PTR(UNIT_EXIT_SEAT_END,				FUNC_PTR_NULL, FUNC_PTR_NULL, 0x590AC0);
		FUNC_PTR(UNIT_ANIMATION_SET_STATE,			FUNC_PTR_NULL, FUNC_PTR_NULL, 0x58B4C0);
		FUNC_PTR(UNIT_READY_DESIRED_WEAPON,			FUNC_PTR_NULL, FUNC_PTR_NULL, 0x58FE60);
		FUNC_PTR(UNIT_ANIMATION_START_ACTION,		FUNC_PTR_NULL, FUNC_PTR_NULL, 0x5864B0);
		FUNC_PTR(UNIT_INVENTORY_GET_WEAPON,			FUNC_PTR_NULL, FUNC_PTR_NULL, 0x58C1F0);
		FUNC_PTR(UNIT_THROW_GRENADE_RELEASE,		FUNC_PTR_NULL, FUNC_PTR_NULL, 0x589110);
		FUNC_PTR(UNIT_SET_ANIMATION,				FUNC_PTR_NULL, FUNC_PTR_NULL, 0x5898E0);
		FUNC_PTR(UNIT_OPEN,							FUNC_PTR_NULL, FUNC_PTR_NULL, 0x58C4D0);
		FUNC_PTR(UNIT_CLOSE,						FUNC_PTR_NULL, FUNC_PTR_NULL, 0x58C4F0);
		FUNC_PTR(UNIT_TRY_AND_EXIT_SEAT,			FUNC_PTR_NULL, FUNC_PTR_NULL, 0x592330);
		FUNC_PTR(UNIT_CAN_SEE_POINT,				FUNC_PTR_NULL, FUNC_PTR_NULL, 0x588290);
		FUNC_PTR(UNIT_CAUSE_PLAYER_MELEE_DAMAGE,	FUNC_PTR_NULL, FUNC_PTR_NULL, 0x58CF30);
		//////////////////////////////////////////////////////////////////////////
		// cryptography
		FUNC_PTR(GENERATE_MD5,					0x604860, 0x588B70, 0x6F78E0);
	};

#else
	#error Undefined engine layout include for: __EL_INCLUDE_GAME
#endif