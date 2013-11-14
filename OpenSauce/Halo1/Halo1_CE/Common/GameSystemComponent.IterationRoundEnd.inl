/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

// Define all undefined component procs as NULL
#if	__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#	ifndef		__GS_COMPONENT_INITIALIZE
#		define	__GS_COMPONENT_INITIALIZE nullptr
#	else // a component function was defined, this round is valid
#		undef	__GS_COMPONENT_INCLUDE_IGNORE_CURRENT_ROUND
#	endif
#
#	ifndef		__GS_COMPONENT_DISPOSE
#		define	__GS_COMPONENT_DISPOSE nullptr
#	else // a component function was defined, this round is valid
#		undef	__GS_COMPONENT_INCLUDE_IGNORE_CURRENT_ROUND
#	endif
#
#	ifndef		__GS_COMPONENT_UPDATE
#		define	__GS_COMPONENT_UPDATE nullptr
#	else // a component function was defined, this round is valid
#		undef	__GS_COMPONENT_INCLUDE_IGNORE_CURRENT_ROUND
#	endif
#
#elif __GS_COMPONENT == __GS_COMPONENT_MAP_LIFECYCLE
#	ifndef		__GS_COMPONENT_INITIALIZE_FOR_NEW_MAP
#		define	__GS_COMPONENT_INITIALIZE_FOR_NEW_MAP nullptr
#	else // a component function was defined, this round is valid
#		undef	__GS_COMPONENT_INCLUDE_IGNORE_CURRENT_ROUND
#	endif
#
#	ifndef		__GS_COMPONENT_DISPOSE_FROM_OLD_MAP
#		define	__GS_COMPONENT_DISPOSE_FROM_OLD_MAP nullptr
#	else // a component function was defined, this round is valid
#		undef	__GS_COMPONENT_INCLUDE_IGNORE_CURRENT_ROUND
#	endif
#
#elif __GS_COMPONENT == __GS_COMPONENT_GAMESTATE_LIFECYCLE
#	ifndef		__GS_COMPONENT_INITIALIZE_FOR_NEW_GAME_STATE
#		define	__GS_COMPONENT_INITIALIZE_FOR_NEW_GAME_STATE nullptr
#	else // a component function was defined, this round is valid
#		undef	__GS_COMPONENT_INCLUDE_IGNORE_CURRENT_ROUND
#	endif
#
#	ifndef		__GS_COMPONENT_INITIALIZE_FOR_NEW_YELO_GAME_STATE
#		define	__GS_COMPONENT_INITIALIZE_FOR_NEW_YELO_GAME_STATE nullptr
#	else // a component function was defined, this round is valid
#		undef	__GS_COMPONENT_INCLUDE_IGNORE_CURRENT_ROUND
#	endif
#
#	ifndef		__GS_COMPONENT_HANDLE_GAME_STATE_LIFECYCLE
#		define	__GS_COMPONENT_HANDLE_GAME_STATE_LIFECYCLE nullptr
#	else // a component function was defined, this round is valid
#		undef	__GS_COMPONENT_INCLUDE_IGNORE_CURRENT_ROUND
#	endif
#
#elif __GS_COMPONENT == __GS_COMPONENT_DX9_LIFECYCLE
#	ifndef		__GS_COMPONENT_DX9_INITIALIZE
#		define	__GS_COMPONENT_DX9_INITIALIZE nullptr
#	else // a component function was defined, this round is valid
#		undef	__GS_COMPONENT_INCLUDE_IGNORE_CURRENT_ROUND
#	endif
#
#	ifndef		__GS_COMPONENT_DX9_ON_LOST_DEVICE
#		define	__GS_COMPONENT_DX9_ON_LOST_DEVICE nullptr
#	else // a component function was defined, this round is valid
#		undef	__GS_COMPONENT_INCLUDE_IGNORE_CURRENT_ROUND
#	endif
#
#	ifndef		__GS_COMPONENT_DX9_ON_RESET_DEVICE
#		define	__GS_COMPONENT_DX9_ON_RESET_DEVICE nullptr
#	else // a component function was defined, this round is valid
#		undef	__GS_COMPONENT_INCLUDE_IGNORE_CURRENT_ROUND
#	endif
#
#	ifndef		__GS_COMPONENT_DX9_RENDER
#		define	__GS_COMPONENT_DX9_RENDER nullptr
#	else // a component function was defined, this round is valid
#		undef	__GS_COMPONENT_INCLUDE_IGNORE_CURRENT_ROUND
#	endif
#
#	ifndef		__GS_COMPONENT_DX9_RELEASE
#		define	__GS_COMPONENT_DX9_RELEASE nullptr
#	else // a component function was defined, this round is valid
#		undef	__GS_COMPONENT_INCLUDE_IGNORE_CURRENT_ROUND
#	endif
#
#endif


// Define the specific component entry, unless the component definitions say otherwise
#ifndef __GS_COMPONENT_INCLUDE_IGNORE_CURRENT_ROUND
#	if	__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
	{
		__GS_COMPONENT_INITIALIZE,
		__GS_COMPONENT_DISPOSE,
		__GS_COMPONENT_UPDATE,
	},
#	elif __GS_COMPONENT == __GS_COMPONENT_MAP_LIFECYCLE
	{
		__GS_COMPONENT_INITIALIZE_FOR_NEW_MAP,
		__GS_COMPONENT_DISPOSE_FROM_OLD_MAP,
	},
#	elif __GS_COMPONENT == __GS_COMPONENT_GAMESTATE_LIFECYCLE
	{
		__GS_COMPONENT_INITIALIZE_FOR_NEW_GAME_STATE,
		__GS_COMPONENT_INITIALIZE_FOR_NEW_YELO_GAME_STATE,
	},
#	elif __GS_COMPONENT == __GS_COMPONENT_DX9_LIFECYCLE
	{
		__GS_COMPONENT_DX9_INITIALIZE,
		__GS_COMPONENT_DX9_ON_LOST_DEVICE,
		__GS_COMPONENT_DX9_ON_RESET_DEVICE,
		__GS_COMPONENT_DX9_RENDER,
		__GS_COMPONENT_DX9_RELEASE,
	},
#	endif
#else // reset the ignore current round state
#	undef __GS_COMPONENT_INCLUDE_IGNORE_CURRENT_ROUND
#endif