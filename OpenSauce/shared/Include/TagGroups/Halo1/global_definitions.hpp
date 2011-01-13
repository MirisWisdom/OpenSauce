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

#include <TagGroups/Halo1/TagGroupDefinitions.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum predicted_resource : _enum {
			_predicted_resource_bitmap,
			_predicted_resource_sound,

			_predicted_resource
		};

		enum periodic_function : _enum
		{
			_periodic_function_one,
			_periodic_function_zero,
			_periodic_function_cosine,
			_periodic_function_cosine_variable,
			_periodic_function_diagonal_wave,
			_periodic_function_diagonal_wave_variable,
			_periodic_function_slide,
			_periodic_function_slide_variable,
			_periodic_function_noise,
			_periodic_function_jitter,
			_periodic_function_wander,
			_periodic_function_spark,

			_periodic_function,
		};

		enum transition_function : _enum
		{
			_transition_function_linear,
			_transition_function_early,
			_transition_function_very_early,
			_transition_function_late,
			_transition_function_very_late,
			_transition_function_cosine,

			_transition_function,
		};

		enum material_type
		{
			_material_type_dirt,
			_material_type_sand,
			_material_type_stone,
			_material_type_snow,
			_material_type_wood,
			_material_type_metal_hollow,
			_material_type_metal_thin,
			_material_type_metal_thick,
			_material_type_rubber,
			_material_type_glass,
			_material_type_force_field,
			_material_type_grunt,
			_material_type_hunter_armor,
			_material_type_hunter_skin,
			_material_type_elite,
			_material_type_jackel,
			_material_type_jackel_energy_shield,
			_material_type_engineer_skin,
			_material_type_engineer_force_field,
			_material_type_flood_combat_form,
			_material_type_flood_carrier_form,
			_material_type_cyborg_armor,
			_material_type_cyborg_energy_shield,
			_material_type_human_armor,
			_material_type_human_skin,
			_material_type_sentinel,
			_material_type_monitor,
			_material_type_plastic,
			_material_type_water,
			_material_type_leaves,
			_material_type_elite_energy_shield,
			_material_type_ice,
			_material_type_hunter_shield,

			_material_type
		};
		enum input_output_channel : _enum
		{
			_input_output_channel_none,
			_input_output_channel_a,
			_input_output_channel_b,
			_input_output_channel_c,
			_input_output_channel_d,

			_input_output_channel,
		};
	};

	namespace TagGroups
	{
		struct predicted_resource
		{
			_enum type;
			int16 resource_index;
			datum_index tag_index;
		}; BOOST_STATIC_ASSERT( sizeof(predicted_resource) == 0x8 ); // max count: 1024
	};
};