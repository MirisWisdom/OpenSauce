--	Yelo: Open Sauce SDK
--		Halo 1 (CE) Edition

--	See license\OpenSauce\Halo1_CE for specific license information

HaloCEClient_Rasterizer_DX9 =
{
--////////////////////////////////////////////////////////////////////////
-- DX9.cpp
	["K_PARAMS"] =                                                                          0x75B780,
	["K_CAPS"] =                                                                            0x75C0C0,
	["K_D3D"] =                                                                             0x6B8430,
	["K_DEVICE"] =                                                                          0x6B842C,
	["K_DINPUT8"] =                                                                         0x64C54C,
	["K_DINPUT8DEVICEKEYBOARD"] =                                                           0x64C750,
	["K_DINPUT8DEVICEMOUSE"] =                                                              0x64C754,
	["K_DINPUT8DEVICEJOYSTICKS"] =                                                          0x64C798,
	
--////////////////////////////////////////////////////////////////////////
-- GBuffer.cpp
	["K_VSF_TABLE_START_REFERENCE"] =                                                       0x533C12,

	["K_RENDER_OBJECT_LIST_HOOK"] =                                                         0x512AE2,
	["K_RENDER_OBJECT_LIST_HOOK_RETN"] =                                                    0x512AEA,

	["K_RENDER_OBJECT_LIST_END_HOOK"] =                                                     0x512DE1,

	["K_RENDER_OBJECT_OBJECT_LOD_HOOK"] =                                                   0x4DA666,
	["K_RENDER_OBJECT_OBJECT_LOD_HOOK_RETN"] =                                              0x4DA66C,

	["K_FIRST_PERSON_WEAPON_DRAW_HOOK"] =                                                   0x4954D3,
	["K_FIRST_PERSON_WEAPON_DRAW_HOOK_RETN"] =                                              0x4954D8,

	["K_COMMAND_CAMERA_SET_HOOK"] =                                                         0x445778,
	["K_COMMAND_CAMERA_SET_HOOK_RETN"] =                                                    0x44577D,

	["K_COMMAND_SWITCH_BSP_HOOK"] =                                                         0x54261B,
	["K_COMMAND_SWITCH_BSP_HOOK_RETN"] =                                                    0x542622,

	["K_COMMAND_GAME_SAVE_HOOK"] =                                                          0x482907,
	["K_COMMAND_GAME_SAVE_HOOK_RETN"] =                                                     0x48290D,

	["K_RENDER_OBJECTS_TRANSPARENT"] =                                                      0x519170,
	["K_RENDER_WINDOW_CALL_RENDER_OBJECTS_TRANSPARENT_HOOK"] =                              0x510194,
	["K_RENDER_WINDOW_CALL_RENDER_OBJECTS_TRANSPARENT_RETN"] =                              0x510199,

	["K_RASTERIZER_DRAW_STATIC_TRIANGLES_STATIC_VERTICES__DRAW_INDEXED_PRIMITIVE_HOOK"] =   0x5201D6,
	["K_RASTERIZER_DRAW_DYNAMIC_TRIANGLES_STATIC_VERTICES2__DRAW_INDEXED_PRIMITIVE_HOOK"] = 0x51FF58,
												
--////////////////////////////////////////////////////////////////////////
-- DX9/rasterizer_dx9_shaders_vshader9.cpp
	["K_RASTERIZER_DX9_EFFECT_COLLECTION"] =                                                0x75B740,
	["K_RASTERIZER_EFFECT_SHADERS"] =                                                       0x638AD8,
	["K_VSF_TABLE"] =                                                                       0x639258,
	
--////////////////////////////////////////////////////////////////////////
-- DeviceHooks.cpp
	["K_RASTERIZER_D3D_CREATE_DEVICE_HOOK"] =                                               0x51ADD2,
	["K_RASTERIZER_D3D_CREATE_DEVICE_RETN"] =                                               0x51ADD7,
	["K_RASTERIZER_D3D_RESET_DEVICE_HOOK"] =                                                0x519B7F,
	["K_RASTERIZER_D3D_BEGIN_SCENE_CALL"] =                                                 0x51B277,
	["K_RASTERIZER_D3D_END_SCENE_CALL"] =                                                   0x51BDA0,

	["K_RASTERIZER_SET_WORLD_VIEW_PROJECTION_MATRIX_VERTEX_CONSTANT_CALL"] =                0x51CCD3,
	["K_RASTERIZER_SET_MODEL_TEX_SCALE_VERTEX_CONSTANT_CALL"] =                             0x52E2D3,
	
	["K_RASTERIZER_SET_MODEL_SPEC_COLOR_VERTEX_CONSTANT_CALL_0"] =                          0x52E2ED,
	["K_RASTERIZER_SET_MODEL_SPEC_COLOR_VERTEX_CONSTANT_CALL_1"] =                          0x52BB4D,
		
	["K_RASTERIZER_SET_MODEL_VERTEX_LIGHT_VERTEX_CONSTANT_CALL"] =                          0x51CA6E,
}

HaloCEClient_Rasterizer_DX9_Index =
{
	[1] = "K_PARAMS",
	[2] = "K_CAPS",
	[3] = "K_D3D",
	[4] = "K_DEVICE",
	[5] = "K_DINPUT8",
	[6] = "K_DINPUT8DEVICEKEYBOARD",
	[7] = "K_DINPUT8DEVICEMOUSE",
	[8] = "K_DINPUT8DEVICEJOYSTICKS",
	[9] = "K_VSF_TABLE_START_REFERENCE",
	[10] = "K_RENDER_OBJECT_LIST_HOOK",
	[11] = "K_RENDER_OBJECT_LIST_HOOK_RETN",
	[12] = "K_RENDER_OBJECT_LIST_END_HOOK",
	[13] = "K_RENDER_OBJECT_OBJECT_LOD_HOOK",
	[14] = "K_RENDER_OBJECT_OBJECT_LOD_HOOK_RETN",
	[15] = "K_FIRST_PERSON_WEAPON_DRAW_HOOK",
	[16] = "K_FIRST_PERSON_WEAPON_DRAW_HOOK_RETN",
	[17] = "K_COMMAND_CAMERA_SET_HOOK",
	[18] = "K_COMMAND_CAMERA_SET_HOOK_RETN",
	[19] = "K_COMMAND_SWITCH_BSP_HOOK",
	[20] = "K_COMMAND_SWITCH_BSP_HOOK_RETN",
	[21] = "K_COMMAND_GAME_SAVE_HOOK",
	[22] = "K_COMMAND_GAME_SAVE_HOOK_RETN",
	[23] = "K_RENDER_OBJECTS_TRANSPARENT",
	[24] = "K_RENDER_WINDOW_CALL_RENDER_OBJECTS_TRANSPARENT_HOOK",
	[25] = "K_RENDER_WINDOW_CALL_RENDER_OBJECTS_TRANSPARENT_RETN",
	[26] = "K_RASTERIZER_DRAW_STATIC_TRIANGLES_STATIC_VERTICES__DRAW_INDEXED_PRIMITIVE_HOOK",
	[27] = "K_RASTERIZER_DRAW_DYNAMIC_TRIANGLES_STATIC_VERTICES2__DRAW_INDEXED_PRIMITIVE_HOOK",
	[28] = "K_RASTERIZER_DX9_EFFECT_COLLECTION",
	[29] = "K_RASTERIZER_EFFECT_SHADERS",
	[30] = "K_VSF_TABLE",
	[31] = "K_RASTERIZER_D3D_CREATE_DEVICE_HOOK",
	[32] = "K_RASTERIZER_D3D_CREATE_DEVICE_RETN",
	[33] = "K_RASTERIZER_D3D_RESET_DEVICE_HOOK",
	[34] = "K_RASTERIZER_D3D_BEGIN_SCENE_CALL",
	[35] = "K_RASTERIZER_D3D_END_SCENE_CALL",
	[36] = "K_RASTERIZER_SET_WORLD_VIEW_PROJECTION_MATRIX_VERTEX_CONSTANT_CALL",
	[37] = "K_RASTERIZER_SET_MODEL_TEX_SCALE_VERTEX_CONSTANT_CALL",
	[38] = "K_RASTERIZER_SET_MODEL_SPEC_COLOR_VERTEX_CONSTANT_CALL_0",
	[39] = "K_RASTERIZER_SET_MODEL_SPEC_COLOR_VERTEX_CONSTANT_CALL_1",
	[40] = "K_RASTERIZER_SET_MODEL_VERTEX_LIGHT_VERTEX_CONSTANT_CALL",
}