/*
	Kornner Studios: Shared Code

	See license\Shared for specific license information
*/
/*!
 * Configures the code for various platforms
 */
#pragma once

#pragma region compiler
#if defined(_MSC_VER)
	#include <shell/Warnings.Msc.hpp>

#elif defined(__MWERKS__)
	#pragma message( "Warning: Unsupported Compiler" )
#elif defined(__GNUC__)
	#pragma message( "Warning: Unsupported Compiler" )
#elif defined(__INTEL_COMPILER)
	#pragma message( "Warning: Unsupported Compiler" )
#else
	#error "Unknown Compiler"
#endif
#pragma endregion


#pragma region architecture
#if defined(i386) || defined(_M_IX86) // INTEL
//
//
#elif /*defined(i386) ? ||*/ defined(_M_X64) // INTEL
//
//
#elif defined(__ppc__) || defined(_M_PPC) || defined(__POWERPC__)
	#if !defined(_XBOX) // MACHINTOSH
		#error "Unsupported CPU"
	#endif
#elif defined(__MIPS__) || defined(__mips__) // PS2
	#error "Unsupported CPU"
#else
	#error "Unknown CPU"
#endif
#pragma endregion


#pragma region platform
#if defined(__WIN32__) || defined(_WIN32)

#elif defined(__WIN64__) || defined(_WIN64)

#elif defined(_XBOX)

#elif defined(__unix__)
	#pragma message( "Warning: Unsupported Platform" )
#elif defined(macintosh) || defined(__APPLE__)
	#pragma message( "Warning: Unsupported Platform" )
#else
	#error "Unknown Platform"
#endif
#pragma endregion

// Windows: Define the path to the Program Files folder
#ifndef PATH_PROGRAM_FILES
	#define PATH_PROGRAM_FILES_X86 "C:\\Program Files"
	#define PATH_PROGRAM_FILES_X64 "C:\\Program Files (x86)"

	// I'm not sure how to determine the host's processor size via the compiler, 
	// so this is a manual setup. Set according to your computer's config
	#define PATH_PROGRAM_FILES PATH_PROGRAM_FILES_X64
#endif