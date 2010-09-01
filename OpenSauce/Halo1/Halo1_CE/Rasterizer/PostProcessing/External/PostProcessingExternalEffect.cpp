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
#include "Rasterizer/PostProcessing/External/PostProcessingExternalEffect.hpp"
#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/External/PostProcessingSubsystem_External.hpp"

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem { namespace External {

		/////////////////////////////////////////////////////////////////////
		// c_external_effect
		bool		c_external_effect::ValidateEffect()
		{
			c_generic_effect_base::ValidateEffect();

			//if we have no shader indices then this effect is invalid
			if(m_effect_external->shader_indices.Count < 1)
				m_effect_external->runtime.flags.valid_effect_bit = false;

			return m_effect_external->runtime.flags.valid_effect_bit;
		}
		void		c_external_effect::SetSource(void* pSource)
		{
			m_effect_external = CAST_PTR(s_shader_postprocess_external_effect*, pSource);

			c_generic_effect_base::SetSource(pSource);
		}		
		void		c_external_effect::AllocateCustomResources(IDirect3DDevice9* pDevice)
		{
			//loop through the shader indices, adding them to our render list
			for(int32 i = 0; i < m_effect_external->shader_indices.Count; ++i)
			{
				// if the index is out of range return
				if(m_effect_external->shader_indices[i].shader_index >= Globals().m_shader_count)
					return;

				c_generic_shader_instance_node* shader_node = new c_generic_shader_instance_node();
				shader_node->Ctor();
				shader_node->SetShaderSource(Globals().m_shader_array[m_effect_external->shader_indices[i].shader_index]);

				AddProcess(shader_node);
			}	
		}
		/////////////////////////////////////////////////////////////////////

	};};};
};
#endif