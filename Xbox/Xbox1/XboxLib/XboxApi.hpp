/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once
#include "XboxLib.hpp"
#include "XboxKrnl.hpp"


#if defined(__cplusplus)
extern "C" {
#endif

// Procedure signature for the exit function of a module
typedef void						(XBOXAPI* xbox_lib_module_exit_proc)();
// Procedure signature for the entry function of a module
// Returns the address of the exit function or NULL if there isn't one
typedef xbox_lib_module_exit_proc	(XBOXAPI* xbox_lib_module_main_proc)();


extern void							ModuleExit();	// !!! DEFINE IN YOUR DXT !!!
extern xbox_lib_module_exit_proc	ModuleMain();	// !!! DEFINE IN YOUR DXT !!!

extern struct _XBOX_API_VTABLE XboxApi;				// !!! DEFINE IN YOUR DXT !!!
extern struct _XBOX_STD_VTABLE XboxStd;				// !!! DEFINE IN YOUR DXT !!!

#include "Xapi/dir.hpp"
#include "Xapi/contsig.hpp"
#include "Xapi/datetime.hpp"
#include "Xapi/error.hpp"
#include "Xapi/filefind.hpp"
#include "Xapi/filehops.hpp"
#include "Xapi/filemisc.hpp"
#include "Xapi/fileopcr.hpp"
#include "Xapi/findcont.hpp"
#include "Xapi/handle.hpp"
#include "Xapi/kthunks.hpp"
#include "Xapi/launch.hpp"
#include "Xapi/lcompat.hpp"
#include "Xapi/lcompata.hpp"
#include "Xapi/mountmu.hpp"
#include "Xapi/pathmisc.hpp"
#include "Xapi/physical.hpp"
#include "Xapi/process.hpp"
#include "Xapi/synch.hpp"
#include "Xapi/thread.hpp"
#include "Xapi/virtual.hpp"
#include "Xapi/widechar.hpp"
#include "Xapi/xapiheap.hpp"
#include "Xapi/xemodule.hpp"
#include "Xapi/xmem.hpp"
#include "Xapi/xcalcsig.hpp"

#include "Xapi/Xnet.hpp"


typedef struct _XBOX_API_VTABLE {
	#include "XboxVTable.Api.inl"
}XBOX_API_VTABLE;
extern const XBOX_API_VTABLE* gXBOX_API_VTABLE;

// Determines if any of this [gXBOX_API_VTABLE]'s function pointers are NULL
bool XboxApiContainsNullFunctions();



#include "Std/Files.hpp"
#include "Std/Sockets.hpp"
#include "Std/Strings.hpp"

#pragma region Misc
// ******************************************************************
// * Math
// ******************************************************************
	XBOX_STDPROC(void, srand, (
				 unsigned int seed
				 ));
	#define Std_srand XboxStd.srand

	XBOX_STDPROC(int, rand, (
				 void
				 ));
	#define Std_rand XboxStd.rand

// ******************************************************************
// * Memory
// ******************************************************************
	XBOX_STDPROC(void*, malloc, (
		__in size_t _Size
	));
	#define Std_malloc XboxStd.malloc
#pragma endregion

//??2@YAPAXI@Z "new"

typedef struct _XBOX_STD_VTABLE {
	#include "XboxVTable.Std.inl"
}XBOX_STD_VTABLE;
extern const XBOX_STD_VTABLE* gXBOX_STD_VTABLE;

// Determines if any of this [gXBOX_STD_VTABLE]'s function pointers are NULL
bool XboxStdContainsNullFunctions();

#if defined(__cplusplus)
};
#endif