/*
    Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition
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