/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/GwenUI/c_gwen_renderer_halo.hpp"

#if !PLATFORM_IS_DEDI

#include <Gwen/Utility.h>
#include <Gwen/Font.h>
#include <Gwen/Texture.h>
#include <Gwen/WindowProvider.h>

#include "Rasterizer/Textures/c_packed_texture_loader.hpp"

#define D3DFVF_VERTEXFORMAT2D ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

struct FontData
{
	ID3DXFont*	pFont;
	int			iSpaceWidth;
};

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI
	{
		c_gwen_renderer_halo::c_gwen_renderer_halo(IDirect3DDevice9* pDevice, c_packed_file& ui_package)
			: m_texture_loader(ui_package)
		{
			m_pDevice = pDevice;
			m_iVertNum = 0;

			// Reset the vertex values
			for ( int i = 0; i < MaxVerts; i++ )
			{
				m_pVerts[ i ].z = 0.5f;
				m_pVerts[ i ].rhw = 1.0f;
			}
		}

		c_gwen_renderer_halo::~c_gwen_renderer_halo()
		{
		}

		void c_gwen_renderer_halo::Begin()
		{
			// Set the device state for rendering the UI
			m_pCurrentTexture = nullptr;
			m_pDevice->SetTexture( 0, NULL );

			m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
			m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
			m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
			m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
			m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
			m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );
			m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
			m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
			m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		D3DTOP_MODULATE );
			m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE );
			m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2,	D3DTA_CURRENT );
			m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE );
			m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE );
			m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2,	D3DTA_CURRENT );
			m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
			m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
		}

		void c_gwen_renderer_halo::End()
		{
			// Draw any remaining faces and remove the UI texture from the device
			Flush();
			m_pDevice->SetTexture( 0, 0 );
			m_pCurrentTexture = NULL;
		}

		void c_gwen_renderer_halo::Flush()
		{
			// Draw any remaining triangles
			if ( m_iVertNum > 0 )
			{
				m_pDevice->SetFVF( D3DFVF_VERTEXFORMAT2D );
				m_pDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, m_iVertNum / 3, &m_pVerts[0], sizeof( VertexFormat ) );
				m_iVertNum = 0;
			}
		}

		void c_gwen_renderer_halo::AddVert( int x, int y )
		{
			// If the max vertex count is reached, draw the current batch of triangles
			if ( m_iVertNum >= MaxVerts - 1 )
			{
				Flush();
			}

			// Add the vertex to the list
			m_pVerts[ m_iVertNum ].x = ( float ) x;
			m_pVerts[ m_iVertNum ].y = ( float ) y;
			m_pVerts[ m_iVertNum ].color = m_Color;
			m_iVertNum++;
		}

		void c_gwen_renderer_halo::AddVert( int x, int y, float u, float v )
		{
			// If the max vertex count is reached, draw the current batch of triangles
			if ( m_iVertNum >= MaxVerts - 1 )
			{
				Flush();
			}
			
			// Add the textured vertex to the list
			m_pVerts[ m_iVertNum ].x = -0.5f + ( float ) x;
			m_pVerts[ m_iVertNum ].y = -0.5f + ( float ) y;
			m_pVerts[ m_iVertNum ].u = u;
			m_pVerts[ m_iVertNum ].v = v;
			m_pVerts[ m_iVertNum ].color = m_Color;
			m_iVertNum++;
		}

		void c_gwen_renderer_halo::DrawFilledRect( Gwen::Rect rect )
		{
			// If a texture is in use, draw the triangles that use it then remove it from the device
			if ( m_pCurrentTexture != NULL )
			{
				Flush();
				m_pDevice->SetTexture( 0, NULL );
				m_pCurrentTexture = NULL;
			}

			// Add vertices for the rectangle to the vertex list after applying the current UI scale amount
			Translate( rect );
			AddVert( rect.x, rect.y );
			AddVert( rect.x + rect.w, rect.y );
			AddVert( rect.x, rect.y + rect.h );
			AddVert( rect.x + rect.w, rect.y );
			AddVert( rect.x + rect.w, rect.y + rect.h );
			AddVert( rect.x, rect.y + rect.h );
		}

		void c_gwen_renderer_halo::SetDrawColor( Gwen::Color color )
		{
			m_Color = D3DCOLOR_ARGB( color.a, color.r, color.g, color.b );
		}

		void c_gwen_renderer_halo::LoadFont( Gwen::Font* font )
		{
			m_FontList.push_back( font );
			// Scale the font according to canvas
			font->realsize = font->size * Scale();
			D3DXFONT_DESC fd;
			memset( &fd, 0, sizeof( fd ) );
			wcstombs( fd.FaceName, font->facename.c_str(), LF_FACESIZE );
			fd.Width = 0;
			fd.MipLevels = 1;
			fd.CharSet = DEFAULT_CHARSET;
			fd.Height = font->realsize * -1.0f;
			fd.OutputPrecision = OUT_DEFAULT_PRECIS;
			fd.Italic = 0;
			fd.Weight = font->bold ? FW_BOLD : FW_NORMAL;
#ifdef CLEARTYPE_QUALITY
			fd.Quality = font->realsize < 14 ? DEFAULT_QUALITY : CLEARTYPE_QUALITY;
#else
			fd.Quality = PROOF_QUALITY;
#endif
			fd.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
			LPD3DXFONT pD3DXFont;
			HRESULT hr = D3DXCreateFontIndirect( m_pDevice, &fd, &pD3DXFont );
			FontData*	pFontData = new FontData();
			pFontData->pFont = pD3DXFont;
			// ID3DXFont doesn't measure trailing spaces, so we measure the width of a space here and store it
			// in the font data - then we can add it to the width when we measure text with trailing spaces.
			{
				RECT rctA = {0, 0, 0, 0};
				pFontData->pFont->DrawTextW( NULL, L"A", -1, &rctA, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE, 0 );
				RECT rctSpc = {0, 0, 0, 0};
				pFontData->pFont->DrawTextW( NULL, L"A A", -1, &rctSpc, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE, 0 );
				pFontData->iSpaceWidth = rctSpc.right - rctA.right * 2;
			}
			font->data = pFontData;
		}

		void c_gwen_renderer_halo::FreeFont( Gwen::Font* pFont )
		{
			m_FontList.remove( pFont );

			// If the fonts DX resource has not been released, release it
			if ( !pFont->data ) { return; }

			FontData* pFontData = ( FontData* ) pFont->data;

			if ( pFontData->pFont )
			{
				pFontData->pFont->Release();
				pFontData->pFont = NULL;
			}

			delete pFontData;
			pFont->data = NULL;
		}

		void c_gwen_renderer_halo::RenderText( Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString & text )
		{
			Flush();

			// If the font doesn't exist, or the font size should be changed
			if ( !pFont->data || fabs( pFont->realsize - pFont->size * Scale() ) > 2 )
			{
				FreeFont( pFont );
				LoadFont( pFont );
			}

			FontData* pFontData = ( FontData* ) pFont->data;
			Translate( pos.x, pos.y );
			RECT ClipRect = { pos.x, pos.y, 0, 0 };
			pFontData->pFont->DrawTextW( NULL, text.c_str(), -1, &ClipRect, DT_LEFT | DT_TOP | DT_NOCLIP | DT_SINGLELINE, m_Color );
		}

		Gwen::Point c_gwen_renderer_halo::MeasureText( Gwen::Font* pFont, const Gwen::UnicodeString & text )
		{
			// If the font doesn't exist, or the font size should be changed
			if ( !pFont->data || fabs( pFont->realsize - pFont->size * Scale() ) > 2 )
			{
				FreeFont( pFont );
				LoadFont( pFont );
			}

			FontData* pFontData = ( FontData* ) pFont->data;
			Gwen::Point size;

			if ( text.empty() )
			{
				RECT rct = {0, 0, 0, 0};
				pFontData->pFont->DrawTextW( NULL, L"W", -1, &rct, DT_CALCRECT, 0 );
				return Gwen::Point( 0, rct.bottom );
			}

			RECT rct = {0, 0, 0, 0};
			pFontData->pFont->DrawTextW( NULL, text.c_str(), -1, &rct, DT_CALCRECT | DT_LEFT | DT_TOP | DT_SINGLELINE, 0 );

			for ( int i = text.length() - 1; i >= 0 && text[i] == L' '; i-- )
			{
				rct.right += pFontData->iSpaceWidth;
			}

			return Gwen::Point( rct.right / Scale(), rct.bottom / Scale() );
		}

		void c_gwen_renderer_halo::StartClip()
		{
			Flush();
			m_pDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, TRUE );
			const Gwen::Rect & rect = ClipRegion();
			RECT r;
			r.left = ceil( ( ( float ) rect.x ) * Scale() );
			r.right = ceil( ( ( float )( rect.x + rect.w ) ) * Scale() );
			r.top = ceil( ( float ) rect.y * Scale() );
			r.bottom = ceil( ( ( float )( rect.y + rect.h ) ) * Scale() );
			m_pDevice->SetScissorRect( &r );
		}

		void c_gwen_renderer_halo::EndClip()
		{
			Flush();
			m_pDevice->SetRenderState( D3DRS_SCISSORTESTENABLE, FALSE );
		}

		void c_gwen_renderer_halo::DrawTexturedRect( Gwen::Texture* pTexture, Gwen::Rect rect, float u1, float v1, float u2, float v2 )
		{
			IDirect3DTexture9* pImage = ( IDirect3DTexture9* ) pTexture->data;

			// Missing image, not loaded properly?
			if ( !pImage )
			{
				return DrawMissingImage( rect );
			}

			Translate( rect );

			if ( m_pCurrentTexture != pImage )
			{
				Flush();
				m_pDevice->SetTexture( 0, pImage );
				m_pCurrentTexture = pImage;
			}

			AddVert( rect.x, rect.y,			u1, v1 );
			AddVert( rect.x + rect.w, rect.y,		u2, v1 );
			AddVert( rect.x, rect.y + rect.h,	u1, v2 );
			AddVert( rect.x + rect.w, rect.y,		u2, v1 );
			AddVert( rect.x + rect.w, rect.y + rect.h, u2, v2 );
			AddVert( rect.x, rect.y + rect.h, u1, v2 );
		}

		void c_gwen_renderer_halo::LoadTexture( Gwen::Texture* pTexture )
		{
			auto& texture_name_wstring = pTexture->name.GetUnicode();
			std::string texture_name(texture_name_wstring.begin(), texture_name_wstring.end());

			IDirect3DTexture9* texture = nullptr;
			D3DXIMAGE_INFO image_info;
			if (!m_texture_loader.LoadTexture(texture_name, &texture, &image_info))
			{
				return;
			}

			pTexture->data = texture;
			pTexture->width = image_info.Width;
			pTexture->height = image_info.Height;
		}

		void c_gwen_renderer_halo::FreeTexture( Gwen::Texture* pTexture )
		{
			IDirect3DTexture9* pImage = ( IDirect3DTexture9* ) pTexture->data;

			if ( !pImage ) { return; }

			pImage->Release();
			pTexture->data = NULL;
			return;
		}

		Gwen::Color c_gwen_renderer_halo::PixelColour( Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color & col_default )
		{
			IDirect3DTexture9* pImage = ( IDirect3DTexture9* ) pTexture->data;

			if ( !pImage ) { return col_default; }

			IDirect3DSurface9* pSurface = NULL;

			if ( pImage->GetSurfaceLevel( 0, &pSurface ) != S_OK ) { return col_default; }

			if ( !pSurface ) { return col_default; }

			D3DLOCKED_RECT lockedRect;
			pSurface->LockRect( &lockedRect, NULL, D3DLOCK_READONLY );
			DWORD* pixels = ( DWORD* ) lockedRect.pBits;
			D3DXCOLOR color = pixels[lockedRect.Pitch / sizeof( DWORD ) * y + x];
			pSurface->UnlockRect();
			pSurface->Release();
			return Gwen::Color( color.r * 255, color.g * 255, color.b * 255, color.a * 255 );
		}

		void c_gwen_renderer_halo::Release()
		{
			Gwen::Font::List::iterator it = m_FontList.begin();

			while ( it != m_FontList.end() )
			{
				FreeFont( *it );
				it = m_FontList.begin();
			}
		}
	};};};
}
#endif