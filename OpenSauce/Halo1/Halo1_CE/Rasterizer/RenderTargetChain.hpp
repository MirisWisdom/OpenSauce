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
		Component: Gfx Post-processing
		Original Author: FireScythe
		Integration: kornman00
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Rasterizer/DX9/DX9.hpp"

namespace Yelo
{
	namespace DX9
	{
		/////////////////////////////////////////////////////////////////////
		// Used to prevent reading and writing to a single render target by
		// flipping between two on each pass. Currently only used for the
		// main scene textures, but could be used on depth/normals/etc.
		/////////////////////////////////////////////////////////////////////
		struct s_render_target_chain
		{
			int32				m_next;			// The next render target to use.
			bool				m_first_render;	// If true shaders should use the original scene as their previous source
			PAD24;								// as no render target has been written to yet.
			struct {
				IDirect3DTexture9* texture;		// Render target textures. Used in shaders.
				IDirect3DSurface9* surface;		// Render target surfaces. Used as device render targets.
			}m_targets[2];

		public:

			bool				IsAvailable() const;
			// Create the textures and get the surfaces.
			void				AllocateResources(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters, D3DFORMAT TargetFormat);
			// Release textures and surfaces.
			void				ReleaseResources();
			// Toggle between surface/texture 1 and 2
			void                Flip();
			// Get the previously written to surface.
			IDirect3DSurface9*  GetPrevTarget()	const	{ return m_targets[1 - m_next].surface; }			
			// Get the previously written to texture.
			IDirect3DTexture9*  GetPrevSource()	const	{ return m_targets[m_next].texture; }
			// Get the next surface to write to.
			IDirect3DSurface9*  GetNextTarget()	const	{ return m_targets[m_next].surface; }
			// Get the next texture being written to.
			IDirect3DTexture9*  GetNextSource() const	{ return m_targets[1 - m_next].texture; }
		};
	};
};
#endif