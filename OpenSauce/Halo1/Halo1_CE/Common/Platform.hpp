/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

//////////////////////////////////////////////////////////////////////////
//#define PLATFORM_TYPE 0x00??

#if defined(PLATFORM_TYPE_CLIENT)
	#define PLATFORM_TYPE 0x0000
#elif defined(PLATFORM_TYPE_DEDI)
	#define PLATFORM_TYPE 0x0001
#else
	#error PLATFORM_TYPE not defined. Step 1: Bash head on keyboard. Step 2: Repeat.
#endif

// Is the target platform user based? (aka, haloce.exe)
#define PLATFORM_IS_USER ( PLATFORM_TYPE == 0x0000 )
// Is the target platform dedicated based? (aka, haloceded.exe)
#define PLATFORM_IS_DEDI ( PLATFORM_TYPE == 0x0001 )
//////////////////////////////////////////////////////////////////////////


#if !defined(PLATFORM_VERSION)
	//#define PLATFORM_VERSION 0x1080 // 1.08 is no longer actively supported, use at your own risk
	#define PLATFORM_VERSION 0x1090 // latest supported release of haloce
#endif


#if PLATFORM_VERSION == 0x1080
	#define PLATFORM_VERSION_VALUE 1.08
#elif PLATFORM_VERSION == 0x1090
	#define PLATFORM_VERSION_VALUE 1.09
#else
	#error FUCK YOU, this version isn't supported
#endif

// Target platform identifier
#define PLATFORM_ID ( (PLATFORM_TYPE<<16) | PLATFORM_VERSION )


// Calling convention of blam engine
#define PLATFORM_API __cdecl

#define PLATFORM_IS_EDITOR 0


// Name of the section where we'll place executable data
#define API_CODEDATA_SECTION_NAME	".yelo"
// Apply this to data which will later contain code which will also be executed
#define API_CODEDATA				__declspec(allocate(API_CODEDATA_SECTION_NAME))
// The fucking linker ignores 'write' with 'execute. Needs manual fix-up
#pragma section( API_CODEDATA_SECTION_NAME, read,write,execute )