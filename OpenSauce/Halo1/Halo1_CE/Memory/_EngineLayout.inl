/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Memory/_EngineLayoutDefinitions.hpp"

#include <boost/preprocessor/iteration/iterate.hpp>

#if !BOOST_PP_IS_ITERATING

	#if defined(__EL_INCLUDE_ID)
		#define __EL_INCLUDE_FILE_MACRO(file)	\
			BOOST_PP_STRINGIZE( BOOST_PP_CAT(Memory/, BOOST_PP_CAT(PLATFORM_VERSION_VALUE, /##file) )

		#define BOOST_PP_ITERATION_PARAMS_1 (3, (__EL_INCLUDE_ID, __EL_INCLUDE_ID, "Memory/_EngineLayout.inl"))
		??=include BOOST_PP_ITERATE()
		#undef __EL_INCLUDE_ID
		#undef __EL_INCLUDE_FILE_ID

		#undef __EL_INCLUDE_FILE_MACRO
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