/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Common/GameSystems.hpp"

#include "Settings/Settings.hpp"
#include "Common/FileIO.hpp"

//////////////////////////////////////////////////////////////////////////
// User only includes
	#if PLATFORM_IS_USER
		#include "Rasterizer/DX9/DxWrapper.hpp"
		#include "Rasterizer/DX9/DX9.hpp"
		#include "Rasterizer/DX9/DeviceHooks.hpp"
		#include "Rasterizer/Rasterizer.hpp"
		#include "Rasterizer/ShaderDraw.hpp"
		#include "Rasterizer/GBuffer.hpp"
		#include "Rasterizer/ShaderExtension/ShaderExtension.hpp"
		#include "Rasterizer/PostProcessing/PostProcessing.hpp"
		#include "Rasterizer/Lightmaps.hpp"
		#include "Rasterizer/Sky.hpp"
		#include "Rasterizer/Textures/c_packed_texture_loader.hpp"
		#include "Game/Camera.hpp"

		#include "Interface/GameUI.hpp"
		#include "Interface/Controls.hpp"
		#include "Interface/Keystone.hpp"
		#include "Interface/OpenSauceUI.hpp"
	#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Debug only includes
	#ifdef API_DEBUG

#endif
	// this is for debugging but it needs to be in the release build too
	#include "Common/DebugDump.hpp"
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Component includes
	#include "Game/GameState.hpp"
	#include "Game/GameBuildNumber.hpp"
	#include "TagGroups/CacheFiles.hpp"
	#include "TagGroups/TagGroups.hpp"

	#include "Game/Console.hpp"

	#include "Game/Scripting.hpp"
	#include "Game/GameStateRuntimeData.hpp"
	#include "Scenario/Scenario.hpp"
	#include "Scenario/StructureBSP.hpp"
	#include "Game/GameEngine.hpp"

	#include "Networking/Networking.hpp"
	#include "Game/Players.hpp"

	#include "Game/AI.hpp"
	#include "Game/Effects.hpp"
	#include "Objects/Objects.hpp"

	#include "Memory/FunctionInterface.hpp"

	#include "Networking/VersionCheck.hpp"
//////////////////////////////////////////////////////////////////////////

#if PLATFORM_IS_DEDI
	#include "Networking/Server.hpp"
	#include "Networking/HTTP/HTTPServer.hpp"
	#include "Networking/HTTP/MapDownloadServer.hpp"
#else
	#include "Networking/HTTP/MapDownloadClient.hpp"
#endif
	#include "Networking/HTTP/HTTPClient.hpp"

//#define API_YELO_NO_PROJECT_COMPONENTS
//#define API_YELO_NO_DX_COMPONENTS

#define __GS_COMPONENT_LIFECYCLE 1
#define __GS_COMPONENT_MAP_LIFECYCLE 2
#define __GS_COMPONENT_GAMESTATE_LIFECYCLE 3
#define __GS_COMPONENT_DX9_LIFECYCLE 4
#define __GS_COMPONENT_BSP_LIFECYCLE 5


namespace Yelo
{
	namespace Main
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_OPEN_SAUCE
#define __EL_INCLUDE_FILE_ID	__EL_COMMON_GAME_SYSTEMS
#include "Memory/_EngineLayout.inl"

		int32 GetProjectComponents(s_project_component*& out_components)
		{
			out_components = nullptr;
			int32 components_count = NONE;

#if !defined(API_YELO_NO_PROJECT_COMPONENTS)
			static s_project_component k_components[] = {
				#define __GS_COMPONENT __GS_COMPONENT_LIFECYCLE
				#include "Common/GameSystemComponent.Definitions.inl"
				{}
			};

			out_components = k_components;
			components_count = NUMBEROF(k_components)-1 - 1;
#endif

			return components_count;
		}
		int32 GetProjectComponents(s_project_map_component*& out_components)
		{
			out_components = nullptr;
			int32 components_count = NONE;

#if !defined(API_YELO_NO_PROJECT_COMPONENTS)
			static s_project_map_component k_components[] = {
				#define __GS_COMPONENT __GS_COMPONENT_MAP_LIFECYCLE
				#include "Common/GameSystemComponent.Definitions.inl"
				{}
			};

			out_components = k_components;
			components_count = NUMBEROF(k_components)-1 - 1;
#endif

			return components_count;
		}
		int32 GetProjectComponents(s_project_bsp_component*& out_components)
		{
			out_components = nullptr;
			int32 components_count = NONE;
			
			//NOTE: There are currently no bsp components in the dedicated server
#if PLATFORM_IS_USER
#if !defined(API_YELO_NO_PROJECT_COMPONENTS)
			static s_project_bsp_component k_components[] = {
				#define __GS_COMPONENT __GS_COMPONENT_BSP_LIFECYCLE
				#include "Common/GameSystemComponent.Definitions.inl"
			};

			out_components = k_components;
			components_count = NUMBEROF(k_components)-1;
#endif
#endif

			return components_count;
		}
		int32 GetProjectComponents(s_project_game_state_component*& out_components)
		{
			out_components = nullptr;
			int32 components_count = NONE;

#if !defined(API_YELO_NO_PROJECT_COMPONENTS)
			static s_project_game_state_component k_components[] = {
				#define __GS_COMPONENT __GS_COMPONENT_GAMESTATE_LIFECYCLE
				#include "Common/GameSystemComponent.Definitions.inl"
				{}
			};

			out_components = k_components;
			components_count = NUMBEROF(k_components)-1 - 1;
#endif

			return components_count;
		}


#if PLATFORM_IS_USER
		int32 GetDXComponents(s_dx_component*& out_components)
		{
			out_components = nullptr;
			int32 components_count = NONE;

	#if !defined(API_YELO_NO_DX_COMPONENTS)
			static s_dx_component k_components[] = {
				#define __GS_COMPONENT __GS_COMPONENT_DX9_LIFECYCLE
				#include "Common/GameSystemComponent.Definitions.inl"
				{}
			};

			out_components = k_components;
			components_count = NUMBEROF(k_components)-1 - 1;
	#endif

			return components_count;
		}
#endif

		static void PLATFORM_API InitializeOnStartup()
		{
			s_project_component* components;
			const int32 component_count = GetProjectComponents(components);

			Settings::InitializeSettings();

			for(Yelo::int32 x = 0; x <= component_count; x++)
				components[x].Initialize();
			
			Settings::Load();
		}

		static void PLATFORM_API DisposeOnExit()
		{
			s_project_component* components;
			const int32 component_count = GetProjectComponents(components);
			
			Settings::Save();

			for(int32 x = component_count; x >= 0; x--)
				components[x].Dispose();

			Settings::DisposeSettings();
		}

		// hooks the call that starts the main game loop to init OS beforehand
		static void PLATFORM_API InitializeOnStartupHook()
		{
			static uintptr_t TEMP_ADDRESS = GET_FUNC_PTR(QUERY_EXITFLAG_REG);

			_asm push ebx

			InitializeOnStartup();

			_asm pop ebx

			_asm call TEMP_ADDRESS;

		}

		// hooks the call that release resources to dispose of OS systems afterwards
		static void PLATFORM_API DisposeOnExitHook()
		{
			static uintptr_t TEMP_ADDRESS = GET_FUNC_PTR(RELEASE_RESOURCES_ON_EXIT);

			_asm call TEMP_ADDRESS;

			DisposeOnExit();
		}

		void InsertHooks()
		{
//////////////////////////////////////////////////////////////////////////
// Unprotect the exe's code so we can freely modify it
			DWORD old;
			VirtualProtect(CAST_PTR(void*, 0x400000),GET_DATA_PTR(PE_DATA_SIZE),PAGE_EXECUTE_READWRITE,&old);
//////////////////////////////////////////////////////////////////////////

			Memory::WriteRelativeCall(&InitializeOnStartupHook, GET_FUNC_VPTR(QUERY_EXITFLAG_REG_CALL), true);
			Memory::WriteRelativeCall(&DisposeOnExitHook, GET_FUNC_VPTR(RELEASE_RESOURCES_ON_EXIT_CALL), true);
		}
	};
};


#undef API_YELO_NO_PROJECT_COMPONENTS
#undef API_YELO_NO_DX_COMPONENTS

#undef __GS_COMPONENT_LIFECYCLE
#undef __GS_COMPONENT_MAP_LIFECYCLE
#undef __GS_COMPONENT_GAMESTATE_LIFECYCLE
#undef __GS_COMPONENT_DX9_LIFECYCLE
#undef __GS_COMPONENT_BSP_LIFECYCLE