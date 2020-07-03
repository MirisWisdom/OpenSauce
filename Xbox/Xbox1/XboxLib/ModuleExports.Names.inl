#ifndef EXPORT_ENTRY
	#define EXPORT_ENTRY(type, name, value) XBOX_LIB_EXPORT type name = value ;
	#define EXPORT_ENTRYC(type, name, value) XBOX_LIB_EXPORT const type name = value ;
#endif

EXPORT_ENTRY(DWORD,					k_exports_signature,		1+ (1+1 + 1 + 1+1 + 1+1 + 1+1) +	// 'Version id' based on the number of exports.
																(1+1+1) +							// Includes this export in the count.
																(1+1+1)
																)

EXPORT_ENTRYC(DWORD*,				k_exports_title_id,			&k_module_title_id)
EXPORT_ENTRYC(DWORD*,				k_exports_title_version,	&k_module_title_version)

EXPORT_ENTRY(BOOL*,					k_exports_emulation_mode,	&k_xbox_lib_emulation_mode)			// Xbox Emulators must set this to non-FALSE

EXPORT_ENTRY(XBOX_EXPORT_TABLE*,	k_exports_kernel_table,		&K_KERNEL_EXPORTS)
EXPORT_ENTRY(void*,					k_exports_build_table_proc,	&BuildKernelExportTable)
EXPORT_ENTRYC(void**,				k_exports_nv_frame_address,	&K_NV_FRAME_BUFFER_ADDRESS)
EXPORT_ENTRYC(DWORD*,				k_exports_nv_frame_size,	&K_NV_FRAME_BUFFER_SIZE)

EXPORT_ENTRY(boost::uint32_t,		k_exports_kernel_version,	5933)
EXPORT_ENTRY(XBOX_KERNEL_VTABLE*,	k_exports_kernel_vtable,	&K_KERNEL_5933)
//EXPORT_ENTRY(XBOX_KERNEL_DATA*,	k_exports_kernel_data,		&K_KERNELDATA_5933)

EXPORT_ENTRY(XBOX_API_VTABLE*,		k_exports_api,				&XboxApi)
EXPORT_ENTRY(XBOX_STD_VTABLE*,		k_exports_stdc,				&XboxStd)

#undef EXPORT_ENTRY
#undef EXPORT_ENTRYC