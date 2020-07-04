/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

#include "Include.hpp"

extern "C" { // Mark as C code so the compiler doesn't mangle the exported function name with C++ markup

	// [lpFileName]	- path to the dll who's addresses we will rebase
	// [code_size]	- size needed
	// returns: S_OK if everything went ok, else E_FAIL, and E_ABORT if internal operations failed
	// ASSUMES:
	//	The file exists. It handles non-existent files, however, will just return zero.
	UTILDLL_API HRESULT Util_CalculateModuleCodeSize(IN LPCWSTR lpFileName, OUT PUINT32 code_size);

	// [lpFileName]	- path to the dll who's addresses we will rebase
	// [reloc_dll]	- buffer to receive the rebased dll file (should be at least as large as the dll in bytes)
	// [reloc_dll_size] - size of the [reloc_dll] buffer
	// [base_address] - 
	//	[in] - address were the code will be located in the xbox's RAM.
	//	[out] - entry point (*offset* in [reloc_dll]) which the xbox should jmp\call to to start the module
	// [reloc_alloc_count] - how many allocation table pointers we should allocate. Set to 0 to use the default size.
	// returns: S_OK if everything went ok, else E_FAIL, and E_ABORT if internal operations failed
	//
	// REMARKS:
	//	[base_address] Is where the first byte of the first section that will start in the xbox's memory, and the
	//	rest of the file will be loaded into each byte after it (duh).
	//	So to get an object's offset in [reloc_dll] just subtract [base_address] from its absolute address.
	// ASSUMES:
	//	.text is the first section
	//	.text, .rdata, and .data sections (however many) all come before discardable sections (eg, .edata, .reloc, etc)
	//	.reloc is the last section of needed data (that has to be in memory) to run the code
	UTILDLL_API HRESULT Util_RebaseModule(IN LPCWSTR lpFileName, OUT PBYTE reloc_dll, IN UINT32 reloc_dll_size, 
		IN OUT PUINT32 base_address, IN UINT32 reloc_alloc_count);
};