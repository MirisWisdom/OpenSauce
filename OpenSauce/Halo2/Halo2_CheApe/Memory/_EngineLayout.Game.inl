/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\Halo2_CheApe for specific license information
*/


//////////////////////////////////////////////////////////////////////////
// EngineFunctions.cpp
#if __EL_INCLUDE_FILE_ID == __EL_GAME_ENGINE_FUNCTIONS
	namespace EngineFunctions
	{
		proc_error error = CAST_PTR(proc_error,								PLATFORM_VALUE(0x48B980, 0x535CE0, 0x4BA140));
		proc_error_category error_category = CAST_PTR(proc_error_category,	PLATFORM_VALUE(0x48B9A0, 0x535D00, 0x4BA160));

		proc_error_unicode error_unicode = CAST_PTR(proc_error_unicode,		PLATFORM_VALUE(0x48BA50, 0x535DB0, 0x4BA210));
		proc_error_unicode_category error_unicode_category = CAST_PTR(proc_error_unicode_category,
																			PLATFORM_VALUE(0x48BA70, 0x535DD0, 0x4BA230));

		FUNC_PTR(DEBUG_MALLOC,	0x49A760, 0x52B540, 0x4D4830);
		FUNC_PTR(DEBUG_FREE,	0x49A260, 0x52B040, 0x4D4330);
		FUNC_PTR(DEBUG_REALLOC,	0x49A9C0, 0x52B7A0, 0x4D4A90);

		FUNC_PTR(FILE_REFERENCE_CREATE,			0x48BAC0,		0x52A010, 0x4B7BD0);
		FUNC_PTR(FILE_REFERENCE_ADD_DIRECTORY,	0x48BC40,		0x52A240, 0x4B7E00);
		FUNC_PTR(FILE_REFERENCE_SET_NAME,		0x48BC60,		0x52A280, 0x4B7E20);
		FUNC_PTR(FIND_FILES,					FUNC_PTR_NULL,	0x52A060, 0x4B7C20);
	};

#else
	#error Undefined engine layout include for: __EL_INCLUDE_GAME
#endif