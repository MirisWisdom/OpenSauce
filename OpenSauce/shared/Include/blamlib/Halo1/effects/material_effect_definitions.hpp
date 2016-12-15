/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum material_type : _enum
		{
			_material_type_none = NONE,

			_material_type_dirt = 0,
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

			k_number_of_material_types
		};
	};

	namespace TagGroups
	{
	};
};