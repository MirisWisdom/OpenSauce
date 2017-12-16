/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace AI
	{
		enum
		{
			k_maximum_number_of_mounted_weapon_units = 8,
		};

		enum ai_sound_volume
		{
			k_number_of_ai_sound_volumes = 5,
		};

		enum ai_unit_effect
		{
			k_number_of_ai_unit_effects = 4,
		};

		namespace e_ai_spatial_effect
		{
			typedef enum : _enum
			{
				environmental_noise,
				weapon_impact,
				weapon_detonation,

				k_count
			} type_t;
		}

		struct s_ai_globals_data;

		s_ai_globals_data* AIGlobals();
	}
}
