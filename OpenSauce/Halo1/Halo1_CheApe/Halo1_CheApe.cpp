/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"

#include "CheApeInterface.hpp"
#include "Engine/EngineFunctions.hpp"
#include "Settings/Settings.hpp"
#include "Yelo/MemoryFixups.hpp"


namespace Yelo
{
	namespace Main
	{
		bool IsYeloEnabled()
		{
			return true;
		}

		HMODULE& YeloModuleHandle()
		{
			static HMODULE g_module = nullptr;

			return g_module;
		}
	};
};

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	static bool g_initialized = false;

	if(ul_reason_for_call == DLL_PROCESS_ATTACH && !g_initialized) {
		Yelo::Main::YeloModuleHandle() = hModule;

		Yelo::CheApe::UnProtectMemoryRegion();
		
		Yelo::Settings::InitializeSettings();
		Yelo::Settings::Load();
		Yelo::Debug::FileInitialize();
		Yelo::c_memory_fixups::InitializePaths();

		Yelo::CheApe::PhysicalMemoryMapInitialize();
		Yelo::CheApe::LoadCacheFile();
		if(Yelo::CheApe::_InitError == Yelo::CheApe::k_error_LoadCacheFile)
		{
			Yelo::Debug::Write("CheApe: cache failure");
			goto dispose;
		}
		Yelo::CheApe::SetupTagGroupPointers();
		Yelo::CheApe::SetupTagGroupCounts();
		Yelo::CheApe::UpdateTagGroupReferences();

		if(Yelo::CheApe::_InitError == Yelo::CheApe::k_error_none)
			Yelo::Initialize();
		else
		{
			Yelo::Debug::Write("CheApe: Yelo initialization failed!");

			std::string msg;
			msg.append("Failed to initialize. Check your ");
			msg.append(Yelo::Settings::K_DEBUG_FILENAME);
			msg.append(" file for details.");
			Yelo::PrepareToDropError(msg.c_str());
		}

		g_initialized = true;
	}
	else if(ul_reason_for_call == DLL_PROCESS_DETACH && g_initialized) {
dispose:
		if(Yelo::CheApe::_InitError == Yelo::CheApe::k_error_none)
			Yelo::Dispose();

		Yelo::CheApe::Dispose();

		Yelo::c_memory_fixups::DisposePaths();
		Yelo::Debug::FileDispose();
		Yelo::Settings::Save();
		Yelo::Settings::DisposeSettings();

		Yelo::Main::YeloModuleHandle() = nullptr;
		g_initialized = false;
	}

    return TRUE;
}