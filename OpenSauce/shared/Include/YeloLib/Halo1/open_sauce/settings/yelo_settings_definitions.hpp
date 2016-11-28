/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <YeloLib/Halo1/open_sauce/yelo_version.hpp>

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

			k_vehicle_view_name_length = 255,
			k_vehicle_view_max_seat_presets = 16,	// Per MAXIMUM_SEATS_PER_UNIT_DEFINITION in Halo 1 definitions
			k_vehicle_view_max_vehicle_presets = 32,
		};

		enum weapon_view {
			k_weapon_view_version = 1, // Versioning state of the 'weapon view' setting group

			k_weapon_view_name_length = 63,
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