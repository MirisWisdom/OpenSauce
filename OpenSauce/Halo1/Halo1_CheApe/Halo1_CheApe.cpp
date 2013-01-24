/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"

#include "CheApeInterface.hpp"
#include "Engine/EngineFunctions.hpp"
#include "Common/YeloSettings.hpp"
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
			static HMODULE g_module = NULL;

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

		Yelo::Settings::Initialize();
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
			Yelo::Debug::Write("CheApe: Yelo initialization failed!");

		g_initialized = true;
	}
	else if(ul_reason_for_call == DLL_PROCESS_DETACH && g_initialized) {
dispose:
		if(Yelo::CheApe::_InitError == Yelo::CheApe::k_error_none)
			Yelo::Dispose();

		Yelo::CheApe::Dispose();

		Yelo::c_memory_fixups::DisposePaths();
		Yelo::Debug::FileDispose();
		Yelo::Settings::Dispose();

		Yelo::Main::YeloModuleHandle() = NULL;
		g_initialized = false;
	}

    return TRUE;
}