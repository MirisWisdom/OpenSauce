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
#pragma once

#if !PLATFORM_IS_DEDI
#include "Rasterizer/DX9/DX9.hpp"

namespace Yelo
{
	namespace Enums
	{
		enum attach_method : long_enum
		{
			_attach_method_top_right,
			_attach_method_top_left,
			_attach_method_top_center,
			_attach_method_bottom_right,
			_attach_method_bottom_left,
			_attach_method_bottom_center,
			_attach_method_left_center,
			_attach_method_right_center,
			_attach_method_center,

			_attach_method
		};
	};

	class TextBlock
	{
	private:
		IDirect3DDevice9* pDevice;
		ID3DXFont *pFont;
		IDirect3DVertexBuffer9* vertexBuffer;
		COLOR_VERTEX* vertices;

		struct block_attributes 
		{
			RECT rect;
			int32 width;
			int32 height;
			int32 screen_width;
			int32 screen_height;
			Enums::attach_method attach;
			real x_percent_offset;
			real y_percent_offset;
			int32 x_pixel_offset;
			int32 y_pixel_offset;
			wcstring text;
			uint32 text_format;
			uint32 text_color;
			uint32 back_color;
			int32 padding;
			bool fade; PAD24;
		}block;

	public:
		TextBlock(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPP);

		void SetFade(bool fade);
		void SetFont(cstring face, int32 size, uint32 weight, bool italic, uint32 quality);
		void SetDimensions(int32 width, int32 height);
		void SetTextAlign(uint32 format);
		/*	DT_BOTTOM, DT_CENTER, DT_LEFT, DT_RIGHT, DT_TOP, DT_VCENTER
			DT_EXPANDTABS, DT_WORDBREAK, DT_WORD_ELLIPSIS
		*/
		void SetBackColor(uint32 color);
		void SetTextColor(uint32 color);
		void SetPadding(int32 padding);
		void Attach(Enums::attach_method attach,
			real x_percent_offset, real y_percent_offset,
			int32 x_pixel_offset, int32 y_pixel_offset);	
		void SetText(wcstring text);

		int32 GetWidth() const;
		int32 GetHeight() const;

		void Refresh();
		void Render();

		void OnResetDevice(D3DPRESENT_PARAMETERS *pPP);
		void OnLostDevice();
		void Release();


		void ApplyScheme();
	};
};
#endif