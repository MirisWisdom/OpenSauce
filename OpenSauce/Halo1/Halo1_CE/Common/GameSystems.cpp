/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Common/GameSystems.hpp"

#include "Common/YeloSettings.hpp"
#include "Common/FileIO.hpp"

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
	#include "Game/GameStateRuntimeData.hpp"
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
#endif
	};
};


#undef API_YELO_NO_PROJECT_COMPONENTS
#undef API_YELO_NO_DX_COMPONENTS
#undef API_YELO_NO_DX_PP_SUBSYSTEM_COMPONENTS