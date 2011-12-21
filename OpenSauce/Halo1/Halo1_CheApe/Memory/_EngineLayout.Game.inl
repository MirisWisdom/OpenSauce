/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/


//////////////////////////////////////////////////////////////////////////
// .cpp
#if __EL_INCLUDE_FILE_ID == __EL_GAME_ENGINE_FUNCTIONS
	namespace EngineFunctions
	{
		proc_error error = CAST_PTR(proc_error, PLATFORM_VALUE(0x4206B0, 0x42CB90, 0x417500));

		FUNC_PTR(DISPLAY_ASSERT,	0x42C840, 0x435F10, 0x4F7000);
		FUNC_PTR(DEBUG_MALLOC,		0x446970, 0x43E6B0, 0x501F50);
		FUNC_PTR(DEBUG_FREE,		0x446A80, 0x43E7C0, 0x502060);
		FUNC_PTR(DEBUG_REALLOC,		0x446B50, 0x43E890, 0x502130);

		FUNC_PTR(FILE_REFERENCE_CREATE,			FUNC_PTR_NULL, 0x4366C0, FUNC_PTR_NULL);
		FUNC_PTR(FILE_REFERENCE_ADD_DIRECTORY,	FUNC_PTR_NULL, 0x436930, FUNC_PTR_NULL);
		FUNC_PTR(FILE_REFERENCE_SET_NAME,		FUNC_PTR_NULL, 0x4369B0, FUNC_PTR_NULL);
		FUNC_PTR(FIND_FILES,					FUNC_PTR_NULL, 0x436740, FUNC_PTR_NULL);
	};

#else
	#error Undefined engine layout include for: __EL_INCLUDE_GAME
#endif