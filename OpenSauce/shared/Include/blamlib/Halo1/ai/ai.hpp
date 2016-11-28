/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_number_of_mounted_weapon_units = 8,
		};

		enum ai_sound_volume {
			k_number_of_ai_sound_volumes = 5,
		};

		enum ai_unit_effect {
			k_number_of_ai_unit_effects = 4,
		};

		enum ai_spatial_effect {
			_ai_spatial_effect_environmental_noise,
			_ai_spatial_effect_weapon_impact,
			_ai_spatial_effect_weapon_detonation,

			k_number_of_ai_spatial_effects
		};
	};

	namespace AI
	{
		struct s_ai_globals_data;

		s_ai_globals_data*				AIGlobals();
	};
};