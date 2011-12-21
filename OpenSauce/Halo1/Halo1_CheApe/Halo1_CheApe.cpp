/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"

#include "Common/YeloSettings.hpp"
#include "Engine/EngineFunctions.hpp"
#include "CheApeInterface.hpp"
#include "Yelo/MemoryFixups.hpp"


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	static bool g_initialized = false;

	if(ul_reason_for_call == DLL_PROCESS_ATTACH && !g_initialized) {
		Yelo::CheApe::UnProtectMemoryRegion();

		Yelo::Settings::Initialize();
		Yelo::c_memory_fixups::InitializePaths();

		Yelo::CheApe::PhysicalMemoryMapInitialize();
		Yelo::CheApe::LoadCacheFile();
		if(Yelo::CheApe::_InitError == Yelo::CheApe::k_error_LoadCacheFile)
		{
			YELO_ERROR(_error_message_priority_none, 
				"CheApe: cache failure");
			goto dispose;
		}
		Yelo::CheApe::SetupTagGroupPointers();
		Yelo::CheApe::SetupTagGroupCounts();
		Yelo::CheApe::UpdateTagGroupReferences();

		if(Yelo::CheApe::_InitError == Yelo::CheApe::k_error_none)
			Yelo::Initialize();
		else
			Yelo::EngineFunctions::error(Yelo::Enums::_error_message_priority_none, "CheApe: Yelo initialization failed!");

		g_initialized = true;
	}
	else if(ul_reason_for_call == DLL_PROCESS_DETACH && g_initialized) {
dispose:
		if(Yelo::CheApe::_InitError == Yelo::CheApe::k_error_none)
			Yelo::Dispose();

		Yelo::CheApe::Dispose();

		Yelo::c_memory_fixups::DisposePaths();
		Yelo::Settings::Dispose();

		g_initialized = false;
	}

    return TRUE;
}