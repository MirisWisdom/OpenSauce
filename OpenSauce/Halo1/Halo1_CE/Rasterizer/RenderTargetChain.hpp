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
#include "Rasterizer/Rasterizer.hpp"

namespace Yelo
{
	namespace DX9
	{
		/////////////////////////////////////////////////////////////////////
		// Used to prevent reading and writing to a single render target by
		// flipping between two on each pass.
		/////////////////////////////////////////////////////////////////////
		struct s_render_target_chain
		{
			bool m_first_render;
			PAD24;
			// 2 render targets + halos primary buffer gives us the 3 that we need
			Rasterizer::s_render_target m_targets[2];
			// pointers to the current setup of targets
			struct{
				// scene holds the scene as it was before an effect is started
				Rasterizer::s_render_target* scene;
				// current holds the target currently being rendered to
				Rasterizer::s_render_target* current;
				// next hold the next (also last) target rendered to
				Rasterizer::s_render_target* next;
			}m_target_setup;

			// returns true if both of the extra render targets are ok to use
			bool				IsAvailable() const;
			// create the render targets
			void				AllocateResources(IDirect3DDevice9* device, 
				uint32 width, 
				uint32 height);
			// release the render targets
			void				ReleaseResources();
			// resets the target setup to the default of halos primary buffer as the scene texture
			void				ResetTargets();
			// swaps the pointers for current and next
			void				Flip();
			// swaps the pointers for scene and next
			void				SetSceneToLast();


			IDirect3DSurface9*  GetSceneSurface()	const	{ return m_target_setup.scene->surface; }			
			IDirect3DTexture9*  GetSceneTexture()	const	{ return m_target_setup.scene->texture; }
			IDirect3DSurface9*  GetCurrentSurface()	const	{ return m_target_setup.current->surface; }			
			IDirect3DTexture9*  GetCurrentTexture()	const	{ return m_target_setup.current->texture; }
			IDirect3DSurface9*  GetNextSurface()	const	{ return m_target_setup.next->surface; }
			IDirect3DTexture9*  GetNextTexture()	const	{ return m_target_setup.next->texture; }
		};
	};
};
#endif