/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/items/item_definitions.hpp>

#include <YeloLib/Halo1/items/equipment_definitions_yelo.hpp>

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum equipment_powerup : _enum
		{
			_equipment_powerup_none,

			_equipment_powerup_double_shield,
			_equipment_powerup_over_shield,
			_equipment_powerup_active_camo,
			_equipment_powerup_full_spectrum_vision,

			_equipment_powerup_health,
			_equipment_powerup_grenade,

			_equipment_powerup
		};
	};

	namespace TagGroups
	{
		struct _equipment_definition
		{
			TAG_ENUM(powerup_type, Enums::equipment_powerup);
			TAG_ENUM(grenade_type, Enums::grenade_type);
			TAG_FIELD(real, powerup_time);
			TAG_FIELD(tag_reference, pickup_sound, 'snd!');
			union {
				TAG_PAD(byte, s_equipment_yelo_definition::k_max_definition_size);
				//s_equipment_yelo_definition yelo;
			};
		}; BOOST_STATIC_ASSERT( sizeof(_equipment_definition) == 0xA8 );
		struct s_equipment_definition : s_item_definition
		{
			enum { k_group_tag = 'eqip' };

			_equipment_definition equipment;
		};
	};
};