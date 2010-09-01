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
#pragma once

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/PostProcessing.hpp"
#include "Rasterizer/PostProcessing/PostProcessingEffect.hpp"

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem { namespace MotionBlur {

		struct s_shader_postprocess_motionblur_effect : public TagGroups::s_shader_postprocess_effect_definition {};

		class c_motionblur_effect : public c_postprocess_effect
		{
		public:
			s_shader_postprocess_motionblur_effect* m_effect_motionblur;

			HRESULT				SetupEffect();
			void				SetSource(void* pSource);

			void				Ctor()
			{
				c_postprocess_effect::Ctor();

				m_effect_motionblur = NULL;
			}
		};

	}; }; };
};
#endif