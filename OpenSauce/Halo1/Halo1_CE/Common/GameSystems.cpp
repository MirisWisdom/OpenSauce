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

		static void PLATFORM_API InitializeOnStartup()
		{
			Settings::InitializeSettings();

			FileIO::Initialize();
			#ifdef API_DEBUG
			Debug::FileInitialize();
			#endif
			Debug::DumpInitialize();
			GameState::Initialize();
			BuildNumber::Initialize();
			Cache::Initialize();
			DataFiles::Initialize();
			TagGroups::Initialize();
			Scripting::Initialize();
			GameState::RuntimeData::Initialize();
			Scenario::Initialize();
			#if PLATFORM_IS_USER
			Rasterizer::DeviceHooks::Initialize();
			DX9::Initialize();
			DX9::c_gbuffer_system::Initialize();
			Rasterizer::Initialize();
			Rasterizer::ShaderDraw::Initialize();
			Rasterizer::ShaderExtension::Initialize();
			Rasterizer::PostProcessing::Initialize();
			Render::Lightmaps::Initialize();
			Render::Sky::Initialize();
			StructureBSP::Initialize();
			Camera::Initialize();
			GameUI::Initialize();
			Input::Initialize();
			Keystone::Initialize();
			Interface::OpenSauceUI::Initialize();
			#endif
			#if PLATFORM_IS_DEDI
			Server::Initialize();
			Networking::HTTP::Server::Initialize();
			Networking::HTTP::Server::MapDownload::Initialize();
			#endif
			Console::Initialize();
			GameEngine::Initialize();
			Networking::Initialize();
			Players::Initialize();
			Effects::Initialize();
			AI::Initialize();
			Objects::Initialize();
			Networking::HTTP::Client::Initialize();
			Networking::VersionCheck::Initialize();
			Memory::c_function_interface_system::Initialize();

			Settings::Load();
		}

		static void PLATFORM_API DisposeOnExit()
		{
			Settings::Save();

			FileIO::Dispose();
			#ifdef API_DEBUG
			Debug::FileDispose();
			#endif
			Debug::DumpDispose();
			GameState::Dispose();
			BuildNumber::Dispose();
			Cache::Dispose();
			DataFiles::Dispose();
			TagGroups::Dispose();
			Scripting::Dispose();
			GameState::RuntimeData::Dispose();
			Scenario::Dispose();
			#if PLATFORM_IS_USER
			Rasterizer::DeviceHooks::Dispose();
			DX9::Dispose();
			DX9::c_gbuffer_system::Dispose();
			Rasterizer::Dispose();
			Rasterizer::ShaderDraw::Dispose();
			Rasterizer::ShaderExtension::Dispose();
			Rasterizer::PostProcessing::Dispose();
			Render::Lightmaps::Dispose();
			Render::Sky::Dispose();
			StructureBSP::Dispose();
			Camera::Dispose();
			GameUI::Dispose();
			Input::Dispose();
			Keystone::Dispose();
			Interface::OpenSauceUI::Dispose();
			#endif
			#if PLATFORM_IS_DEDI
			Server::Dispose();
			Networking::HTTP::Server::Dispose();
			Networking::HTTP::Server::MapDownload::Dispose();
			#endif
			Console::Dispose();
			GameEngine::Dispose();
			Networking::Dispose();
			Players::Dispose();
			Effects::Dispose();
			AI::Dispose();
			Objects::Dispose();
			Networking::HTTP::Client::Dispose();
			Networking::VersionCheck::Dispose();
			Memory::c_function_interface_system::Dispose();

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
