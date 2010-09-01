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

#include "Objects/ObjectDefinitions.hpp"

namespace Yelo
{
	namespace Enums
	{
		enum equipment_powerup
		{
			_equipment_powerup_none,
			_equipment_powerup_double_shield,
			_equipment_powerup_over_shield,
			_equipment_powerup_active_camo,
			_equipment_powerup_full_spectrum_vision,
			_equipment_powerup_health,
			_equipment_powerup_grenade,
			_equipment_powerup,
		};
	};

	namespace Objects
	{
		struct s_item_data : TStructImpl(Enums::k_object_size_item - Enums::k_object_size_object)
		{
			enum { DATA_OFFSET = Enums::k_object_size_object, };
		};

		struct s_weapon_data : TStructImpl(Enums::k_object_size_weapon - Enums::k_object_size_item)
		{
			enum { DATA_OFFSET = Enums::k_object_size_item, };

			TStructSubGetPtrImpl(real,					Heat, 0x23C);
			TStructSubGetPtrImpl(real,					Age, 0x240);
			TStructSubGetPtrImpl(real,					IntegratedLightPower, 0x248);

			// 0x260 - triggers[2], sizeof(0x20?)

			struct s_magazine_state // '?' means IDK if its actually padding or there are values there. If there are, IDK their types (could be a boolean!)
			{
				// 1 = reloading
				// 2 = ?
				// 3 = ?
				_enum state;
				PAD16; // ?
				PAD16; // ?
				int16 rounds_unloaded;
				int16 rounds_loaded;
				PAD16; // ?
				UNKNOWN_TYPE(int16); // I just know a WORD is here, may be an _enum
				PAD16; // ?
			}; BOOST_STATIC_ASSERT( sizeof(s_magazine_state) == 0x10 );
			TStructSubGetPtrImpl(s_magazine_state,		Magazines, 0x2B0); // [2]

			TStructSubGetPtrImpl(bool,					BaselineValid, 0x2E0);
			TStructSubGetPtrImpl(byte,					BaselineIndex, 0x2E1);
			TStructSubGetPtrImpl(byte,					MessageIndex, 0x2E2);

			// 0x2E4 baseline data structure
		};

		struct s_equipment_data : TStructImpl(Enums::k_object_size_equipment - Enums::k_object_size_item)
		{
			enum { DATA_OFFSET = Enums::k_object_size_item, };
		};




		struct s_item_datum
		{
			s_object_data object;
			s_item_data item;
		}; BOOST_STATIC_ASSERT( sizeof(s_item_datum) == Enums::k_object_size_item );

		struct s_weapon_datum : s_item_datum
		{
			s_weapon_data weapon;
		}; BOOST_STATIC_ASSERT( sizeof(s_weapon_datum) == Enums::k_object_size_weapon );

		struct s_equipment_datum : s_item_datum
		{
			s_equipment_data equipment;
		}; BOOST_STATIC_ASSERT( sizeof(s_equipment_datum) == Enums::k_object_size_equipment );
	};
};