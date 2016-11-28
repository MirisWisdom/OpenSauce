/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/GameSystemComponent.IterationRoundBegin.inl"

#if		BOOST_PP_ITERATION() == 1
#	if		__GS_COMPONENT == __GS_COMPONENT_LIFECYCLE
#		define __GS_COMPONENT_INITIALIZE	Memory::c_function_interface_system::Initialize
#		define __GS_COMPONENT_DISPOSE		Memory::c_function_interface_system::Dispose
#	endif
#endif

#include "Common/GameSystemComponent.IterationRoundEnd.inl"