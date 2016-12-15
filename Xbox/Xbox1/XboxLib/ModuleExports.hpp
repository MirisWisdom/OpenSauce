/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

// Name of the PE data section where the module exports are stored
#define K_XBOX_LIB_EXPORT_SECTION "modxport"
// Define the properties of our module exports data section
// NOTE: If emulating, the emulator must modify this section in memory 
// to be 'writable' as by default, it is read-only. Once the emulator 
// is finished processing its changes to the module, set back to read-only.
#pragma section(K_XBOX_LIB_EXPORT_SECTION, read,discard)

// Declaration prefix for storing code elements in our module export section
// but without actually exporting them
#define XBOX_LIB_EXPORT_HIDDEN							\
	__declspec( allocate(K_XBOX_LIB_EXPORT_SECTION) )	\
	static

// Declaration prefix for storing code elements in our module export section
#define XBOX_LIB_EXPORT									\
	__declspec( allocate(K_XBOX_LIB_EXPORT_SECTION) )	\
	__declspec( dllexport )

// Target game's TitleId (match with the XBE certificate)
extern DWORD				k_module_title_id;		// !!! DEFINE IN YOUR DXT !!!
// Target game's Version (match with the XBE certificate)
extern DWORD				k_module_title_version;	// !!! DEFINE IN YOUR DXT !!!