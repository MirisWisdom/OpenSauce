/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"

#include <XboxInterface.hpp>
#include "Common/GameSystems.hpp"
#include "Game/EngineFunctions.hpp"


#if defined(__cplusplus)
extern "C" {
#endif

void XBOXAPI ModuleExit()
{
	Yelo::Main::s_project_component* components;
	const Yelo::int32 component_count = Yelo::Main::GetProjectComponents(components);

	for(Yelo::int32 x = component_count; x >= 0; x--)
		components[x].Dispose();
}

xbox_lib_module_exit_proc XBOXAPI ModuleMain()
// since we're not using the normal way to load the dll
// we don't use the normal Main signature. This avoids unneeded
// overhead when calling Main from Halo2
{
	Yelo::Main::s_project_component* components;
	const Yelo::int32 component_count = Yelo::Main::GetProjectComponents(components);

	for(Yelo::int32 x = 0; x <= component_count; x++)
		components[x].Initialize();
	Yelo::Engine::Interface::HudMessagePrint(L"Yelo ate halo2.");

	return &ModuleExit;
}

#if defined(__cplusplus)
};
#endif