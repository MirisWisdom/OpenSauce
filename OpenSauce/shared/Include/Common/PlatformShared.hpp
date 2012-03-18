/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

//////////////////////////////////////////////////////////////////////////
// For all codebases
#define PLATFORM_TARGET_PC		0x00000001
// Xbox 1
#define PLATFORM_TARGET_XBOX	0x00000002
// Xbox 360
#define PLATFORM_TARGET_XENON	0x00000003

// Define the following in specific game codebases
//#define PLATFORM_TARGET		PLATFORM_TARGET_


//////////////////////////////////////////////////////////////////////////
// For game codebases

// Define the following in specific game codebases
//#define PLATFORM_IS_EDITOR FALSE


//////////////////////////////////////////////////////////////////////////
// For editor codebases
#define PLATFORM_GUERILLA	0x00000001
#define PLATFORM_TOOL		0x00000002
#define PLATFORM_SAPIEN		0x00000003

#define CHEAPE_PLATFORM_HALO1 1
#define CHEAPE_PLATFORM_HALO2 2

// Define the following in specific editor codebases
//#define PLATFORM_IS_EDITOR TRUE
//#define CHEAPE_PLATFORM CHEAPE_PLATFORM_