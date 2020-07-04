/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#ifndef __GS_COMPONENT
	#error __GS_COMPONENT needs to be defined before including this file
#endif

// Reminder: BOOST_PP_ITERATION_LIMITS (min, max)
// Remember, there's a space between the name and the '('.
// min - the inclusive starting value of BOOST_PP_ITERATION()
// max - the inclusive ending value of BOOST_PP_ITERATION()

// Settings and FileIO must come first, many systems use the Report filing system
// ...like debugging for example uses the report filing system
#define BOOST_PP_ITERATION_LIMITS (1, 3)
#define BOOST_PP_FILENAME_1 "Common/GameSystemComponents.Startup.inl"
??=include BOOST_PP_ITERATE()

#define BOOST_PP_ITERATION_LIMITS (1, 2)
#define BOOST_PP_FILENAME_1 "Common/GameSystemComponents.GameState.inl"
??=include BOOST_PP_ITERATE()

#define BOOST_PP_ITERATION_LIMITS (1, 3)
#define BOOST_PP_FILENAME_1 "Common/GameSystemComponents.Tags.inl"
??=include BOOST_PP_ITERATE()

#define BOOST_PP_ITERATION_LIMITS (1, 1)
#define BOOST_PP_FILENAME_1 "Common/GameSystemComponents.Scripting.inl"
??=include BOOST_PP_ITERATE()

#define BOOST_PP_ITERATION_LIMITS (1, 3)
#define BOOST_PP_FILENAME_1 "Common/GameSystemComponents.GameStateYelo.inl"
??=include BOOST_PP_ITERATE()

#if PLATFORM_IS_USER
	#define BOOST_PP_ITERATION_LIMITS (1, 12)
	#define BOOST_PP_FILENAME_1 "Common/GameSystemComponents.Render.inl"
	??=include BOOST_PP_ITERATE()
#endif

#define BOOST_PP_ITERATION_LIMITS (1, 8)
#define BOOST_PP_FILENAME_1 "Common/GameSystemComponents.Game.inl"
??=include BOOST_PP_ITERATE()

#if PLATFORM_IS_USER
	#define BOOST_PP_ITERATION_LIMITS (1, 4)
	#define BOOST_PP_FILENAME_1 "Common/GameSystemComponents.Interface.inl"
	??=include BOOST_PP_ITERATE()
#endif

#define BOOST_PP_ITERATION_LIMITS (1, 5)
#define BOOST_PP_FILENAME_1 "Common/GameSystemComponents.Networking.inl"
??=include BOOST_PP_ITERATE()

#define BOOST_PP_ITERATION_LIMITS (1, 1)
#define BOOST_PP_FILENAME_1 "Common/GameSystemComponents.Misc.inl"
??=include BOOST_PP_ITERATE()

// Reset __GS_COMPONENT so 'callers' of this include don't have to
#undef __GS_COMPONENT

/*
#if		BOOST_PP_ITERATION() == 1
#	include "Common/GameSystemComponent.IterationRoundBegin.inl"
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#	elif	__GS_COMPONENT == __GS_COMPONENT_MAP_LIFECYCLE
#	elif	__GS_COMPONENT == __GS_COMPONENT_GAMESTATE_LIFECYCLE
#	elif	__GS_COMPONENT == __GS_COMPONENT_DX9_LIFECYCLE
#	elif	__GS_COMPONENT == __GS_COMPONENT_BSP_LIFECYCLE
#	endif
#	include "Common/GameSystemComponent.IterationRoundEnd.inl"
#elif	BOOST_PP_ITERATION() == 2
#	include "Common/GameSystemComponent.IterationRoundBegin.inl"
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#	elif	__GS_COMPONENT == __GS_COMPONENT_MAP_LIFECYCLE
#	elif	__GS_COMPONENT == __GS_COMPONENT_GAMESTATE_LIFECYCLE
#	elif	__GS_COMPONENT == __GS_COMPONENT_DX9_LIFECYCLE
#	elif	__GS_COMPONENT == __GS_COMPONENT_BSP_LIFECYCLE
#	endif
#	include "Common/GameSystemComponent.IterationRoundEnd.inl"
#endif
*/