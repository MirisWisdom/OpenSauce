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
		enum weapon_state : byte_enum
		{
			_weapon_state_idle,
			_weapon_state_fire1,
			_weapon_state_fire2,
			_weapon_state_chamber1,
			_weapon_state_chamber2,
			_weapon_state_reload1,
			_weapon_state_reload2,
			_weapon_state_charged1,
			_weapon_state_charged2,
			_weapon_state_ready,
			_weapon_state_put_away,

			_weapon_state
		};

		enum weapon_trigger_state : byte_enum
		{
			_weapon_trigger_state_idle,
			_weapon_trigger_state_fire1,
			_weapon_trigger_state_fire2,
			_weapon_trigger_state_unknown3,
			_weapon_trigger_state_unknown4,
			_weapon_trigger_state_unknown5,
			_weapon_trigger_state_unknown6,
			_weapon_trigger_state_unknown7,
			_weapon_trigger_state_unknown8,

			_weapon_trigger_state,
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

			TStructSubGetPtrImpl(Enums::weapon_state,	WeaponState, 0x238);
			TStructSubGetPtrImpl(real,					Heat, 0x23C);
			TStructSubGetPtrImpl(real,					Age, 0x240);
			TStructSubGetPtrImpl(real,					IntegratedLightPower, 0x248);

			struct s_trigger_state
			{
				PAD8; // ?
				Enums::weapon_trigger_state state;
				int16 time;
				UNKNOWN_TYPE(long_flags);
				UNKNOWN_TYPE(int16);
				UNKNOWN_TYPE(int16);
				UNKNOWN_TYPE(int16);
				UNKNOWN_TYPE(int16);
				UNKNOWN_TYPE(real);
				UNKNOWN_TYPE(real);

				PAD(0, 0x10); // I haven't researched the rest of the structure
			}; BOOST_STATIC_ASSERT( sizeof(s_trigger_state) == 0x28 );
			TStructSubGetPtrImpl(s_trigger_state,		Triggers, 0x260); // [2]

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

			TStructSubGetPtrImpl(datum_index,			ReadyEffect, 0x2CC); // effects datum

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