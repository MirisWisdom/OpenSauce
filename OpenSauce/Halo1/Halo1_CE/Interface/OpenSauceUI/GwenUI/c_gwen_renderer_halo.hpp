/*
	GWEN
	Copyright (c) 2012 Facepunch Studios
	See license in Gwen.h
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <Gwen/Gwen.h>
#include <Gwen/BaseRender.h>

#include "Rasterizer/Textures/c_packed_texture_loader.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI
	{
		/// <summary>	A gwen renderer for the halo runtime. </summary>
		class c_gwen_renderer_halo final
			: public Gwen::Renderer::Base
		{
			Rasterizer::Textures::c_packed_texture_loader m_texture_loader;

			public:
				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Constructor. </summary>
				///
				/// <param name="device">	 	[in] The current render device. </param>
				/// <param name="ui_package">	The ui package. </param>
				c_gwen_renderer_halo(IDirect3DDevice9* device, c_packed_file& ui_package);

				/// <summary>	Destructor. </summary>
				~c_gwen_renderer_halo();

				/// <summary>	Begins the render pass. </summary>
				void Begin();

				/// <summary>	Ends the render pass. </summary>
				void End();

				/// <summary>	Releases the renderer. </summary>
				void Release();

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Sets the draw color. </summary>
				///
				/// <param name="color">	The color. </param>
				void SetDrawColor( Gwen::Color color );

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Draw a filled rectangle. </summary>
				///
				/// <param name="rect">	The rectangle to draw. </param>
				void DrawFilledRect( Gwen::Rect rect );

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Loads a font. </summary>
				///
				/// <param name="pFont">	[in] The font to load. </param>
				void LoadFont( Gwen::Font* pFont );

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Frees a font. </summary>
				///
				/// <param name="pFont">	[in] The font to free. </param>
				void FreeFont( Gwen::Font* pFont );

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Renders a string. </summary>
				///
				/// <param name="pFont">	[in] The font to use. </param>
				/// <param name="pos">  	The position to render at. </param>
				/// <param name="text"> 	The text to draw. </param>
				void RenderText( Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString & text );

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Measures the size of a text string when rendered. </summary>
				///
				/// <param name="pFont">	[in] The font to use. </param>
				/// <param name="text"> 	The text to measure. </param>
				///
				/// <returns>	The rendered size. </returns>
				Gwen::Point MeasureText( Gwen::Font* pFont, const Gwen::UnicodeString & text );

				/// <summary>	Sets up the clipping bounds. </summary>
				void StartClip();

				/// <summary>	Removes the clipping bounds. </summary>
				void EndClip();

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Draws a textured rectangle. </summary>
				///
				/// <param name="pTexture">   	[in] The texture to use. </param>
				/// <param name="pTargetRect">	The target area. </param>
				/// <param name="u1">		  	(Optional) the top left U texture coordinate. </param>
				/// <param name="v1">		  	(Optional) the top left V texture coordinate. </param>
				/// <param name="u2">		  	(Optional) the bottom right U texture coordinate. </param>
				/// <param name="v2">		  	(Optional) the bottom right V texture coordinate. </param>
				void DrawTexturedRect( Gwen::Texture* pTexture, Gwen::Rect pTargetRect, float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f );

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Loads a texture. </summary>
				///
				/// <param name="pTexture">	[in] The texture to load. </param>
				void LoadTexture( Gwen::Texture* pTexture );

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Frees a texture. </summary>
				///
				/// <param name="pTexture">	[int] The texture to free. </param>
				void FreeTexture( Gwen::Texture* pTexture );

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Gets the color of a pixel from a texture. </summary>
				///
				/// <param name="pTexture">   	[in] The texture to sample. </param>
				/// <param name="x">		  	The x texture coordinate. </param>
				/// <param name="y">		  	The y texture coordinate. </param>
				/// <param name="col_default">	The default color. </param>
				///
				/// <returns>	The pixel's color. </returns>
				Gwen::Color PixelColour( Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color & col_default );

			protected:

				void*				m_pCurrentTexture;
				IDirect3DDevice9*	m_pDevice;
				DWORD				m_Color;
				Gwen::Font::List	m_FontList;
				//Gwen::Texture::List	m_TextureList;

				/// <summary>	Draws any remaining faces. </summary>
				void Flush();

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Adds a vertex to draw. </summary>
				///
				/// <param name="x">	The x coordinate. </param>
				/// <param name="y">	The y coordinate. </param>
				void AddVert( int x, int y );

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Adds a vertex to draw with texture coordinates. </summary>
				///
				/// <param name="x">	The x coordinate. </param>
				/// <param name="y">	The y coordinate. </param>
				/// <param name="u">	The u texture coordinate. </param>
				/// <param name="v">	The v texture coordinate. </param>
				void AddVert( int x, int y, float u, float v );

			protected:

				struct VertexFormat
				{
					FLOAT x, y, z, rhw;
					DWORD color;
					float u, v;
				};

				static const int		MaxVerts = 1024;
				VertexFormat			m_pVerts[MaxVerts];
				int						m_iVertNum;

		};
	};};};
};
#endif
