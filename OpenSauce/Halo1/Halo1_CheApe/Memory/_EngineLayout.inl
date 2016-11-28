/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Memory/_EngineLayoutDefinitions.hpp"

#include <boost/preprocessor/iteration/iterate.hpp>

#if !BOOST_PP_IS_ITERATING

	#if defined(__EL_INCLUDE_ID)
		#define BOOST_PP_ITERATION_PARAMS_1 (3, (__EL_INCLUDE_ID, __EL_INCLUDE_ID, "Memory/_EngineLayout.inl"))
		??=include BOOST_PP_ITERATE()
		#undef __EL_INCLUDE_ID
		#undef __EL_INCLUDE_FILE_ID
	#endif

//////////////////////////////////////////////////////////////////////////
// Common
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_OPEN_SAUCE
	#include "Memory/_EngineLayout.OpenSauce.inl"

//////////////////////////////////////////////////////////////////////////
// Game
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_GAME
	#include "Memory/_EngineLayout.Game.inl"

//////////////////////////////////////////////////////////////////////////
// Interface
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_INTERFACE
	#include "Memory/_EngineLayout.Interface.inl"

//////////////////////////////////////////////////////////////////////////
// Networking
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_NETWORKING
	#include "Memory/_EngineLayout.Networking.inl"

//////////////////////////////////////////////////////////////////////////
// Objects
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_OBJECTS
	#include "Memory/_EngineLayout.Objects.inl"

//////////////////////////////////////////////////////////////////////////
// Rasterizer
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_RASTERIZER
	#include "Memory/_EngineLayout.Rasterizer.inl"
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_RASTERIZER_DX9
	#include "Memory/_EngineLayout.Rasterizer.DX9.inl"
	
//////////////////////////////////////////////////////////////////////////
// TagGroups
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_TAGGROUPS
	#include "Memory/_EngineLayout.TagGroups.inl"

//#elif BOOST_PP_ITERATION() == 
#else
#endif