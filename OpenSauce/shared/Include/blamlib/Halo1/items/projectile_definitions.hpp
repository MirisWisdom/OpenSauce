/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/objects/object_definitions.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum material_response
		{
			_material_response_disappear,
			_material_response_detonate,
			_material_response_reflect,
			_material_response_overpenetrate,
			_material_response_attach,

			_material_response,
		};
	};

	namespace TagGroups
	{
		struct s_projectile_material_response_definition
		{
			struct _flags {
				TAG_FLAG16(cannot_be_overpenetrated);
			}flags;

			struct _default_result {
				TAG_ENUM(response, material_response);
				TAG_FIELD(tag_reference, effect, 'effe');
				TAG_PAD(tag_reference, 1); // 16
			}default_result;

			struct _potential_result {
				TAG_ENUM(response, material_response);
				struct _flags {
					TAG_FLAG16(only_against_units);
				}flags;
				TAG_FIELD(real, skip_fraction);
				TAG_FIELD(angle_bounds, between, "degrees");
				TAG_FIELD(real_bounds, and, "world units per second");
				TAG_FIELD(tag_reference, effect, 'effe');
				TAG_PAD(tag_reference, 1); // 16
			}potential_result;

			TAG_ENUM(scale_effects_by, effect_scale);
			PAD16;
			TAG_FIELD(real, angular_noise, "degrees", "the angle of incidence is randomly perturbed by at most this amount to simulate irregularity.");
			TAG_FIELD(real, velocity_noise, "world units per second", "the velocity is randomly perturbed by at most this amount to simulate irregularity.");
			TAG_FIELD(tag_reference, detonation_effect, 'effe');
			TAG_PAD(int32, 6); // 24

			TAG_FIELD(real, initial_friction, "the fraction of the projectile's velocity lost on penetration");
			TAG_FIELD(real, max_distance, "the maximum distance the projectile can travel through on object of this material");

			TAG_FIELD(real, parallel_friction,"the fraction of the projectile's velocity parallel to the surface lost on impact");
			TAG_FIELD(real, perpendicular_friction, "the fraction of the projectile's velocity perpendicular to the surface lost on impact");
		}; BOOST_STATIC_ASSERT( sizeof(s_projectile_material_response_definition) == 0xA0 );

		struct _projectile_definition
		{
			struct _projectile_definition_flags {
				TAG_FLAG(oriented_along_velocity);
				TAG_FLAG(ai_must_use_basllistic_aiming);
				TAG_FLAG(detonation_max_time_if_attached);
				TAG_FLAG(has_super_combining_explosion);
				//combine initial velocity with parent velocity
				//random attached detonation time
				//minimum unattached detonation time
			}flags; BOOST_STATIC_ASSERT( sizeof(_projectile_definition_flags) == sizeof(long_flags) );

			TAG_ENUM(detonation_timer_starts, projectile_detonation_timer_mode);
			TAG_ENUM(impact_noise, ai_sound_volume);

			_enum function_exports[Enums::k_number_of_incoming_object_functions];

			TAG_FIELD(tag_reference, super_detonation, 'effe');

			TAG_FIELD(real, ai_perception_radius, "world units");
			TAG_FIELD(real, collision_radius, "world units");

			TAG_FIELD(real, arming_time, "seconds", "won't detonate before this time elapses");
			TAG_FIELD(real, danger_radius, "world units");
			TAG_FIELD(tag_reference, effect, 'effe');
			TAG_FIELD(real_bounds, timer, "seconds", "detonation countdown (zero is untimed)");
			TAG_FIELD(real, min_velocity, "world units per second", "detonates when slowed below this velocity");
			TAG_FIELD(real, max_range, "world units per second", "detonates after travelling this distance");

			TAG_FIELD(real, air_gravity_scale, "", "the proportion of normal gravity applied to the projectile when in air.");
			TAG_FIELD(real_bounds, air_damage_range, "world units", "the range over which damage is scaled when the projectile is in air.");
			TAG_FIELD(real, water_gravity_scale, "", "the proportion of normal gravity applied to the projectile when in water.");
			TAG_FIELD(real_bounds, water_damage_range, "world units", "the range over which damage is scaled when the projectile is in water.");
			TAG_FIELD(real, initial_velocity, "world units per second", "bullet's velocity when inflicting maximum damage");
			TAG_FIELD(real, final_velocity, "world units per second", "bullet's velocity when inflicting minimum damage");
			TAG_FIELD(real, guided_angular_velocity, "degrees per second");
			TAG_ENUM(detonation_noise, ai_sound_volume);
			PAD16;

			TAG_FIELD(tag_reference, detonation_started,		'effe');
			TAG_FIELD(tag_reference, flyby_sound,				'snd!');
			TAG_FIELD(tag_reference, attached_detonation_damage,'jpt!');
			TAG_FIELD(tag_reference, impact_damage,				'jpt!');
			TAG_PAD(tag_block, 1); // 12

			TAG_TBLOCK(material_responses, s_projectile_material_response_definition);

		}; BOOST_STATIC_ASSERT( sizeof(_projectile_definition) == 0xD0 );


		struct s_projectile_definition : s_object_definition
		{
			enum { k_group_tag = 'proj' };

			_projectile_definition projectile;
		};
	};
};