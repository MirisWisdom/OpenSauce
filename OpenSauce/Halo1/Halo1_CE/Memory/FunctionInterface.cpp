/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Memory/FunctionInterface.hpp"

#include "Memory/MemoryInterface.hpp"
#include "Rasterizer/GBuffer.hpp"
#include "Rasterizer/PostProcessing/PostProcessing.hpp"
#include "Interface/GameUI.hpp"

namespace Yelo
{
	namespace Memory
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_OPEN_SAUCE
#define __EL_INCLUDE_FILE_ID	__EL_MEMORY_FUNCTION_INTERFACE
#include "Memory/_EngineLayout.inl"

#if !PLATFORM_IS_DEDI
	#include "Memory/FunctionInterface.Client.inl"
#endif

		void c_function_interface_system::Initialize()
		{
#if !PLATFORM_IS_DEDI
			WRITE_HOOK_BLOCK_PROCESS(render_sky, GET_FUNC_PTR(RENDER_WINDOW_CALL_HOOK_RENDER_SKY));
			WRITE_HOOK_BLOCK_PROCESS(render_objects, GET_FUNC_PTR(RENDER_WINDOW_CALL_HOOK_RENDER_OBJECTS));
			WRITE_HOOK_BLOCK_PROCESS(render_structure, GET_FUNC_PTR(RENDER_WINDOW_CALL_HOOK_STRUCTURE_RENDER_LIGHTMAPS));
			WRITE_HOOK_BLOCK_PROCESS(weather_particle_systems_render, GET_FUNC_PTR(RENDER_WINDOW_CALL_HOOK_WEATHER_PARTICLE_SYSTEMS_RENDER));
			WRITE_HOOK_BLOCK_PROCESS(render_hud, GET_FUNC_PTR(RENDER_WINDOW_CALL_HOOK_INTERFACE_DRAW_SCREEN));
			WRITE_HOOK_BLOCK_PROCESS(render_ui, GET_FUNC_PTR(RENDER_WINDOW_CALL_HOOK_RENDER_UI_WIDGETS));
			WRITE_HOOK_BLOCK_PROCESS(render_cursor, GET_FUNC_PTR(CALL_HOOK_RENDER_UI_CURSOR));
			WRITE_HOOK_BLOCK_PROCESS(update_ui_widgets, GET_FUNC_VPTR(CALL_HOOK_UPDATE_UI_WIDGETS));
#endif
		}

		void c_function_interface_system::Dispose()
		{
		}
	};
};