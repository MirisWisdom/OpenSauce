/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#if		BOOST_PP_ITERATION() == 1
#	include "Common/GameSystemComponent.IterationRoundBegin.inl"
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	GameUI::Initialize
#		define __GS_COMPONENT_DISPOSE		GameUI::Dispose
#	else
#		define __GS_COMPONENT_INCLUDE_IGNORE_CURRENT_ROUND
#	endif
#	include "Common/GameSystemComponent.IterationRoundEnd.inl"
#elif	BOOST_PP_ITERATION() == 2 // No active code currently
#	include "Common/GameSystemComponent.IterationRoundBegin.inl"
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	Input::Initialize
#		define __GS_COMPONENT_DISPOSE		Input::Dispose
#	else
#		define __GS_COMPONENT_INCLUDE_IGNORE_CURRENT_ROUND
#	endif
#	include "Common/GameSystemComponent.IterationRoundEnd.inl"
#elif	BOOST_PP_ITERATION() == 3
#	include "Common/GameSystemComponent.IterationRoundBegin.inl"
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	Keystone::Initialize
#		define __GS_COMPONENT_DISPOSE		Keystone::Dispose
#	else
#		define __GS_COMPONENT_INCLUDE_IGNORE_CURRENT_ROUND
#	endif
#	include "Common/GameSystemComponent.IterationRoundEnd.inl"
#endif