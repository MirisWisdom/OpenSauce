/*
    Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition
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
	ENGINE_PTR(D3DC9, Direct3DCreate9,						DATA_PTR_NULL, 0x1116734, 0xE38454);

	ENGINE_PTR(D3DPRESENT_PARAMETERS, Params,				DATA_PTR_NULL, DATA_PTR_NULL, 0xF7D6E0);
	ENGINE_DPTR(IDirect3D9, D3D,							DATA_PTR_NULL, DATA_PTR_NULL, 0xDCC270);
	ENGINE_DPTR(IDirect3DDevice9, Device,					DATA_PTR_NULL, DATA_PTR_NULL, 0xDCC26C);


//////////////////////////////////////////////////////////////////////////
// GBuffer.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_RASTERIZER_DX9_GBUFFER

#else
	#error Undefined engine layout include for: __EL_INCLUDE_RASTERIZER_DX9
#endif