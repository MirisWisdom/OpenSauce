/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/GameSystemComponent.IterationRoundBegin.inl"

#if		BOOST_PP_ITERATION() == 1
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	GameUI::Initialize
#		define __GS_COMPONENT_DISPOSE		GameUI::Dispose
#	endif
#elif	BOOST_PP_ITERATION() == 2 // No active code currently
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	Input::Initialize
#		define __GS_COMPONENT_DISPOSE		Input::Dispose
#	endif
#elif	BOOST_PP_ITERATION() == 3
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	Keystone::Initialize
#		define __GS_COMPONENT_DISPOSE		Keystone::Dispose
#	endif
#endif

#include "Common/GameSystemComponent.IterationRoundEnd.inl"