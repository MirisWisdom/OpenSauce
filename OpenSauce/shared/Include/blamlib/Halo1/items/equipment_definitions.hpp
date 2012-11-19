/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/items/item_definitions.hpp>

#include <blamlib/Halo1/tag_files/tag_groups.hpp>

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

		enum equipment_yelo_type : _enum
		{
			_equipment_yelo_type_none,

			_equipment_yelo_type_armor_ability,
			_equipment_yelo_type_deployable,

			_equipment_yelo_type,
		};

		enum equipment_yelo_function_mode : _enum
		{
			_equipment_yelo_function_mode_none,

			_equipment_yelo_function_mode,
		};
	};

	namespace Flags
	{
		enum equipment_yelo_flags : word_flags
		{
		};
	};

	namespace TagGroups
	{
		struct s_equipment_yelo_armor_ability
		{
			struct s_tank {
				TAG_FIELD(real, size);
				TAG_FIELD(real, recharge_rate);
				TAG_FIELD(int16, count);
				TAG_FIELD(int16, swap_time);
			}energy_tank;

			struct s_energy {
				TAG_FIELD(real, use_rate);
				TAG_FIELD(real, recharge_rate);
				TAG_FIELD(real, recharge_delay);
			}energy;
		};
		struct s_equipment_yelo_deployable
		{
			TAG_FIELD(int16, usage_count);
			PAD16;
		};

		struct s_equipment_yelo_definition
		{
			enum { k_max_definition_size = sizeof(tag_reference)*9 }; // size of the useless padding in equipment

			TAG_FIELD(Flags::equipment_yelo_flags, flags);
			PAD16;
			PAD32; PAD32; PAD32;

			struct s_effects {
				TAG_FIELD(tag_reference, activate, 'effe', 'snd!');
				TAG_FIELD(tag_reference, active, 'effe', 'snd!');
				TAG_FIELD(tag_reference, deactivate, 'effe', 'snd!');
			}effects;

		}; BOOST_STATIC_ASSERT( sizeof(s_equipment_yelo_definition) <= s_equipment_yelo_definition::k_max_definition_size );
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