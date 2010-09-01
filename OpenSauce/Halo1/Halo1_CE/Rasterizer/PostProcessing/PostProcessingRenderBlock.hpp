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
		Component: Post-processing
		Original Author: FireScythe
		Integration: kornman00
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Rasterizer/DX9/DX9.hpp"
#include "Rasterizer/PostProcessing/PostProcessing.hpp"
#include "Rasterizer/PostProcessing/PostProcessingEffect.hpp"

namespace Yelo
{
	namespace Postprocessing
	{
		/////////////////////////////////////////////////////////////////////
		// Convenience struct to more easily know what effects to render
		// without using loads of ifs
		/////////////////////////////////////////////////////////////////////
		struct c_postprocess_render_block
		{
		protected:
			struct s_effect_node
			{
				c_postprocess_effect*	m_effect;
				s_effect_node*			m_next;
				s_effect_node(c_postprocess_effect* pEffect) : m_effect(pEffect), m_next(NULL) {}

				~s_effect_node()
				{
					delete m_next;
					m_next = NULL;
				}
			};
			s_effect_node*			m_effect_list_head;	// Linked list of all effects this render point uses
			uint32					m_effect_count;

		public:
			// Add an effect pointer to the list and increment [m_effect_count]
			void AddEffect(c_postprocess_effect* effect);
			// Deletes all [m_effect_list_head nodes
			void ClearList();
			// Iterate through [m_effect_list_head rendering each effect in order
			void RenderEffects(IDirect3DDevice9* pDevice, double frame_time);
		};
	};
};
#endif