/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#if		BOOST_PP_ITERATION() == 1 && PLATFORM_IS_USER
#	include "Common/GameSystemComponent.IterationRoundBegin.inl"
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	Camera::Initialize
#		define __GS_COMPONENT_DISPOSE		Camera::Dispose
#	endif
#	include "Common/GameSystemComponent.IterationRoundEnd.inl"
#elif	BOOST_PP_ITERATION() == 2
#	include "Common/GameSystemComponent.IterationRoundBegin.inl"
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE // No active code currently
#		define __GS_COMPONENT_INITIALIZE	Console::Initialize
#		define __GS_COMPONENT_DISPOSE		Console::Dispose
#	endif
#	include "Common/GameSystemComponent.IterationRoundEnd.inl"
#elif	BOOST_PP_ITERATION() == 3
#	include "Common/GameSystemComponent.IterationRoundBegin.inl"
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	GameEngine::Initialize
#		define __GS_COMPONENT_DISPOSE		GameEngine::Dispose
#	endif
#	include "Common/GameSystemComponent.IterationRoundEnd.inl"
#elif	BOOST_PP_ITERATION() == 4
#	include "Common/GameSystemComponent.IterationRoundBegin.inl"
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	Networking::Initialize
#		define __GS_COMPONENT_DISPOSE		Networking::Dispose
#	endif
#	include "Common/GameSystemComponent.IterationRoundEnd.inl"
#elif	BOOST_PP_ITERATION() == 5
#	include "Common/GameSystemComponent.IterationRoundBegin.inl"
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	Players::Initialize
#		define __GS_COMPONENT_DISPOSE		Players::Dispose
											// No active code in Update game hook currently
#	endif
#	include "Common/GameSystemComponent.IterationRoundEnd.inl"
#elif	BOOST_PP_ITERATION() == 6 // No active code currently
#	include "Common/GameSystemComponent.IterationRoundBegin.inl"
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	Effects::Initialize
#		define __GS_COMPONENT_DISPOSE		Effects::Dispose
#	endif
#	include "Common/GameSystemComponent.IterationRoundEnd.inl"
#elif	BOOST_PP_ITERATION() == 7 // No active code currently
#	include "Common/GameSystemComponent.IterationRoundBegin.inl"
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	AI::Initialize
#		define __GS_COMPONENT_DISPOSE		AI::Dispose
#	endif
#	include "Common/GameSystemComponent.IterationRoundEnd.inl"
#elif	BOOST_PP_ITERATION() == 8
#	include "Common/GameSystemComponent.IterationRoundBegin.inl"
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
#	include "Common/GameSystemComponent.IterationRoundEnd.inl"
#endif