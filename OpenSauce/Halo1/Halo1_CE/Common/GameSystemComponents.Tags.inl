/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#if		BOOST_PP_ITERATION() == 1
#	include "Common/GameSystemComponent.IterationRoundBegin.inl"
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	Cache::Initialize
#		define __GS_COMPONENT_DISPOSE		Cache::Dispose
#	endif
#	include "Common/GameSystemComponent.IterationRoundEnd.inl"
#elif	BOOST_PP_ITERATION() == 2
#	include "Common/GameSystemComponent.IterationRoundBegin.inl"
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	DataFiles::Initialize
#		define __GS_COMPONENT_DISPOSE		DataFiles::Dispose
#	endif
#	include "Common/GameSystemComponent.IterationRoundEnd.inl"
#elif	BOOST_PP_ITERATION() == 3
#	include "Common/GameSystemComponent.IterationRoundBegin.inl"
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE				TagGroups::Initialize
#		define __GS_COMPONENT_DISPOSE					TagGroups::Dispose
#	elif	__GS_COMPONENT == __GS_COMPONENT_MAP_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE_FOR_NEW_MAP	TagGroups::InitializeForNewMap
#	endif
#	include "Common/GameSystemComponent.IterationRoundEnd.inl"
#endif