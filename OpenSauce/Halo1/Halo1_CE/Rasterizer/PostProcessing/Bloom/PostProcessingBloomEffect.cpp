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
#include "Rasterizer/PostProcessing/Bloom/PostProcessingBloomEffect.hpp"
#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem { namespace Bloom {	
		/////////////////////////////////////////////////////////////////////
		// c_bloom_effect
		void		c_bloom_effect::SetSource(void* pSource)
		{
			m_effect_bloom = CAST_PTR(s_shader_postprocess_bloom_effect*, pSource);

			c_postprocess_effect::SetSource(pSource);
		}
		HRESULT		c_bloom_effect::SetupEffect()
		{				
			c_postprocess_effect::SetupEffect();

			m_effect_bloom->runtime.flags.is_active_bit = true;
			m_effect_bloom->runtime.flags.valid_effect_bit = false;

			return S_OK;
		}
		/////////////////////////////////////////////////////////////////////
	}; }; };
};
#endif