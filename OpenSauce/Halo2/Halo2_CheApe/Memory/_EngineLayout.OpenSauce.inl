/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\Halo2_CheApe for specific license information
*/


//////////////////////////////////////////////////////////////////////////
// .cpp
#if __EL_INCLUDE_FILE_ID == __EL_COMMON_YELO_SETTINGS

//////////////////////////////////////////////////////////////////////////
// CheApeCore.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_MEMORY_CHE_APE_INTERFACE
	namespace CheApe
	{
		static const uintptr_t TagGroupDefinitionsRefs[] = { // &tag_groups[x]
			PLATFORM_VALUE(0x482E06, 0x52D416, NULL),	PLATFORM_VALUE(0x482E29, 0x52D439, NULL),	PLATFORM_VALUE(0x482E52, 0x52D462, NULL),
			PLATFORM_VALUE(0x483B62, 0x52E1C2, NULL),	PLATFORM_VALUE(0x483C07, 0x52E267, NULL),	PLATFORM_VALUE(0x484029, 0x52E689, NULL),
			PLATFORM_VALUE(0x484051, 0x52E6B1, NULL),	PLATFORM_VALUE(0x486622, 0x530F52, NULL),	PLATFORM_VALUE(0x486684, 0x530FB4, NULL),
			PLATFORM_VALUE(0x4868C7, 0x5311F7, NULL),	PLATFORM_VALUE(0x488BD2, 0x533772, NULL),	PLATFORM_VALUE(0x488C4C, 0x5337EC, NULL),
			PLATFORM_VALUE(0x489B61, 0x5346C1, NULL),
		};

		static const uintptr_t TagGroupDefinitionsCountRefs8bit[] = { // CMP si, 77h
			PLATFORM_VALUE(0x482E12, 0x52D422, NULL),
		};

		enum {
			kTagGroupDefinitionsAddress =	PLATFORM_VALUE(0x901B90, 0x98BAB0, NULL),
			kTagGroupDefinitionsCount =		0x78,

			kExeBaseAddress =				0x400000,
			kExeSize =						PLATFORM_VALUE(0x4FC200, 0x579800, 0x57E200),
		};
	};

#else
	#error Undefined engine layout include for: __EL_INCLUDE_OPEN_SAUCE
#endif