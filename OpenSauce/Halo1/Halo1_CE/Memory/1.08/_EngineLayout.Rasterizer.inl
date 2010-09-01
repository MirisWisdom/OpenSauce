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

	FUNC_PTR(RENDER_WINDOW_END_HOOK,			0x51BD68, FUNC_PTR_NULL);

#else
	#error Undefined engine layout include for: __EL_INCLUDE_RASTERIZER
#endif