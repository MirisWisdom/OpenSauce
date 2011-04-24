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
#include "Common/Precompile.hpp"
#include "Common/GameSystems.hpp"

#include "Common/YeloSettings.hpp"

//////////////////////////////////////////////////////////////////////////
// User only includes
	#if PLATFORM_IS_USER
		#include "Rasterizer/DX9/DxWrapper.hpp"
		#include "Rasterizer/DX9/DX9.hpp"
		#include "Rasterizer/GBuffer.hpp"
		#include "Rasterizer/Rasterizer.hpp"
		#include "Rasterizer/ShaderExtension/ShaderExtension.hpp"
		#include "Rasterizer/PostProcessing/PostProcessing.hpp"
		#include "Game/Camera.hpp"

		#include "Interface/GameUI.hpp"
		#include "Interface/Controls.hpp"
		#include "Interface/Keystone.hpp"

		#include "Rasterizer/PostProcessing/MotionBlur/PostProcessingSubsystem_MotionBlur.hpp"
		#include "Rasterizer/PostProcessing/Internal/PostProcessingSubsystem_Internal.hpp"
		#include "Rasterizer/PostProcessing/External/PostProcessingSubsystem_External.hpp"
		#include "Rasterizer/PostProcessing/Bloom/PostProcessingSubsystem_Bloom.hpp"

	#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Debug only includes
	#ifdef API_DEBUG
		#include "Common/DebugFile.hpp"
	#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Component includes
	#include "Game/GameState.hpp"
	#include "TagGroups/CacheFiles.hpp"
	#include "TagGroups/TagGroups.hpp"

	#include "Game/Console.hpp"

	#include "Game/Scripting.hpp"
	#include "Game/GameEngine.hpp"

	#include "Networking/Networking.hpp"
	#include "Game/Players.hpp"

	#include "Game/AI.hpp"
	#include "Game/Effects.hpp"
	#include "Objects/Objects.hpp"

	#include "Memory/FunctionInterface.hpp"

	#include "Networking/VersionCheck.hpp"
//////////////////////////////////////////////////////////////////////////


//#define API_YELO_NO_PROJECT_COMPONENTS
//#define API_YELO_NO_DX_COMPONENTS
//#define API_YELO_NO_DX_PP_SUBSYSTEM_COMPONENTS


namespace Yelo
{
	namespace Main
	{
		int32 GetProjectComponents(s_project_component*& out_components)
		{
			out_components = NULL;
			int32 components_count = NONE;

#if !defined(API_YELO_NO_PROJECT_COMPONENTS)
			static s_project_component k_components[] = {
				#include "Common/GameSystems.ProjComponents.inl"
			};

			out_components = k_components;
			components_count = NUMBEROF(k_components)-1;
#endif

			return components_count;
		}


#if PLATFORM_IS_USER
		int32 GetDXComponents(s_dx_component*& out_components)
		{
			out_components = NULL;
			int32 components_count = NONE;

	#if !defined(API_YELO_NO_DX_COMPONENTS)
			static s_dx_component k_components[] = {
				#include "Common/GameSystems.DxComponents.inl"
			};

			out_components = k_components;
			components_count = NUMBEROF(k_components)-1;
	#endif

			return components_count;
		}



		int32 GetPostprocessingSubsystemComponents(s_postprocessing_subsystem_component*& out_components)
		{
			out_components = NULL;
			int32 components_count = NONE;

	#if !defined(API_YELO_NO_DX_COMPONENTS) && !defined(API_YELO_NO_DX_PP_SUBSYSTEM_COMPONENTS)
			static s_postprocessing_subsystem_component k_components[] = {
				#include "Common/GameSystems.PostprocessingSubsystems.inl"
			};

			out_components = k_components;
			components_count = NUMBEROF(k_components)-1;
	#endif

			return components_count;
		}
#endif
	};
};


#undef API_YELO_NO_PROJECT_COMPONENTS
#undef API_YELO_NO_DX_COMPONENTS
#undef API_YELO_NO_DX_PP_SUBSYSTEM_COMPONENTS