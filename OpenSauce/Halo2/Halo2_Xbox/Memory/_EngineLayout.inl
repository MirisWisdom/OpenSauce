/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
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
// Open Sauce
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_OPEN_SAUCE
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.OpenSauce.inl)


//////////////////////////////////////////////////////////////////////////
// AI
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_AI
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.AI.inl)


//////////////////////////////////////////////////////////////////////////
// Cache
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_CACHE
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.Cache.inl)


//////////////////////////////////////////////////////////////////////////
// Effects
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_EFFECTS
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.Effects.inl)


//////////////////////////////////////////////////////////////////////////
// Game
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_GAME
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.Game.inl)


//////////////////////////////////////////////////////////////////////////
// HS
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_HS
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.HS.inl)


//////////////////////////////////////////////////////////////////////////
// Interface
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_INTERFACE
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.Interface.inl)


//////////////////////////////////////////////////////////////////////////
// Networking
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_NETWORKING
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.Networking.inl)


//////////////////////////////////////////////////////////////////////////
// Objects
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_OBJECTS
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.Objects.inl)


//////////////////////////////////////////////////////////////////////////
// Physics
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_PHYSICS
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.Physics.inl)


//////////////////////////////////////////////////////////////////////////
// Sound
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_SOUND
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.Sound.inl)


//////////////////////////////////////////////////////////////////////////
// Structures
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_STRUCTURES
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.Structures.inl)


//#elif BOOST_PP_ITERATION() == 
#else
#endif