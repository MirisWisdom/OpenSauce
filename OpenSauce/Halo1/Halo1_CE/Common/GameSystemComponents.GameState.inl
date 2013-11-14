/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#if		BOOST_PP_ITERATION() == 1
#	include "Common/GameSystemComponent.IterationRoundBegin.inl"
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	GameState::Initialize
#		define __GS_COMPONENT_DISPOSE		GameState::Dispose
#	endif
#	include "Common/GameSystemComponent.IterationRoundEnd.inl"
#elif	BOOST_PP_ITERATION() == 2
#	include "Common/GameSystemComponent.IterationRoundBegin.inl"
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE				BuildNumber::Initialize
#		define __GS_COMPONENT_DISPOSE					BuildNumber::Dispose
#	elif	__GS_COMPONENT == __GS_COMPONENT_MAP_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE_FOR_NEW_MAP	BuildNumber::InitializeForNewMap
#	endif
#	include "Common/GameSystemComponent.IterationRoundEnd.inl"
#endif