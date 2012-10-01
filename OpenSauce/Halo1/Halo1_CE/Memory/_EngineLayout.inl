/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
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
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.OpenSauce.inl)

//////////////////////////////////////////////////////////////////////////
// Game
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_GAME
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.Game.inl)

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
// Rasterizer
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_RASTERIZER
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.Rasterizer.inl)
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_RASTERIZER_DX9
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.Rasterizer.DX9.inl)
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_RASTERIZER_SHADEREXTENSION
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.Rasterizer.ShaderExtension.inl)
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_RASTERIZER_SHADERDRAW
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.Rasterizer.ShaderDraw.inl)
	
//////////////////////////////////////////////////////////////////////////
// TagGroups
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_TAGGROUPS
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.TagGroups.inl)

//////////////////////////////////////////////////////////////////////////
// Simulation
#elif BOOST_PP_ITERATION() == __EL_INCLUDE_SIMULATION
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.Simulation.inl)

//#elif BOOST_PP_ITERATION() == 
#else
#endif