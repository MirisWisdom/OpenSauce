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
			return m_targets[0].IsEnabled() &&
				m_targets[1].IsEnabled();
		}

		void		s_render_target_chain::AllocateResources(IDirect3DDevice9* device, uint32 width, uint32 height)
		{
			for(int i = 0; i < 2; i++)
				m_targets[i].CreateTarget(device, 
					width, 
					height, 
					Rasterizer::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary].format);
			ResetTargets();
		}

		void		s_render_target_chain::ReleaseResources()
		{
			for(int i = 0; i < 2; i++)
				m_targets[i].ReleaseTarget();
		}

		void		s_render_target_chain::ResetTargets()
		{
			m_target_setup.scene = &Rasterizer::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary];
			m_target_setup.current = &m_targets[0];
			m_target_setup.next = &m_targets[1];
		}

		void		s_render_target_chain::Flip()
		{
			std::swap<Rasterizer::s_render_target*>(m_target_setup.current, m_target_setup.next);
		}

		void		s_render_target_chain::SetSceneToLast()
		{
			std::swap<Rasterizer::s_render_target*>(m_target_setup.next, m_target_setup.scene);
		}
	};
};
#endif