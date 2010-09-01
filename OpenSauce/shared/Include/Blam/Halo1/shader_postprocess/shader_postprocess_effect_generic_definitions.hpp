/*
    Yelo: Open Sauce SDK

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

#include <Blam/Halo1/shader_postprocess_definitions.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct s_shader_postprocess_effect_generic : s_shader_postprocess_effect_definition
		{
			enum { k_group_tag = 'speg' };
			PAD16;
			PAD16;

			struct _effect_activation_flags {
				TAG_FLAG16(not_initially_active);
				TAG_FLAG16(not_active_during_cutscenes);
				TAG_FLAG16(disabled_when_zoomed);
				TAG_FLAG16(disabled_when_using_a_vehicle);
			}effect_activation_flags;	BOOST_STATIC_ASSERT( sizeof(_effect_activation_flags) == sizeof(word_flags) );
			PAD16;

		};BOOST_STATIC_ASSERT( sizeof(s_shader_postprocess_effect_generic) == 0x8 + sizeof(s_shader_postprocess_effect_definition));
	};
};