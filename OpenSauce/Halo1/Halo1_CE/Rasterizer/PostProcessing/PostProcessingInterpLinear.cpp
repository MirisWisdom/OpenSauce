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
#include "Common/Precompile.hpp"
#include "Rasterizer/Postprocessing/PostProcessingInterpLinear.hpp"

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Postprocessing
	{
		void	c_postprocess_interp_linear::Update(real delta_time)
		{
			// increase the interpolation value at the required rate
			if(m_interpolation.change_time == 0)
				m_interpolation.current_interpolation = 1.0f;
			else
				m_interpolation.current_interpolation += CAST(real, (1.0f / m_interpolation.change_time) * delta_time);

			if(m_interpolation.current_interpolation > 1.0f)
				m_interpolation.current_interpolation = 1.0f;
			
			// if the interpolation value is 1.0f simply set the current value to the target value
			if(m_interpolation.current_interpolation == 1.0f)
				m_values.current_value = m_values.values[1];
			else
				InterpolateValues();
		}

		void	c_postprocess_interp_linear::Begin(real interp_time)
		{
			// set the change time and reset the interpolation to zero
			m_interpolation.change_time = interp_time;
			m_interpolation.current_interpolation = 0.0f;

			if(m_interpolation.change_time == 0.0f)
				m_values.current_value = m_values.values[1];
		}

		void	c_postprocess_interp_linear::Reset()
		{
			m_values.current_value = m_values.values[1] = m_values.values[0];
			m_interpolation.change_time = 0.0f;
			m_interpolation.current_interpolation = 0.0f;
		}

		void	c_postprocess_interp_linear::InterpolateValues()
		{
			switch(m_values.base_type)
			{
			case Enums::_shader_variable_base_type_boolean:
				m_values.current_value.boolean = (m_interpolation.current_interpolation > 0.5 ? m_values.values[1].boolean : m_values.values[0].boolean);
				break;
			case Enums::_shader_variable_base_type_integer:
				{
					int range = m_values.values[1].integer32 - m_values.values[0].integer32;
					m_values.current_value.integer32 = m_values.values[0].integer32 + (int)(range * m_interpolation.current_interpolation);
				}
				break;
			case Enums::_shader_variable_base_type_float:
				for(int i = 0; i < m_values.base_count; i++)
				{
					real range = m_values.values[1].reals[i] - m_values.values[0].reals[i];
					m_values.current_value.reals[i] = m_values.values[0].reals[i] + (range * m_interpolation.current_interpolation);
				}
				break;
			case Enums::_shader_variable_base_type_argb_color:
				for(int i = 0; i < 4; i++)
				{
					real range = m_values.values[1].reals[i] - m_values.values[0].reals[i];
					m_values.current_value.reals[i] = m_values.values[0].reals[i] + (range * m_interpolation.current_interpolation);
				}
				break;
			}
		}
	};
};
#endif