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

namespace Yelo
{
//////////////////////////////////////////////////////////////////////////
// Client settings

	namespace Enums
	{
		// Persisted in it's own config file
		enum engine_mod {
			k_engine_mod_config_version = 1,

			k_engine_mod_max_data_file_sets = 16,
		};


		enum vehicle_view {
			k_vehicle_view_version = 1,				// Versioning state of the 'vehicle view' setting group

			k_vehicle_view_name_length = 256,
			k_vehicle_view_max_seat_presets = 16,	// Per MAXIMUM_SEATS_PER_UNIT_DEFINITION in Halo 1 definitions
			k_vehicle_view_max_vehicle_presets = 32,
		};

		enum weapon_view {
			k_weapon_view_version = 1, // Versioning state of the 'weapon view' setting group

			k_weapon_view_name_length = 64,
			k_weapon_view_max_weapon_presets = 200,
		};

		enum interface_view {
			k_interface_view_version = 2, // Versioning state of the 'interface view' setting group
		};
	};

	namespace Settings
	{
	//////////////////////////////////////////////////////////////////////////
	// Vehicle View Presets

		struct s_vehicle_view_seat_preset
		{
			real_vector3d Offset;
		};
		struct s_vehicle_view_preset
		{
			tag_string Name;
			// Bit-vector detailing if a specific vehicle seat is overridden or not
			uint16 SeatPresentExists[ BIT_VECTOR_SIZE_IN_WORDS(Enums::k_vehicle_view_max_seat_presets) ];
			s_vehicle_view_seat_preset SeatPresets[Enums::k_vehicle_view_max_seat_presets];
		};

	//////////////////////////////////////////////////////////////////////////
	// Weapon View Presets

		struct s_weapon_view_preset
		{
			tag_string Name;
			real_vector3d Offset;
		};

	//////////////////////////////////////////////////////////////////////////
	// Interface View

		struct s_interface_view
		{
			struct {
				unsigned ShowHud : 1;
				unsigned ShowChat : 1;
				unsigned ShowKills : 1;
				unsigned ShowKillDeathMsg : 1;

				unsigned ScaleHud : 1;
			}Flags;

			struct {
				real VerticalRadians;
			}Fov; // Field Of View
		};
	};


//////////////////////////////////////////////////////////////////////////
// Server settings

	namespace Enums
	{
	};

	namespace Settings
	{
	};
};