/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Common/Precompile.hpp"
#include "Memory/FunctionInterface.hpp"

#include "Rasterizer/GBuffer.hpp"
#include "Rasterizer/PostProcessing/PostProcessing.hpp"

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
#endif
		}
	};
};