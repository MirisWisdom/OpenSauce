/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/

// Currently this file is included at the very end of XboxKrnl.cpp

#if defined(__cplusplus)
extern "C" {
#endif
	#include "ModuleExports.Names.inl"

	XBOX_LIB_EXPORT_HIDDEN const char	k_exports_api_names_[][32] = {
		#define VTABLE_ENTRY(function) #function ,
		#define VTABLE_ENTRYA(function) #function "A" ,
		#include "XboxVTable.Api.inl"

		""
	};
	// If we don't reference the names table, it won't get compiled in...
	XBOX_LIB_EXPORT const char*			k_exports_api_names =			&k_exports_api_names_[0][0];

	XBOX_LIB_EXPORT_HIDDEN const char	k_exports_stdc_names_[][32] = {
		#define VTABLE_ENTRY(function) #function ,
		#define VTABLE_ENTRY_(function) "_" #function ,
		#include "XboxVTable.Std.inl"

		""
	};
	// If we don't reference the names table, it won't get compiled in...
	XBOX_LIB_EXPORT const char*			k_exports_stdc_names =			&k_exports_stdc_names_[0][0];
#if defined(__cplusplus)
};
#endif