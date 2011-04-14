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

/*
	User Add-on
		Component: Post-processing
		Original Author: FireScythe
		Integration: kornman00
*/
#pragma once
#include <Common/Halo1/YeloSharedSettings.hpp>

namespace Yelo
{
	namespace Postprocessing { namespace Debug {
		/// Writes a line to the console
		void Write(cstring string);
		/// Writes a formatted line to the console
		void WriteLine(cstring format, ...);
		/// Writes a number of lines in a D3DX error to the console
		void WriteD3DXErrors(LPD3DXBUFFER pBuffer, int32 error_count);
	}; };
};