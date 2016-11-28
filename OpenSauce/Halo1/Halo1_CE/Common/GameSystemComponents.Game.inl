/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/GameSystemComponent.IterationRoundBegin.inl"

#if		BOOST_PP_ITERATION() == 1 && PLATFORM_IS_USER
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	Camera::Initialize
#		define __GS_COMPONENT_DISPOSE		Camera::Dispose
#	endif
#elif	BOOST_PP_ITERATION() == 2
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE // No active code currently
#		define __GS_COMPONENT_INITIALIZE	Console::Initialize
#		define __GS_COMPONENT_DISPOSE		Console::Dispose
#	endif
#elif	BOOST_PP_ITERATION() == 3
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	GameEngine::Initialize
#		define __GS_COMPONENT_DISPOSE		GameEngine::Dispose
#	endif
#elif	BOOST_PP_ITERATION() == 4
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	Networking::Initialize
#		define __GS_COMPONENT_DISPOSE		Networking::Dispose
#	endif
#elif	BOOST_PP_ITERATION() == 5
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	Players::Initialize
#		define __GS_COMPONENT_DISPOSE		Players::Dispose
											// No active code in Update game hook currently
#	endif
#elif	BOOST_PP_ITERATION() == 6 // No active code currently
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	Effects::Initialize
#		define __GS_COMPONENT_DISPOSE		Effects::Dispose
#	endif
#elif	BOOST_PP_ITERATION() == 7 // No active code currently
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE					AI::Initialize
#		define __GS_COMPONENT_DISPOSE						AI::Dispose
#	elif	__GS_COMPONENT == __GS_COMPONENT_MAP_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE_FOR_NEW_MAP		AI::InitializeForNewMap
#		define __GS_COMPONENT_DISPOSE_FROM_OLD_MAP			AI::DisposeFromOldMap
#	elif	__GS_COMPONENT == __GS_COMPONENT_GAMESTATE_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE_FOR_NEW_GAME_STATE	AI::InitializeForNewGameState
#		define __GS_COMPONENT_HANDLE_GAME_STATE_LIFECYCLE	AI::HandleGameStateLifeCycle
#	endif
#elif	BOOST_PP_ITERATION() == 8
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE							Objects::Initialize
#		define __GS_COMPONENT_DISPOSE								Objects::Dispose
																	// No active code in Update game hook currently
#	elif	__GS_COMPONENT == __GS_COMPONENT_MAP_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE_FOR_NEW_MAP				Objects::InitializeForNewMap
#		define __GS_COMPONENT_DISPOSE_FROM_OLD_MAP					Objects::DisposeFromOldMap
#	elif	__GS_COMPONENT == __GS_COMPONENT_GAMESTATE_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE_FOR_NEW_YELO_GAME_STATE	Objects::InitializeForYeloGameState
#	endif
#endif

#include "Common/GameSystemComponent.IterationRoundEnd.inl"