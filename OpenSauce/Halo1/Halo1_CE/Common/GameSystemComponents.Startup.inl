/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#if		BOOST_PP_ITERATION() == 1
#	include "Common/GameSystemComponent.IterationRoundBegin.inl"
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	Settings::Initialize
#		define __GS_COMPONENT_DISPOSE		Settings::Dispose
#	endif
#	include "Common/GameSystemComponent.IterationRoundEnd.inl"
#elif	BOOST_PP_ITERATION() == 2
#	include "Common/GameSystemComponent.IterationRoundBegin.inl"
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	FileIO::Initialize
#		define __GS_COMPONENT_DISPOSE		FileIO::Dispose
#	endif
#	include "Common/GameSystemComponent.IterationRoundEnd.inl"
#elif	BOOST_PP_ITERATION() == 3 && defined(API_DEBUG)
#	include "Common/GameSystemComponent.IterationRoundBegin.inl"
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	Debug::FileInitialize
#		define __GS_COMPONENT_DISPOSE		Debug::FileDispose
#	endif
#	include "Common/GameSystemComponent.IterationRoundEnd.inl"
#elif	BOOST_PP_ITERATION() == 4 && PLATFORM_IS_USER // compile in release too to get dumps from users
#	include "Common/GameSystemComponent.IterationRoundBegin.inl"
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	Debug::DumpInitialize
#		define __GS_COMPONENT_DISPOSE		Debug::DumpDispose
#		define __GS_COMPONENT_UPDATE		Debug::Update
#	endif
#	include "Common/GameSystemComponent.IterationRoundEnd.inl"
#endif