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

DEFINE_HOOK_PROCESS(func_gbuffer_render_sky_preprocess, DX9::c_gbuffer_system::render_progress::SkyPreProcess);
DEFINE_HOOK_PROCESS(func_gbuffer_render_sky_postprocess, DX9::c_gbuffer_system::render_progress::SkyPostProcess);

DEFINE_HOOK_BLOCK_PREPROCESS(render_sky) = {
	GET_HOOK_PROCESS(func_gbuffer_render_sky_preprocess).FunctionHook,
};
DEFINE_HOOK_BLOCK_POSTPROCESS(render_sky) = {
	GET_HOOK_PROCESS(func_gbuffer_render_sky_postprocess).FunctionHook,
};


DEFINE_HOOK_PROCESS(func_gbuffer_render_objects_preprocess, DX9::c_gbuffer_system::render_progress::ObjectsPreProcess);
DEFINE_HOOK_PROCESS(func_gbuffer_render_objects_postprocess, DX9::c_gbuffer_system::render_progress::ObjectsPostProcess);	

DEFINE_HOOK_BLOCK_PREPROCESS(render_objects) = {
	GET_HOOK_PROCESS(func_gbuffer_render_objects_preprocess).FunctionHook,
};
DEFINE_HOOK_BLOCK_POSTPROCESS(render_objects) = {
	GET_HOOK_PROCESS(func_gbuffer_render_objects_postprocess).FunctionHook,
};


DEFINE_HOOK_PROCESS(func_gbuffer_render_structure_preprocess, DX9::c_gbuffer_system::render_progress::StructurePreProcess);
DEFINE_HOOK_PROCESS(func_gbuffer_render_structure_postprocess, DX9::c_gbuffer_system::render_progress::StructurePostProcess);

DEFINE_HOOK_BLOCK_PREPROCESS(render_structure) = {
	GET_HOOK_PROCESS(func_gbuffer_render_structure_preprocess).FunctionHook,
};
DEFINE_HOOK_BLOCK_POSTPROCESS(render_structure) = {
	GET_HOOK_PROCESS(func_gbuffer_render_structure_postprocess).FunctionHook,
};


DEFINE_HOOK_PROCESS(func_postprocessing_weather_particle_systems_render_preprocess, Postprocessing::RenderPreAlphaBlended);	

DEFINE_HOOK_BLOCK_PREPROCESS(weather_particle_systems_render) = {
	GET_HOOK_PROCESS(func_postprocessing_weather_particle_systems_render_preprocess).FunctionHook,
};


DEFINE_HOOK_PROCESS(func_gbuffer_render_hud_preprocess, DX9::c_gbuffer_system::render_progress::UIPreProcess);
DEFINE_HOOK_PROCESS(func_postprocessing_render_hud_preprocess, Postprocessing::RenderPreHUD);
DEFINE_HOOK_PROCESS(func_postprocessing_render_hud_postprocess, Postprocessing::RenderPreMenu);	

DEFINE_HOOK_BLOCK_PREPROCESS(render_hud) = {
	GET_HOOK_PROCESS(func_postprocessing_render_hud_preprocess).FunctionHook,
	GET_HOOK_PROCESS(func_gbuffer_render_hud_preprocess).FunctionHook,
};
DEFINE_HOOK_BLOCK_POSTPROCESS(render_hud) = {
	GET_HOOK_PROCESS(func_postprocessing_render_hud_postprocess).FunctionHook,
};


DEFINE_HOOK_PROCESS(func_postprocessing_render_ui_postprocess, Postprocessing::RenderPostMenu);	

DEFINE_HOOK_BLOCK_POSTPROCESS(render_ui) = {
	GET_HOOK_PROCESS(func_postprocessing_render_ui_postprocess).FunctionHook,
};


DEFINE_HOOK_BLOCK_PROCESS(render_sky, GET_FUNC_VPTR(RENDER_SKY), render_sky, render_sky);
DEFINE_HOOK_BLOCK_PROCESS(render_objects, GET_FUNC_VPTR(RENDER_OBJECTS), render_objects, render_objects);
DEFINE_HOOK_BLOCK_PROCESS(render_structure, GET_FUNC_VPTR(STRUCTURE_RENDER_LIGHTMAPS), render_structure, render_structure);
DEFINE_HOOK_BLOCK_PROCESS_PRE(weather_particle_systems_render, GET_FUNC_VPTR(WEATHER_PARTICLE_SYSTEMS_RENDER), weather_particle_systems_render);
DEFINE_HOOK_BLOCK_PROCESS(render_hud, GET_FUNC_VPTR(INTERFACE_DRAW_SCREEN), render_hud, render_hud);
DEFINE_HOOK_BLOCK_PROCESS_POST(render_ui, GET_FUNC_VPTR(RENDER_UI_WIDGETS), render_ui);

bool& FunctionProcessRenderHudIsDisabled() { return GET_HOOK_BLOCK_PROCESS(render_hud).g_is_disabled; }