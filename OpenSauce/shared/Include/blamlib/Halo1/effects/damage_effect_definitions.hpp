/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/effects/material_effect_definitions.hpp>

#include <blamlib/Halo1/tag_files/tag_groups.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum damage_side_effect
		{
			_damage_side_effect_none,
			_damage_side_effect_harmless,
			_damage_side_effect_legthal_to_unsuspecting,
			_damage_side_effect_emp,

			_damage_side_effect,
		};

		enum damage_category
		{
			_damage_category_none,
			_damage_category_falling,
			_damage_category_bullet,
			_damage_category_grenade,
			_damage_category_high_explosive,
			_damage_category_sniper,
			_damage_category_melee,
			_damage_category_flame,
			_damage_category_mounted_weapon,
			_damage_category_vehicle,
			_damage_category_plasma,
			_damage_category_needle,
			_damage_category_shotgun,

			_damage_category
		};
	};

	namespace TagGroups
	{
		struct s_damage_effect
		{
			TAG_FIELD(real_bounds, radius);
			TAG_FIELD(real_fraction, cutoff_scale);
			TAG_FIELD(long_flags, flags); // not exposed for continuous_damage_effect
			TAG_PAD(int32, 5);
		}; BOOST_STATIC_ASSERT( sizeof(s_damage_effect) == 0x24 );

		struct s_damage_camera_effect
		{
			PAD32;
			TAG_PAD(int32, 3);
			TAG_FIELD(real, shake_duration); // not exposed for continuous_damage_effect
			TAG_ENUM(shake_falloff_function, Enums::transition_function); // not exposed for continuous_damage_effect
			PAD16;

			TAG_FIELD(real, random_translation);
			TAG_FIELD(real, random_rotation);
			TAG_PAD(int32, 3);

			TAG_ENUM(wobble_function, Enums::periodic_function);
			PAD16;
			TAG_FIELD(real, wobble_period);
			TAG_FIELD(real_fraction, wobble_weight);
			PAD32;

			TAG_PAD(int32, 5);
			TAG_PAD(int32, 2);
		}; BOOST_STATIC_ASSERT( sizeof(s_damage_camera_effect) == 0x58 );

		struct s_damage_breaking_effect
		{
			TAG_PAD(int32, 28);

			struct {
				TAG_FIELD(real, velocity);
				TAG_FIELD(real, radius);
				TAG_FIELD(real, exponent);
				TAG_PAD(int32, 3);
			}forward, outward;
		}; BOOST_STATIC_ASSERT( sizeof(s_damage_breaking_effect) == 0xA0 );

		struct s_damage_data
		{
			TAG_ENUM(side_effect, Enums::damage_side_effect);
			TAG_ENUM(category, Enums::damage_category);
			TAG_FIELD(long_flags, flags);
			TAG_FIELD(real, area_of_effect_core_radius); // not exposed for continuous_damage_effect
			TAG_FIELD(real, damage_lower_bound);
			TAG_FIELD(real_bounds, damage_upper_bound);
			TAG_FIELD(real, vehicle_pass_through_penalty);
			TAG_FIELD(real, active_camo_damage); // not exposed for continuous_damage_effect
			TAG_FIELD(real, stun);
			TAG_FIELD(real, max_stun);
			TAG_FIELD(real, stun_time);
			PAD32;
			TAG_FIELD(real, instantaneous_acceleration);
			PAD32; PAD32;
		}; BOOST_STATIC_ASSERT( sizeof(s_damage_data) == 0x3C );

		struct s_damage_modifiers
		{
			real modifier[Enums::k_number_of_material_types];
			TAG_PAD(int32, 7);
		}; BOOST_STATIC_ASSERT( sizeof(s_damage_modifiers) == 0xA0 );

		struct s_continuous_damage_effect_definition
		{
			s_damage_effect damage_effect;

			TAG_FIELD(real_fraction_bounds, vibrate_frequency);
			s_damage_camera_effect damage_camera_effect;
			s_damage_breaking_effect breaking_effect; // not exposed
			s_damage_data damage_data;
			s_damage_modifiers damage_modifiers;
		}; BOOST_STATIC_ASSERT( sizeof(s_continuous_damage_effect_definition) == 0x200 );

		struct s_damage_effect_definition
		{
			struct s_duration_function {
				TAG_FIELD(real, duration);
				TAG_ENUM(fade_function, Enums::transition_function);
				PAD16;

				TAG_FIELD(angle, rotation); // these are only exposed for camera impulse freq
				TAG_FIELD(real, pushback);
			};

			s_damage_effect damage_effect;

			struct s_screen_flash {
				TAG_ENUM(type, );
				TAG_ENUM(priority, );
				TAG_PAD(int32, 3);

				s_duration_function function;
				TAG_FIELD(real_fraction, max_intensity);
				PAD32;
				TAG_FIELD(real_argb_color, color);
			}screen_flash;

			struct s_rumble_frequency {
				TAG_FIELD(real_fraction, frequency);
				s_duration_function function;
			} rumble_low, rumble_high;
			TAG_PAD(s_rumble_frequency, 1);

			struct s_camera_impulse_frequency {
				s_duration_function function;
				TAG_FIELD(real_bounds, jitter);
				PAD32; PAD32;
			}temp_camera_impulse;
			TAG_FIELD(angle, perm_camera_impulse_angle);

			s_damage_camera_effect damage_camera_effect;

			TAG_FIELD(tag_reference, sound, 'snd!');

			s_damage_breaking_effect breaking_effect;
			s_damage_data damage_data;
			s_damage_modifiers damage_modifiers;
		}; BOOST_STATIC_ASSERT( sizeof(s_damage_effect_definition) == 0x2A0 );
	};
};