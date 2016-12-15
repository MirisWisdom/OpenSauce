/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\Halo2_CheApe for specific license information
*/
#include "Common/Precompile.hpp"

#include "CheApeInterface.hpp"
#include "Engine/EngineFunctions.hpp"


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
// 		Yelo::CheApe::PhysicalMemoryMapInitialize();
// 		Yelo::CheApe::LoadCacheFile();
// 		if(Yelo::CheApe::_InitError == Yelo::CheApe::k_error_LoadCacheFile)
// 		{
// 			YELO_ERROR(_error_message_priority_none, 
// 				"CheApe: cache failure");
// 			goto dispose;
// 		}
// 		Yelo::CheApe::SetupTagGroupPointers();
// 		Yelo::CheApe::SetupTagGroupCounts();
// 		Yelo::CheApe::UpdateTagGroupReferences();

		if(Yelo::CheApe::_InitError == Yelo::CheApe::k_error_none)
			Yelo::Initialize();
		else
			Yelo::EngineFunctions::error(Yelo::Enums::_error_message_priority_none, "CheApe: Yelo initialization failed!");

		g_initialized = true;
	}
	else if(ul_reason_for_call == DLL_PROCESS_DETACH && g_initialized) {
//dispose:
		if(Yelo::CheApe::_InitError == Yelo::CheApe::k_error_none)
			Yelo::Dispose();

//		Yelo::CheApe::Dispose();

		Yelo::Main::YeloModuleHandle() = NULL;
		g_initialized = false;
	}

	return TRUE;
}