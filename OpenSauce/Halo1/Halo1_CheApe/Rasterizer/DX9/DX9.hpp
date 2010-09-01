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
#pragma once

#ifndef PLATFORM_NO_DX9
namespace Yelo
{
	namespace Rasterizer { namespace DX9 {
		void Initialize();
		void Dispose();

		// D3D Present Properties
		D3DPRESENT_PARAMETERS* D3DPresetParams();

		// D3D Properties
		IDirect3D9* Direct3D9();

		// D3D Device Properties
		IDirect3DDevice9* Direct3DDevice();
	}; };
};
#endif