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
#if PLATFORM_ID == PLATFORM_TOOL

namespace Yelo
{
	namespace Enums
	{
		enum console_color
		{
			_console_color_black = 0x0,
			_console_color_blue = 0x1,
			_console_color_green = 0x2,
			_console_color_aqua = 0x3,
			_console_color_red = 0x4,
			_console_color_purple = 0x5,
			_console_color_yellow = 0x6,
			_console_color_white = 0x7,
			_console_color_gray = 0x8,
			_console_color_lightblue = 0x9,
			_console_color_lightgreen = 0xA,
			_console_color_lightaqua = 0xB,
			_console_color_lightred = 0xC,
			_console_color_lightpurple = 0xD,
			_console_color_lightyellow = 0xE,
			_console_color_brightwhite = 0xF,

			_console_color
		};
	};
	namespace Tool { namespace Console
	{
		void ColorPrintF(const WORD color,
			const char* format, ...);
		void ColorPrint(const WORD color,
			const char* string, bool new_line = false);
	};};
};
#endif