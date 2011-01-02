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
// ShaderExtension.cpp
#if __EL_INCLUDE_FILE_ID == __EL_RASTERIZER_SHADEREXTENSION_MODEL
	ENGINE_PTR(void*, RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_SETUP__SET_SHADER_ENVIRONMENT_PTR,	0x52EB35, PTR_NULL);
	ENGINE_PTR(void*, RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_SETUP__SET_SHADER_MODEL_PTR,			0x52EB3F, PTR_NULL);

	FUNC_PTR(RASTERIZER_MODEL_DRAW_INVERT_BACKFACE_NORMALS_CHECK_HOOK,		0x52E3C5, FUNC_PTR_NULL);
	FUNC_PTR(RASTERIZER_MODEL_DRAW_INVERT_BACKFACE_NORMALS_CHECK_RETN,		0x52E3CB, FUNC_PTR_NULL);

	FUNC_PTR(RASTERIZER_DX9_SHADERS_EFFECT_SHADERS_INITIALIZE__SPRINTF_CALL,0x533094, PTR_NULL);

	FUNC_PTR(RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER,						0x52B740, FUNC_PTR_NULL);
	FUNC_PTR(RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_MODEL,				0x52D460, FUNC_PTR_NULL);

	static cstring* K_VSH_COLLECTION_PATH_REFERENCES[] = {
		CAST_PTR(cstring*, 0x533AFE), CAST_PTR(cstring*, 0x533B99),
	};

#else
	#error Undefined engine layout include for: __EL_RASTERIZER_SHADEREXTENSION
#endif