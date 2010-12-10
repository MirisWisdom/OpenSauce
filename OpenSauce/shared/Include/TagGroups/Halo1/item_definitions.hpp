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
		enum global_grenade_type
		{
			_global_grenade_type_frag,
			_global_grenade_type_plasma,

			_global_grenade_type
		};

		enum equipment_powerup
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
		struct _item_definition
		{
			struct _item_definition_flags {
				TAG_FLAG(maintains_z_up);
				TAG_FLAG(destroyed_by_explosions);
				TAG_FLAG(unaffected_by_gravity);
			}flags; BOOST_STATIC_ASSERT( sizeof(_item_definition_flags) == sizeof(long_flags) );

			int16 message_index;
			int16 sort_order;
			real scale;
			int16 hud_message_value_scale;
			PAD16;

			TAG_PAD(int32, 4);
			_enum function_exports[4];
			TAG_PAD(int32, 41);

			struct {
				TAG_FIELD(tag_reference, material_effects, "foot");
				TAG_FIELD(tag_reference, collision_sound, "snd!");
			}references;

			TAG_PAD(int32, 30);

			struct {
				real_bounds Delay;
				TAG_FIELD(tag_reference, detonating_effect, "effe");
				TAG_FIELD(tag_reference, effect, "effe");
			}detonation;
		}; BOOST_STATIC_ASSERT( sizeof(_item_definition) == 0x18C );


		struct s_item_definition : s_object_definition
		{
			TagGroups::_item_definition item;
		};
	};
};