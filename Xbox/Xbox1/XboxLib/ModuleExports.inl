/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/

// Currently this file is included at the very end of XboxKrnl.cpp

#if defined(__cplusplus)
extern "C" {
#endif
	XBOX_LIB_EXPORT DWORD				k_exports_signature =			1+ (1 + 1+1 + 1+1 + 1+1);		// 'Version id' based on the number of exports.
																										// Includes this export in the count.

	XBOX_LIB_EXPORT BOOL*				k_exports_emulation_mode =		&k_xbox_lib_emulation_mode;		// Xbox Emulators must set this to non-FALSE
	XBOX_LIB_EXPORT XBOX_EXPORT_TABLE*	k_exports_kernel_table =		&K_KERNEL_EXPORTS;
	XBOX_LIB_EXPORT void*				k_exports_build_table_proc =	&BuildKernelExportTable;
	XBOX_LIB_EXPORT const void**		k_exports_nv_frame_address =	&K_NV_FRAME_BUFFER_ADDRESS;
	XBOX_LIB_EXPORT const DWORD*		k_exports_nv_frame_size =		&K_NV_FRAME_BUFFER_SIZE;

	XBOX_LIB_EXPORT boost::uint32_t		k_exports_kernel_version =		5933;
	XBOX_LIB_EXPORT XBOX_KERNEL_VTABLE* k_exports_kernel_vtable =		&K_KERNEL_5933;
	//XBOX_LIB_EXPORT XBOX_KERNEL_DATA* k_exports_kernel_data =			&K_KERNELDATA_5933;
#if defined(__cplusplus)
};
#endif