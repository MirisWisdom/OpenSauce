/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#if		BOOST_PP_ITERATION() == 1
#	include "Common/GameSystemComponent.IterationRoundBegin.inl"
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	Scripting::Initialize
#		define __GS_COMPONENT_DISPOSE		Scripting::Dispose
											// No active code in Update game hook currently
#	endif
#	include "Common/GameSystemComponent.IterationRoundEnd.inl"
#endif