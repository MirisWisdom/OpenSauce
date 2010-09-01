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
#include "Common/Precompile.hpp"
#include "Rasterizer/RenderTargetChain.hpp"
#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace DX9
	{
		bool		s_render_target_chain::IsAvailable() const
		{
			return	m_targets[0].texture && 
					m_targets[0].surface && 
					m_targets[1].texture && 
					m_targets[1].surface;
		}

		void		s_render_target_chain::AllocateResources(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters, D3DFORMAT TargetFormat)
		{
			ZeroMemory( m_targets, sizeof( m_targets ) );

			for( int32 t = 0; t < 2; ++t )
			{
				pDevice->CreateTexture(pParameters->BackBufferWidth,
					pParameters->BackBufferHeight,
					1,
					D3DUSAGE_RENDERTARGET,
					TargetFormat,
					D3DPOOL_DEFAULT,
					&m_targets[t].texture,
					NULL );
				m_targets[t].texture->GetSurfaceLevel(0, &m_targets[t].surface);
			}
		}

		void		s_render_target_chain::ReleaseResources()
		{
			for( int32 t = 0; t < 2; ++t )
			{
				Yelo::safe_release(m_targets[t].surface);
				Yelo::safe_release(m_targets[t].texture);
			}
		}

		void		s_render_target_chain::Flip()
		{
			m_next = 1 - m_next;
		};
	};
};
#endif