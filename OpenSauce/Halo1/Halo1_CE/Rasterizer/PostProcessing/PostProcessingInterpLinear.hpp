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
#include <Blam/Halo1/shader_postprocess_definitions.hpp>

namespace Yelo
{
	namespace Postprocessing
	{
		typedef union 
		{
			bool				boolean;
			int32				integer32;
			real				real32;
			real_vector2d		vector2d;
			real_vector3d		vector3d;
			real_quaternion		vector4d;
			real_rgb_color		color3d;
			real_argb_color		color4d;
			real				reals[4];
		} t_value_union;

		class c_postprocess_interp_linear
		{
			struct{
				/// The time to interpolate for
				real			change_time;
				/// The current interpolation amount
				real			current_interpolation;
			} m_interpolation;

			struct{
				Enums::shader_variable_base_type	base_type;
				int32								base_count;
				t_value_union						values[2];
				t_value_union						current_value;
			} m_values;

		public:
			inline t_value_union&	GetStart()			{ return m_values.values[0]; }
			inline t_value_union&	GetEnd()			{ return m_values.values[1]; }
			inline t_value_union&	GetCurrent()		{ return m_values.current_value; }
			inline void				SetType(Enums::shader_variable_base_type type, int32 count )
				{ m_values.base_type = type; m_values.base_count = count; }

			void					Update(real delta_time);
			void					Begin(real interp_time);
			void					Reset();

			void					InterpolateValues();
		};
	};
};
#endif