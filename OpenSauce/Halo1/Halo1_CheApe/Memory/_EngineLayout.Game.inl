/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/


//////////////////////////////////////////////////////////////////////////
// .cpp
#if __EL_INCLUDE_FILE_ID == __EL_GAME_ENGINE_FUNCTIONS
	namespace Engine
	{
		FUNC_PTR(GET_CMD_LINE_PARAMETER,			0x421ED0, 0x42DC70, 0x492E60);

		proc_error error = CAST_PTR(proc_error, PLATFORM_VALUE(0x4206B0, 0x42CB90, 0x417500));

		FUNC_PTR(DISPLAY_ASSERT,	0x42C840, 0x435F10, 0x4F7000);
		FUNC_PTR(DEBUG_MALLOC,		0x446970, 0x43E6B0, 0x501F50);
		FUNC_PTR(DEBUG_FREE,		0x446A80, 0x43E7C0, 0x502060);
		FUNC_PTR(DEBUG_REALLOC,		0x446B50, 0x43E890, 0x502130);
	};

	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// math/periodic_functions
		FUNC_PTR(PERIODIC_FUNCTION_EVALUATE,	0x4DC780, 0x490250, 0x61A620);
		FUNC_PTR(TRANSITION_FUNCTION_EVALUATE,	0x4DC8C0, 0x490390, 0x61A760);
		//////////////////////////////////////////////////////////////////////////
		// memory/byte_swapping
		FUNC_PTR(BYTE_SWAP_DATA_EXPLICIT,		0x4968F0, 0x490060, 0x5ABCF0);
		//////////////////////////////////////////////////////////////////////////
		// memory/data
		FUNC_PTR(DATUM_TRY_AND_GET,				0x45CF40, 0x4759D0, 0x51CFC0);
		FUNC_PTR(DATUM_GET,						0x45CFF0, 0x475A80, 0x51D070);
		FUNC_PTR(DATA_VERIFY,					0x45D0C0, 0x475B50, 0x51D140);
		FUNC_PTR(DATA_NEW,						0x45D1A0, 0x475C30, 0x51D220);
		FUNC_PTR(DATA_DISPOSE,					0x45D1F0, 0x475C80, 0x51D270);
		FUNC_PTR(DATUM_NEW_AT_INDEX,			0x45D240, 0x475CD0, 0x51D2C0);
		FUNC_PTR(DATUM_NEW,						0x45D2F0, 0x475D80, 0x51D420);
		FUNC_PTR(DATUM_DELETE,					0x45D3A0, 0x475E40, 0x51D4E0);
		FUNC_PTR(DATA_DELETE_ALL,				0x45D4B0, 0x475E90, 0x51D530);
		FUNC_PTR(DATA_ITERATOR_NEXT,			0x45D5A0, 0x475F80, 0x51D620);
		FUNC_PTR(DATA_NEXT_INDEX,				0x45D670, 0x476050, 0x51D6F0);
		FUNC_PTR(DATA_MAKE_VALID,				0x45D890, 0x476170, 0x51D910);
		//////////////////////////////////////////////////////////////////////////
		// tag_files/files
		FUNC_PTR(FILE_REFERENCE_CREATE,			0x443D10, 0x4366C0, 0x4FF300);
		FUNC_PTR(FIND_FILES,					0x443D90, 0x436740, 0x4FF380);
		FUNC_PTR(FILE_REFERENCE_ADD_DIRECTORY,	0x444090, 0x436930, 0x4FF680);
		FUNC_PTR(FILE_REFERENCE_SET_NAME,		0x444110, 0x4369B0, 0x4FF700);
		FUNC_PTR(FILE_REFERENCE_GET_NAME,		0x4441A0, 0x436A20, 0x4FF790);
		FUNC_PTR(FILE_CREATE,					0x4455B0, 0x44B580, 0x500BA0);
		FUNC_PTR(FILE_DELETE,					0x445680, 0x44B650, 0x500C70);
		FUNC_PTR(FILE_EXISTS,					0x445760, 0x44B730, 0x500D50);
	};

#else
	#error Undefined engine layout include for: __EL_INCLUDE_GAME
#endif