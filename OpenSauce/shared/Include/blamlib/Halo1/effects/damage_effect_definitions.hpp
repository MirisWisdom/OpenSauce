/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/effects/material_effect_definitions.hpp>

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

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

	namespace Flags
	{
		enum {
			_damage_does_not_hurt_owner_bit,
			_damage_can_cause_headshots_bit,
			_damage_pings_resistant_units_bit,
			_damage_does_not_hurt_friends_bit,
			_damage_does_not_ping_units_bit,
			_damage_detonates_explosives_bit,
			_damage_only_hurts_shields_bit,
			_damage_causes_flaming_death_bit,
			_damage_direction_indicators_always_point_down_bit,
			_damage_skips_shields_bit,
			_damage_only_hurts_one_infection_form_bit,
			_damage_can_cause_multiplayer_headshots_bit,
			_damage_infection_form_pop_bit,

			k_number_of_damage_flags,

			_damage_use_3d_acceleration_yelo_bit = k_number_of_damage_flags,

			k_number_of_damage_flags_yelo,
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

		struct s_damage_definition
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

			union {
				TAG_FIELD(real, instantaneous_acceleration);
				//PAD32; PAD32;

				// modifies instantaneous_acceleration to support specifying the j & k component acceleration
				TAG_FIELD(real_vector3d, instantaneous_acceleration3d);
			};

			bool UseInstantaneousAcceleration3D() const
			{
				return TEST_FLAG(flags, Flags::_damage_use_3d_acceleration_yelo_bit);
			}
		}; BOOST_STATIC_ASSERT( sizeof(s_damage_definition) == 0x3C );

		struct s_damage_modifiers
		{
			real modifier[Enums::k_number_of_material_types];
			TAG_PAD(int32, 7);
		}; BOOST_STATIC_ASSERT( sizeof(s_damage_modifiers) == 0xA0 );

		struct s_damage_effect_definition
		{
			enum { k_group_tag = 'jpt!' };

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
			s_damage_definition damage;
			s_damage_modifiers damage_modifiers;
		}; BOOST_STATIC_ASSERT( sizeof(s_damage_effect_definition) == 0x2A0 );


		struct s_continuous_damage_effect_definition
		{
			enum { k_group_tag = 'cdmg' };

			s_damage_effect damage_effect;

			TAG_FIELD(real_fraction_bounds, vibrate_frequency);
			s_damage_camera_effect damage_camera_effect;
			s_damage_breaking_effect breaking_effect; // not exposed
			s_damage_definition damage;
			s_damage_modifiers damage_modifiers;
		}; BOOST_STATIC_ASSERT( sizeof(s_continuous_damage_effect_definition) == 0x200 );
	};
};