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
#include "Rasterizer/PostProcessing/PostProcessingRenderBlock.hpp"
#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Postprocessing
	{
		void		c_postprocess_render_block::AddEffect(c_postprocess_effect* effect)
		{
			// Adds a post process effect to this render blocks list
			if(m_effect_list_head == NULL)
				m_effect_list_head = new s_effect_node(effect);
			else
			{
				s_effect_node* curr_node = m_effect_list_head;
				while (curr_node->m_next)
					curr_node = curr_node->m_next;
				curr_node->m_next = new s_effect_node(effect);
			}
			m_effect_count++;
		}
		void		c_postprocess_render_block::ClearList()
		{
			// deleting the head node will recursively delete all child nodes
			delete m_effect_list_head;
			m_effect_list_head = NULL;

			m_effect_count = 0;
		}
		bool		c_postprocess_render_block::RenderEffects(IDirect3DDevice9* pDevice, double frame_time)
		{
			bool effects_applied = false;
			// Apply the effects in this render block in order
			s_effect_node* curr_node = m_effect_list_head;
			while (curr_node)
			{
				if(curr_node->m_effect)
					effects_applied |= SUCCEEDED(curr_node->m_effect->DoPostProcessEffect(pDevice, frame_time));
				curr_node = curr_node->m_next;
			}
			return effects_applied;
		}
	};
};
#endif