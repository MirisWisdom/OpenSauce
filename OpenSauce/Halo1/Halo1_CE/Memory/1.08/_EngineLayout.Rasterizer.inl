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
// Rasterizer.cpp
#if __EL_INCLUDE_FILE_ID == __EL_RASTERIZER_RASTERIZER
	ENGINE_PTR(s_render_globals, render_globals,					0x75E020, PTR_NULL);
	ENGINE_PTR(s_render_target, global_render_targets,				0x638A20, PTR_NULL);
	ENGINE_PTR(s_rasterizer_debug_options, rasterizer_debug_data,	0x6249C0, PTR_NULL);
	ENGINE_PTR(s_rasterizer_frame_inputs, rasterizer_frame_inputs,	0x75C130, PTR_NULL);

	FUNC_PTR(RENDER_WINDOW_END_HOOK,								0x51BD68, FUNC_PTR_NULL);

	FUNC_PTR(RASTERIZER_DISPOSE,									0x51C110, FUNC_PTR_NULL);
	FUNC_PTR(RASTERIZER_DISPOSE_CALL_FROM_RASTERIZER,				0x51B0E0, FUNC_PTR_NULL);
	FUNC_PTR(RASTERIZER_DISPOSE_CALL_FROM_SHELL,					0x5447D9, FUNC_PTR_NULL);
	

	ENGINE_PTR(DWORD, RASTERIZER_DEVICE_ADAPTER_INDEX				0x6B8350, PTR_NULL);
	ENGINE_PTR(char, RESOLUTION_LIST_COUNT,							0x4BE35C, PTR_NULL);
	ENGINE_PTR(void**, RESOLUTION_LIST_STRING_NULL_REFERENCE,		0x4BE39B, PTR_NULL);

	FUNC_PTR(RESOLUTION_LIST_SETUP_RESOLUTIONS_CALL,				0x4BE2BF, FUNC_PTR_NULL);

	static void** K_RESOLUTION_LIST_X_REFERENCES[] = {
		CAST_PTR(void**, 0x4BE2AA), CAST_PTR(void**, 0x4BE2D2), CAST_PTR(void**, 0x4BE37C),
		CAST_PTR(void**, 0x4BE54C), CAST_PTR(void**, 0x4BE937)
	};
	static void** K_RESOLUTION_LIST_Y_REFERENCES[] = {
		CAST_PTR(void**, 0x4BE33A), CAST_PTR(void**, 0x4BE382), CAST_PTR(void**, 0x4BE40C),
		CAST_PTR(void**, 0x4BE4CE), CAST_PTR(void**, 0x4BE94B)
	};
	static void** K_RESOLUTION_LIST_STRING_REFERENCES[] = {
		CAST_PTR(void**, 0x4BE376), CAST_PTR(void**, 0x4BEBFC)
	};

	static void** K_RESOLUTION_LIST_REFRESH_COUNT_REFERENCES[] = {
		CAST_PTR(void**, 0x4BE38D), CAST_PTR(void**, 0x4BE3B1), CAST_PTR(void**, 0x4BE3D4),
		CAST_PTR(void**, 0x4BE3F5), CAST_PTR(void**, 0x4BE506), CAST_PTR(void**, 0x4BE55E),
		CAST_PTR(void**, 0x4BE97B), CAST_PTR(void**, 0x4BEC28), CAST_PTR(void**, 0x4BEC2E)
	};
	static void** K_RESOLUTION_LIST_REFRESH_RATE_REFERENCES[] = {
		CAST_PTR(void**, 0x4BE2EB), CAST_PTR(void**, 0x4BE3A3), CAST_PTR(void**, 0x4BE3CE),
		CAST_PTR(void**, 0x4BE401), CAST_PTR(void**, 0x4BE516), CAST_PTR(void**, 0x4BE98C),
		CAST_PTR(void**, 0x4BEC6A),
	};
#else
	#error Undefined engine layout include for: __EL_INCLUDE_RASTERIZER
#endif