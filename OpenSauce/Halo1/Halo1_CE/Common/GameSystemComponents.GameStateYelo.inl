/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/GameSystemComponent.IterationRoundBegin.inl"

#if		BOOST_PP_ITERATION() == 1
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE					GameState::RuntimeData::Initialize
#		define __GS_COMPONENT_DISPOSE						GameState::RuntimeData::Dispose
#	elif	__GS_COMPONENT == __GS_COMPONENT_MAP_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE_FOR_NEW_MAP		GameState::RuntimeData::InitializeForNewMap
#	elif	__GS_COMPONENT == __GS_COMPONENT_GAMESTATE_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE_FOR_NEW_GAME_STATE	GameState::RuntimeData::InitializeForNewGameState
#	endif
#elif	BOOST_PP_ITERATION() == 2
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	Scenario::Initialize
#		define __GS_COMPONENT_DISPOSE		Scenario::Dispose
#	endif
#endif

#include "Common/GameSystemComponent.IterationRoundEnd.inl"