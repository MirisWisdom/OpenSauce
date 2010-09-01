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
// YeloSettings.cpp
#if __EL_INCLUDE_FILE_ID == __EL_COMMON_YELO_SETTINGS
	ENGINE_PTR(char, g_profile_path, 0x647768, 0x5B9598); // set by -path in haloce


//////////////////////////////////////////////////////////////////////////
// Memory/FunctionInterface.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_MEMORY_FUNCTION_INTERFACE
	FUNC_PTR(RENDER_SKY,												0x5148C0, FUNC_PTR_NULL);
	FUNC_PTR(RENDER_WINDOW_CALL_HOOK_RENDER_SKY,						0x50FD02, FUNC_PTR_NULL);

	FUNC_PTR(RENDER_OBJECTS,											0x5125A0, FUNC_PTR_NULL);
	FUNC_PTR(RENDER_WINDOW_CALL_HOOK_RENDER_OBJECTS,					0x50FD11, FUNC_PTR_NULL);

	FUNC_PTR(STRUCTURE_RENDER_LIGHTMAPS,								0x556300, FUNC_PTR_NULL);
	FUNC_PTR(RENDER_WINDOW_CALL_HOOK_STRUCTURE_RENDER_LIGHTMAPS,		0x50FD1B, FUNC_PTR_NULL);

	FUNC_PTR(WEATHER_PARTICLE_SYSTEMS_RENDER,							0x459610, FUNC_PTR_NULL);
	FUNC_PTR(RENDER_WINDOW_CALL_HOOK_WEATHER_PARTICLE_SYSTEMS_RENDER,	0x51009C, FUNC_PTR_NULL);

	FUNC_PTR(RENDER_UI_WIDGETS,											0x49B5F0, FUNC_PTR_NULL);
	FUNC_PTR(RENDER_WINDOW_CALL_HOOK_RENDER_UI_WIDGETS,					0x5101CD, FUNC_PTR_NULL);

	FUNC_PTR(RENDER_UI_MOUSE,											0x49A480, FUNC_PTR_NULL);
	FUNC_PTR(CALL_HOOK_RENDER_UI_MOUSE,									0x49B6C0, FUNC_PTR_NULL);

	FUNC_PTR(INTERFACE_DRAW_SCREEN,										0x4976B0, FUNC_PTR_NULL);
	FUNC_PTR(RENDER_WINDOW_CALL_HOOK_INTERFACE_DRAW_SCREEN,				0x5101C1, FUNC_PTR_NULL);

	FUNC_PTR(UPDATE_UI_WIDGETS,											0x49AFB0, FUNC_PTR_NULL);
	FUNC_PTR(CALL_HOOK_UPDATE_UI_WIDGETS,								0x4CB0A7, FUNC_PTR_NULL);

#else
	#error Undefined engine layout include for: __EL_INCLUDE_OPEN_SAUCE
#endif