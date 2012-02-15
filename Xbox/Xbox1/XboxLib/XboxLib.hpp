/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

#include <boost/preprocessor.hpp>
#include <boost/static_assert.hpp>
#include <boost/cstdint.hpp>

#if defined(__cplusplus)
extern "C" {
#endif

#include <windows.h> // for winnt.h shit
#include <cseries/MacrosCpp.hpp>
#include "XboxTypes.hpp"
#include "ModuleExports.hpp"

	#define XBOXAPI __stdcall

	#define IN
	#define OUT
	#define OPTIONAL
	#define UNALIGNED

	#define XBOX_PROC(ret, func, args) \
		typedef ret (XBOXAPI* _##func) args;

	// cdecl
	#define XBOX_STDPROC(ret, func, args) \
		typedef ret (__cdecl* _##func) args;

	// stdcall
	#define XBOX_WINPROC(ret, func, args) \
		typedef ret (__stdcall* _##func) args;



	// For usage after calling cdecl functions in assembly code.
	// In the case were our assembly code is just interfacing 
	// with an outside function.
	#define API_FUNC_NAKED_CDECL(arg_count) \
		__asm add	esp, (arg_count * 4) \
		__asm pop	ebp \
		__asm retn	(arg_count * 4) \
		}


	// FALSE means we're not being ran in an emulator, >0 means we are
	extern BOOL k_xbox_lib_emulation_mode;

	#pragma region CRT declares
	extern float _fltused; // HACKA HACKA HACKA...
	#pragma endregion


#define USE_CR0_HACK
	#pragma region cr0 hacks

#if defined(USE_CR0_HACK)
	extern boost::uint32_t CR0_REGISTER;

	#define CR0_BEGIN() __asm \
		{ \
			__asm	mov	eax, cr0 \
			__asm	mov	CR0_REGISTER, eax \
			__asm	and	eax, 0xFFFEFFFF \
			__asm	mov	cr0, eax \
		}

	#define CR0_END() __asm \
		{ \
			__asm	mov	eax, CR0_REGISTER \
			__asm	mov	cr0, eax \
		}
#else

	#define CR0_BEGIN()
	#define CR0_END()

#endif
#pragma endregion

#if defined(__cplusplus)
};
#endif