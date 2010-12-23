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

//////////////////////////////////////////////////////////////////////////
// DX9.cpp
#if __EL_INCLUDE_FILE_ID == __EL_RASTERIZER_DX9_DX9
	ENGINE_PTR(D3DPRESENT_PARAMETERS, Params,				0x75B780, PTR_NULL);
	ENGINE_DPTR(IDirect3D9, D3D,							0x6B8430, PTR_NULL);
	ENGINE_DPTR(IDirect3DDevice9, Device,					0x6B842C, PTR_NULL);
	ENGINE_DPTR(IDirectInput8, DInput8,						0x64C54C, PTR_NULL);
	ENGINE_DPTR(IDirectInputDevice8, DInput8DeviceKeyboard,	0x64C750, PTR_NULL);
	ENGINE_DPTR(IDirectInputDevice8, DInput8DeviceMouse,	0x64C754, PTR_NULL);
	ENGINE_DPTR(IDirectInputDevice8, DInput8DeviceJoysticks,0x64C798, PTR_NULL);


//////////////////////////////////////////////////////////////////////////
// GBuffer.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_RASTERIZER_DX9_GBUFFER
ENGINE_PTR(DX9::s_vsf_entry*, VSF_TABLE_START_REFERENCE,	0x533C12, FUNC_PTR_NULL);

	FUNC_PTR(RENDER_WINDOW_CALL,				0x50FA4B, FUNC_PTR_NULL);
	FUNC_PTR(RENDER_WINDOW,						0x50FC50, FUNC_PTR_NULL);
	FUNC_PTR(RENDER_WINDOW_CALL_RETN,			0x50FA50, FUNC_PTR_NULL);

	FUNC_PTR(RENDER_OBJECT_LIST_HOOK,			0x512AE2, FUNC_PTR_NULL);
	FUNC_PTR(RENDER_OBJECT_LIST_HOOK_RETN,		0x512AEA, FUNC_PTR_NULL);

	FUNC_PTR(RENDER_OBJECT_LIST_END_HOOK,		0x512DE1, FUNC_PTR_NULL);

	FUNC_PTR(RENDER_OBJECT_OBJECT_LOD_HOOK,		0x4DA666, FUNC_PTR_NULL);
	FUNC_PTR(RENDER_OBJECT_OBJECT_LOD_HOOK_RETN,0x4DA66C, FUNC_PTR_NULL);

	FUNC_PTR(FIRST_PERSON_WEAPON_DRAW_HOOK,		0x4954D3, FUNC_PTR_NULL);
	FUNC_PTR(FIRST_PERSON_WEAPON_DRAW_HOOK_RETN,0x4954D8, FUNC_PTR_NULL);

	FUNC_PTR(COMMAND_CAMERA_SET_HOOK,			0x445778, FUNC_PTR_NULL);
	FUNC_PTR(COMMAND_CAMERA_SET_HOOK_RETN,		0x44577D, FUNC_PTR_NULL);

	FUNC_PTR(COMMAND_SWITCH_BSP_HOOK,			0x54261B, FUNC_PTR_NULL);
	FUNC_PTR(COMMAND_SWITCH_BSP_HOOK_RETN,		0x542622, FUNC_PTR_NULL);

	FUNC_PTR(COMMAND_GAME_SAVE_HOOK,			0x482907, FUNC_PTR_NULL);
	FUNC_PTR(COMMAND_GAME_SAVE_HOOK_RETN,		0x48290D, FUNC_PTR_NULL);

	FUNC_PTR(RASTERIZER_DRAW_STATIC_TRIANGLES_STATIC_VERTICES__DRAW_INDEXED_PRIMITIVE_HOOK,
												0x5201D6, FUNC_PTR_NULL);
	FUNC_PTR(RASTERIZER_DRAW_DYNAMIC_TRIANGLES_STATIC_VERTICES2__DRAW_INDEXED_PRIMITIVE_HOOK,
												0x51FF58, FUNC_PTR_NULL);
	FUNC_PTR(RASTERIZER_SET_WORLD_VIEW_PROJECTION_MATRIX_VERTEX_CONSTANT_HOOK,
												0x51CCD3, FUNC_PTR_NULL);

#else
	#error Undefined engine layout include for: __EL_INCLUDE_RASTERIZER_DX9
#endif