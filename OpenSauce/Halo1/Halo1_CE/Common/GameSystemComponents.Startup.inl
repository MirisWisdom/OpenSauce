/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/GameSystemComponent.IterationRoundBegin.inl"

#if		BOOST_PP_ITERATION() == 1
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	FileIO::Initialize
#		define __GS_COMPONENT_DISPOSE		FileIO::Dispose
#	endif
#elif	BOOST_PP_ITERATION() == 2 && defined(API_DEBUG)
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	Debug::FileInitialize
#		define __GS_COMPONENT_DISPOSE		Debug::FileDispose
#	endif
#elif	BOOST_PP_ITERATION() == 3 // compile in release too to get dumps from users
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	Debug::DumpInitialize
#		define __GS_COMPONENT_DISPOSE		Debug::DumpDispose
#		define __GS_COMPONENT_UPDATE		Debug::Update
#	endif
#endif

#include "Common/GameSystemComponent.IterationRoundEnd.inl"