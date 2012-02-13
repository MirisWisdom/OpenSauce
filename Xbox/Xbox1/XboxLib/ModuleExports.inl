/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/

// Currently this file is included at the very end of XboxKrnl.cpp

#if defined(__cplusplus)
extern "C" {
#endif
	XBOX_LIB_EXPORT DWORD				k_exports_signature =			1+ (1 + 1 + 1+1 + 1+1 + 1+1) +	// 'Version id' based on the number of exports.
																		(1+1+1) +						// Includes this export in the count.
																		(1+1+1)
																		;

	XBOX_LIB_EXPORT const DWORD*		k_exports_title_id =			&k_module_title_id;
	XBOX_LIB_EXPORT BOOL*				k_exports_emulation_mode =		&k_xbox_lib_emulation_mode;		// Xbox Emulators must set this to non-FALSE

	XBOX_LIB_EXPORT XBOX_EXPORT_TABLE*	k_exports_kernel_table =		&K_KERNEL_EXPORTS;
	XBOX_LIB_EXPORT void*				k_exports_build_table_proc =	&BuildKernelExportTable;
	XBOX_LIB_EXPORT const void**		k_exports_nv_frame_address =	&K_NV_FRAME_BUFFER_ADDRESS;
	XBOX_LIB_EXPORT const DWORD*		k_exports_nv_frame_size =		&K_NV_FRAME_BUFFER_SIZE;

	XBOX_LIB_EXPORT boost::uint32_t		k_exports_kernel_version =		5933;
	XBOX_LIB_EXPORT XBOX_KERNEL_VTABLE* k_exports_kernel_vtable =		&K_KERNEL_5933;
	//XBOX_LIB_EXPORT XBOX_KERNEL_DATA* k_exports_kernel_data =			&K_KERNELDATA_5933;

	// Note: since we have pointers that reference data within our export section,
	// this will probably cause relocation entries for this section. Need to validate
	// that our module rebaser utility doesn't choke on this

	XBOX_LIB_EXPORT XBOX_API_VTABLE*	k_exports_api =					&XboxApi;
	XBOX_LIB_EXPORT_HIDDEN const char	k_exports_api_names_[][32] = {
		#define VTABLE_ENTRY(function) #function ,
		#define VTABLE_ENTRYA(function) #function "A" ,
		#include "XboxVTable.Api.inl"

		""
	};
	// If we don't reference the names table, it won't get compiled in...
	XBOX_LIB_EXPORT const char*			k_exports_api_names =			&k_exports_api_names_[0][0];

	XBOX_LIB_EXPORT _XBOX_STD_VTABLE*	k_exports_stdc =				&XboxStd;
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