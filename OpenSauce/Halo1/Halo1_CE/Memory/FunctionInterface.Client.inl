/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
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

	
DEFINE_HOOK_PROCESS(func_postprocessing_weather_particle_systems_render_preprocess, Rasterizer::PostProcessing::RenderPreAlphaBlended);	

DEFINE_HOOK_BLOCK_PREPROCESS(weather_particle_systems_render) = {
	GET_HOOK_PROCESS(func_postprocessing_weather_particle_systems_render_preprocess).FunctionHook,
};

DEFINE_HOOK_PROCESS(func_gameui_enable_widget_offset, Hud::EnableWidgetScale);
DEFINE_HOOK_PROCESS(func_gameui_enable_hud_offset, Hud::EnableHUDScale);
DEFINE_HOOK_PROCESS(func_gameui_disable_offset, Hud::DisableScale);

DEFINE_HOOK_PROCESS(func_gbuffer_render_hud_preprocess, DX9::c_gbuffer_system::render_progress::UIPreProcess);
DEFINE_HOOK_PROCESS(func_postprocessing_render_hud_preprocess, Rasterizer::PostProcessing::RenderPreHUD);
DEFINE_HOOK_PROCESS(func_postprocessing_render_hud_postprocess, Rasterizer::PostProcessing::RenderPreMenu);

DEFINE_HOOK_BLOCK_PREPROCESS(render_hud) = {
	GET_HOOK_PROCESS(func_postprocessing_render_hud_preprocess).FunctionHook,
	GET_HOOK_PROCESS(func_gbuffer_render_hud_preprocess).FunctionHook,
	GET_HOOK_PROCESS(func_gameui_enable_hud_offset).FunctionHook,
};
DEFINE_HOOK_BLOCK_POSTPROCESS(render_hud) = {
	GET_HOOK_PROCESS(func_postprocessing_render_hud_postprocess).FunctionHook,
	GET_HOOK_PROCESS(func_gameui_disable_offset).FunctionHook,
};


DEFINE_HOOK_PROCESS(func_postprocessing_render_ui_postprocess, Rasterizer::PostProcessing::RenderPostMenu);	

DEFINE_HOOK_BLOCK_PREPROCESS(render_ui) = {
	GET_HOOK_PROCESS(func_gameui_enable_widget_offset).FunctionHook,
};
DEFINE_HOOK_BLOCK_POSTPROCESS(render_ui) = {
	GET_HOOK_PROCESS(func_postprocessing_render_ui_postprocess).FunctionHook,
	GET_HOOK_PROCESS(func_gameui_disable_offset).FunctionHook,
};


DEFINE_HOOK_BLOCK_PREPROCESS(render_cursor) = {
	GET_HOOK_PROCESS(func_gameui_enable_widget_offset).FunctionHook,
};
DEFINE_HOOK_BLOCK_POSTPROCESS(render_cursor) = {
	GET_HOOK_PROCESS(func_gameui_disable_offset).FunctionHook,
};

DEFINE_HOOK_BLOCK_PROCESS(render_sky, GET_FUNC_VPTR(RENDER_SKY), render_sky, render_sky);
DEFINE_HOOK_BLOCK_PROCESS(render_objects, GET_FUNC_VPTR(RENDER_OBJECTS), render_objects, render_objects);
DEFINE_HOOK_BLOCK_PROCESS(render_structure, GET_FUNC_VPTR(STRUCTURE_RENDER_LIGHTMAPS), render_structure, render_structure);
DEFINE_HOOK_BLOCK_PROCESS_PRE(weather_particle_systems_render, GET_FUNC_VPTR(WEATHER_PARTICLE_SYSTEMS_RENDER), weather_particle_systems_render);
DEFINE_HOOK_BLOCK_PROCESS(render_hud, GET_FUNC_VPTR(INTERFACE_DRAW_SCREEN), render_hud, render_hud);
DEFINE_HOOK_BLOCK_PROCESS(render_ui, GET_FUNC_VPTR(RENDER_UI_WIDGETS), render_ui, render_ui);
DEFINE_HOOK_BLOCK_PROCESS(render_cursor, GET_FUNC_VPTR(RENDER_UI_CURSOR), render_cursor, render_cursor);
DEFINE_HOOK_BLOCK_PROCESS_NONE(update_ui_widgets, GET_FUNC_VPTR(UPDATE_UI_WIDGETS));

bool& FunctionProcessRenderHudIsDisabled() { return GET_HOOK_BLOCK_PROCESS(render_hud).g_is_disabled; }
bool& FunctionProcessUpdateUIWidgetsDisabled() { return GET_HOOK_BLOCK_PROCESS(update_ui_widgets).g_is_disabled; }
bool& FunctionProcessRenderCursorDisabled() { return GET_HOOK_BLOCK_PROCESS(render_cursor).g_is_disabled; }