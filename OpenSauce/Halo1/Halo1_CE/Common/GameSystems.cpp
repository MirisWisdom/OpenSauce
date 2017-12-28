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
#include "Rasterizer/DX9/DX9.hpp"
#include "Rasterizer/DX9/DeviceHooks.hpp"
#include "Rasterizer/Rasterizer.hpp"
#include "Rasterizer/ShaderDraw.hpp"
#include "Rasterizer/GBuffer.hpp"
#include "Rasterizer/ShaderExtension/ShaderExtension.hpp"
#include "Rasterizer/PostProcessing/PostProcessing.hpp"
#include "Rasterizer/Lightmaps.hpp"
#include "Rasterizer/Sky.hpp"
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

namespace Yelo
{
	namespace Main
	{
		#define __EL_INCLUDE_ID			__EL_INCLUDE_OPEN_SAUCE
		#define __EL_INCLUDE_FILE_ID	__EL_COMMON_GAME_SYSTEMS
		#include "Memory/_EngineLayout.inl"

		int32 GetProjectComponents(
			s_project_component*& out_components)
		{
			out_components = nullptr;
			int32 components_count = NONE;

			static s_project_component k_components[] = {
				{FileIO::Initialize , FileIO::Dispose ,nullptr},
				#ifdef API_DEBUG
				{Debug::FileInitialize , Debug::FileDispose , nullptr},
				#endif
				{Debug::DumpInitialize , Debug::DumpDispose , Debug::Update},
				{GameState::Initialize , GameState::Dispose , nullptr},
				{BuildNumber::Initialize , BuildNumber::Dispose , nullptr},
				{Cache::Initialize , Cache::Dispose , nullptr},
				{DataFiles::Initialize , DataFiles::Dispose , nullptr},
				{TagGroups::Initialize , TagGroups::Dispose , nullptr},
				{Scripting::Initialize , Scripting::Dispose , nullptr},
				{GameState::RuntimeData::Initialize , GameState::RuntimeData::Dispose , nullptr},
				{Scenario::Initialize , Scenario::Dispose , nullptr},
				#if PLATFORM_IS_USER
				{Rasterizer::DeviceHooks::Initialize , Rasterizer::DeviceHooks::Dispose , nullptr},
				{DX9::Initialize , DX9::Dispose , nullptr},
				{DX9::c_gbuffer_system::Initialize , DX9::c_gbuffer_system::Dispose , DX9::c_gbuffer_system::Update},
				{Rasterizer::Initialize , Rasterizer::Dispose , nullptr},
				{Rasterizer::ShaderDraw::Initialize , Rasterizer::ShaderDraw::Dispose , nullptr},
				{Rasterizer::ShaderExtension::Initialize , Rasterizer::ShaderExtension::Dispose , nullptr},
				{Rasterizer::PostProcessing::Initialize , Rasterizer::PostProcessing::Dispose , Rasterizer::PostProcessing::Update},
				{Render::Lightmaps::Initialize , Render::Lightmaps::Dispose , nullptr},
				{Render::Sky::Initialize , Render::Sky::Dispose , nullptr},
				{StructureBSP::Initialize , StructureBSP::Dispose , nullptr},
				{Camera::Initialize , Camera::Dispose , nullptr},
				#endif

				{Console::Initialize , Console::Dispose , nullptr},
				{GameEngine::Initialize , GameEngine::Dispose , nullptr},
				{Networking::Initialize , Networking::Dispose , nullptr},
				{Players::Initialize , Players::Dispose , nullptr},
				{Effects::Initialize , Effects::Dispose , nullptr},
				{AI::Initialize , AI::Dispose , nullptr},
				{Objects::Initialize , Objects::Dispose , nullptr},

				#if PLATFORM_IS_USER
				{GameUI::Initialize , GameUI::Dispose , nullptr},
				{Input::Initialize , Input::Dispose , nullptr},
				{Keystone::Initialize , Keystone::Dispose , nullptr},
				{Interface::OpenSauceUI::Initialize , Interface::OpenSauceUI::Dispose , nullptr},
				#endif

				#if PLATFORM_IS_DEDI
				{ Server::Initialize , Server::Dispose , nullptr },
				{ Networking::HTTP::Server::Initialize , Networking::HTTP::Server::Dispose , Networking::HTTP::Server::Update },
				{ Networking::HTTP::Server::MapDownload::Initialize , Networking::HTTP::Server::MapDownload::Dispose , nullptr },
				#endif
				{Networking::HTTP::Client::Initialize , Networking::HTTP::Client::Dispose , Networking::HTTP::Client::Update},
				{Networking::VersionCheck::Initialize , Networking::VersionCheck::Dispose , Networking::VersionCheck::Update},
				{Memory::c_function_interface_system::Initialize , Memory::c_function_interface_system::Dispose , nullptr}
			};

			out_components = k_components;
			components_count = NUMBEROF(k_components);
			return components_count;
		}

		int32 GetProjectComponents(
			s_project_map_component*& out_components)
		{
			out_components = nullptr;
			int32 components_count = NONE;

			static s_project_map_component k_components[] = {
				{BuildNumber::InitializeForNewMap, nullptr},
				{TagGroups::InitializeForNewMap, nullptr},
				{GameState::RuntimeData::InitializeForNewMap, nullptr},
				#if PLATFORM_IS_USER
				{Rasterizer::PostProcessing::InitializeForNewMap, Rasterizer::PostProcessing::DisposeFromOldMap},
				{Render::Sky::InitializeForNewMap, Render::Sky::DisposeFromOldMap},
				{StructureBSP::InitializeForNewMap, StructureBSP::DisposeFromOldMap},
				#endif
				{AI::InitializeForNewMap, AI::DisposeFromOldMap},
				{Objects::InitializeForNewMap, Objects::DisposeFromOldMap},
				{Networking::VersionCheck::InitializeForNewMap, nullptr},
			};

			out_components = k_components;
			components_count = NUMBEROF(k_components);

			return components_count;
		}

		int32 GetProjectComponents(
			s_project_bsp_component*& out_components)
		{
			out_components = nullptr;
			int32 components_count = NONE;

			//NOTE: There are currently no bsp components in the dedicated server
			#if PLATFORM_IS_USER
			static s_project_bsp_component k_components[] = {
				{StructureBSP::InitializeForNewBSP,StructureBSP::DisposeFromOldBSP}
			};

			out_components = k_components;
			components_count = NUMBEROF(k_components);
			#endif

			return components_count;
		}

		int32 GetProjectComponents(
			s_project_game_state_component*& out_components)
		{
			out_components = nullptr;
			int32 components_count = NONE;

			static s_project_game_state_component k_components[] = {
				{GameState::RuntimeData::InitializeForNewGameState, nullptr, nullptr},
				#if PLATFORM_IS_USER
				{StructureBSP::InitializeForNewGameState, nullptr, StructureBSP::HandleGameStateLifeCycle},
				#endif
				{AI::InitializeForNewGameState, nullptr, AI::HandleGameStateLifeCycle},
				{nullptr, Objects::InitializeForYeloGameState, nullptr}
			};

			out_components = k_components;
			components_count = NUMBEROF(k_components);

			return components_count;
		}

		#if PLATFORM_IS_USER
		int32 GetDXComponents(
			s_dx_component*& out_components)
		{
			out_components = nullptr;
			int32 components_count = NONE;

			static s_dx_component k_components[] = {
				{DX9::Initialize3D, DX9::OnLostDevice, DX9::OnResetDevice, DX9::Render, DX9::Release},
				{DX9::c_gbuffer_system::Initialize3D, DX9::c_gbuffer_system::OnLostDevice, DX9::c_gbuffer_system::OnResetDevice, DX9::c_gbuffer_system::Render, DX9::c_gbuffer_system::Release},
				{Rasterizer::ShaderExtension::Initialize3D, Rasterizer::ShaderExtension::OnLostDevice, Rasterizer::ShaderExtension::OnResetDevice, Rasterizer::ShaderExtension::Render, Rasterizer::ShaderExtension::Release},
				{Rasterizer::PostProcessing::Initialize3D, Rasterizer::PostProcessing::OnLostDevice, Rasterizer::PostProcessing::OnResetDevice, Rasterizer::PostProcessing::Render, Rasterizer::PostProcessing::Release},
				{Hud::Initialize3D, Hud::OnLostDevice, Hud::OnResetDevice, Hud::Render, Hud::Release},
				{Interface::OpenSauceUI::Initialize3D, Interface::OpenSauceUI::OnLostDevice, Interface::OpenSauceUI::OnResetDevice, Interface::OpenSauceUI::Render, Interface::OpenSauceUI::Release},
			};

			out_components = k_components;
			components_count = NUMBEROF(k_components);

			return components_count;
		}
		#endif

		static void PLATFORM_API InitializeOnStartup()
		{
			s_project_component* components;
			const int32 component_count = GetProjectComponents(components);

			Settings::InitializeSettings();

			for (int32 x = 0; x < component_count; x++)
				components[x].Initialize();

			Settings::Load();
		}

		static void PLATFORM_API DisposeOnExit()
		{
			s_project_component* components;
			const int32 component_count = GetProjectComponents(components);

			Settings::Save();

			for (int32 x = component_count - 1; x > 0; x--)
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
			VirtualProtect(CAST_PTR(void*, 0x400000),GET_DATA_PTR(PE_DATA_SIZE),PAGE_EXECUTE_READWRITE, &old);
			//////////////////////////////////////////////////////////////////////////

			Memory::WriteRelativeCall(&InitializeOnStartupHook, GET_FUNC_VPTR(QUERY_EXITFLAG_REG_CALL), true);
			Memory::WriteRelativeCall(&DisposeOnExitHook, GET_FUNC_VPTR(RELEASE_RESOURCES_ON_EXIT_CALL), true);
		}
	}
}
