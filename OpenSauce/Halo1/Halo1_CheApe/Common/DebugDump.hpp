/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Debug
	{
		// call the stock CWinApp::Run()
		// not actually __stdcall, but __thiscall (first param is 'this'!)
		// stdcall gives us the desired stack behavior, without having to explicitly pass a 'this' pointer (ecx)
		typedef int(__stdcall* winapp_run_t)(void);

		void DumpInitialize();
		void DumpDispose();
	};
};