/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/TextBlock.hpp"

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	TextBlock::TextBlock(IDirect3DDevice9* pDevice,D3DPRESENT_PARAMETERS* pPP)
	{
		this->pDevice = pDevice;
		block.screen_width = pPP->BackBufferWidth;
		block.screen_height = pPP->BackBufferHeight;

		pDevice->CreateVertexBuffer(sizeof(COLOR_VERTEX) * 4, NULL,D3DFVF_XYZRHW | D3DFVF_DIFFUSE,D3DPOOL_MANAGED,&vertexBuffer,NULL);
	}

	void TextBlock::SetFade(bool fade)	{ block.fade = fade; }

	void TextBlock::SetFont(cstring face, int32 size, uint32 weight, bool italic, uint32 quality)
	{
		D3DXCreateFont( pDevice,					// D3D device
							 size,					// Height
							 0,                     // Width
							 weight,				// Weight
							 1,                     // MipLevels, 0 = autogen mipmaps
							 italic,                // Italic
							 DEFAULT_CHARSET,       // CharSet
							 OUT_DEFAULT_PRECIS,    // OutputPrecision
							 quality,				// Quality
							 DEFAULT_PITCH | FF_DONTCARE, // PitchAndFamily
							 face,					// pFaceName
							 &pFont);				// ppFont
	}

	void TextBlock::SetDimensions(int32 width, int32 height)
	{
		block.width = width;
		block.height = height;
	}

	void TextBlock::SetTextAlign(uint32 format)	{ block.text_format = format; }

	void TextBlock::SetBackColor(uint32 color)	{ block.back_color = color; }

	void TextBlock::SetTextColor(uint32 color)	{ block.text_color = color; }
	
	void TextBlock::SetPadding(int32 padding)	{ block.padding = padding; }

	void TextBlock::Attach(Enums::attach_method attach,
		real x_percent_offset, real y_percent_offset,
		int32 x_pixel_offset, int32 y_pixel_offset)
	{
		block.attach = attach;
		block.x_percent_offset = x_percent_offset;
		block.y_percent_offset = y_percent_offset;
		block.x_pixel_offset = x_pixel_offset;
		block.y_pixel_offset = y_pixel_offset;
	}

	void TextBlock::SetText(wcstring text)	{ block.text = text; }

	int32 TextBlock::GetWidth()  const	{ return block.width; }
	int32 TextBlock::GetHeight()  const	{ return block.height; }

	void TextBlock::Refresh()
	{
		SetRect(&block.rect, block.width, 0, 0, 0);
		block.height = pFont->DrawTextW(NULL, block.text, NONE, &block.rect, DT_CALCRECT | block.text_format, 0);

		// add padding for the actual block around the text
		block.height += block.padding*2;

		int MAXX = block.screen_width;
		int MAXY = block.screen_height;

		// calculate vertices
		switch (block.attach)
		{
		case Enums::_attach_method_top_right:
			block.rect.left =		MAXX-block.width;
			block.rect.top =		0;
			block.rect.right =		MAXX;
			block.rect.bottom =		block.height;
			break;

		case Enums::_attach_method_top_left:
			block.rect.left =		0;
			block.rect.top =		0;
			block.rect.right =		block.width;
			block.rect.bottom =		block.height;
			break;

		case Enums::_attach_method_top_center:
			block.rect.left =		MAXX/2 - block.width/2;
			block.rect.top =		0;
			block.rect.right =		MAXX/2 + block.width/2;
			block.rect.bottom =		block.height;
			break;

		case Enums::_attach_method_bottom_right:
			block.rect.left =		MAXX-block.width;
			block.rect.top =		MAXY-block.height;
			block.rect.right =		MAXX;
			block.rect.bottom =		MAXY;
			break;

		case Enums::_attach_method_bottom_left:
			block.rect.left =		0;
			block.rect.top =		MAXY-block.height;
			block.rect.right =		block.width;
			block.rect.bottom =		MAXY;
			break;

		case Enums::_attach_method_bottom_center:
			block.rect.left =		MAXX/2 - block.width/2;
			block.rect.top =		MAXY-block.height;
			block.rect.right =		MAXX/2 + block.width/2;
			block.rect.bottom =		MAXY;
			break;

		case Enums::_attach_method_left_center:
			block.rect.left =		0;
			block.rect.top =		MAXY/2 - block.height/2;
			block.rect.right =		block.width;
			block.rect.bottom =		MAXY/2 + block.height/2;
			break;

		case Enums::_attach_method_right_center:
			block.rect.left =		MAXX-block.width;
			block.rect.top =		MAXY/2 - block.height/2;
			block.rect.right =		MAXX;
			block.rect.bottom =		MAXY/2 + block.height/2;
			break;

		case Enums::_attach_method_center:
			block.rect.left =		MAXX/2 - block.width/2;
			block.rect.top =		MAXY/2 - block.height/2;
			block.rect.right =		MAXX/2 + block.width/2;
			block.rect.bottom =		MAXY/2 + block.height/2;
			break;
		}

		// shift the block
		block.rect.left +=	CAST(LONG, block.x_percent_offset/100.0f*MAXX + block.x_pixel_offset);
		block.rect.top +=	CAST(LONG, block.y_percent_offset/100.0f*MAXY + block.y_pixel_offset);
		block.rect.right +=	CAST(LONG, block.x_percent_offset/100.0f*MAXX + block.x_pixel_offset);
		block.rect.bottom +=CAST(LONG, block.y_percent_offset/100.0f*MAXY + block.y_pixel_offset);

		// convert block to raw vertices
		vertexBuffer->Lock(0, 0, CAST_PTR(void**, &vertices), NULL);

		NEW_COLOR_VERTEX(vertices[0], block.rect.left, block.rect.top, 0, 1.0f, block.back_color);
		NEW_COLOR_VERTEX(vertices[1], block.rect.right, block.rect.top, 0, 1.0f, block.back_color);

		uint32 color = block.fade ? 0 : block.back_color;
		NEW_COLOR_VERTEX(vertices[2], block.rect.right, block.rect.bottom, 0, 1.0f, color);
		NEW_COLOR_VERTEX(vertices[3], block.rect.left, block.rect.bottom, 0, 1.0f, color);

		vertexBuffer->Unlock();
	}

	void TextBlock::Render()
	{
		// set appropriate render states
		pDevice->SetVertexShader(NULL);
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// draw block
		pDevice->SetTexture(0,NULL);
		pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		pDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(COLOR_VERTEX));
		pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

		// add padding for the text
		RECT text_rect;
		SetRect(&text_rect,
			block.rect.left + block.padding,
			block.rect.top + block.padding,
			block.rect.right - block.padding,
			block.rect.bottom - block.padding);

		// draw text
		pFont->DrawTextW(NULL, block.text, NONE, &text_rect, block.text_format, block.text_color);
	}

	void TextBlock::OnResetDevice(D3DPRESENT_PARAMETERS *pPP)
	{
		if (pFont) pFont->OnResetDevice();
		block.screen_width = pPP->BackBufferWidth;
		block.screen_height = pPP->BackBufferHeight;
	}

	void TextBlock::OnLostDevice()
	{
		if (pFont) pFont->OnLostDevice();
	}

	void TextBlock::Release()
	{
		Yelo::safe_release(pFont);
		Yelo::safe_release(vertexBuffer);
	}


	void TextBlock::ApplyScheme()
	{
		this->SetFont("Lucida Sans Unicode", 18, FW_NORMAL, false, 6);
		this->SetTextColor(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
		this->SetBackColor(D3DXCOLOR(.2f,.2f,.2f,0.6f));
		this->SetPadding(7);
		this->SetFade(false);
	}
};
#endif