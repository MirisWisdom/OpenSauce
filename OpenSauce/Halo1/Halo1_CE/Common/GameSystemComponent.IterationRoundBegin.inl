/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#ifndef __GS_COMPONENT_INCLUDE_ITERATION_ROUND
#	define __GS_COMPONENT_INCLUDE_ITERATION_ROUND
#else
#	error Mismatching IterationRoundBegin.inl include
#endif

// Undefine all the macros which the component definitions could specify this round
#if	__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#	undef __GS_COMPONENT_INITIALIZE
#	undef __GS_COMPONENT_DISPOSE
#	undef __GS_COMPONENT_UPDATE
#elif __GS_COMPONENT == __GS_COMPONENT_MAP_LIFECYCLE
#	undef __GS_COMPONENT_INITIALIZE_FOR_NEW_MAP
#	undef __GS_COMPONENT_DISPOSE_FROM_OLD_MAP
#elif __GS_COMPONENT == __GS_COMPONENT_GAMESTATE_LIFECYCLE
#	undef __GS_COMPONENT_INITIALIZE_FOR_NEW_GAME_STATE
#	undef __GS_COMPONENT_INITIALIZE_FOR_NEW_YELO_GAME_STATE
#	undef __GS_COMPONENT_HANDLE_GAME_STATE_LIFECYCLE
#elif __GS_COMPONENT == __GS_COMPONENT_DX9_LIFECYCLE
#	undef __GS_COMPONENT_DX9_INITIALIZE
#	undef __GS_COMPONENT_DX9_ON_LOST_DEVICE
#	undef __GS_COMPONENT_DX9_ON_RESET_DEVICE
#	undef __GS_COMPONENT_DX9_RENDER
#	undef __GS_COMPONENT_DX9_RELEASE
#elif __GS_COMPONENT == __GS_COMPONENT_BSP_LIFECYCLE
#	undef __GS_COMPONENT_INITIALIZE_FOR_NEW_BSP
#	undef __GS_COMPONENT_DISPOSE_FROM_OLD_BSP
#endif

// The round will automatically be ignored, unless one of the component functions above are defined
// for the given LIFECYCLE definition
#define __GS_COMPONENT_INCLUDE_IGNORE_CURRENT_ROUND