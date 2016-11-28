/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <blamlib/Halo1/shaders/shader_definitions.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum particle_creation_physics : _enum
		{
			_particle_creation_physics_default,
			_particle_creation_physics_explosion,
			_particle_creation_physics_jet,

			_particle_creation_physics,
		};
		enum particle_update_physics : _enum
		{
			_particle_update_physics_default,

			_particle_update_physics,
		};
		enum complex_sprite_render_modes : _enum
		{
			_complex_sprite_render_modes_simple,
			_complex_sprite_render_modes_rotational,

			_complex_sprite_render_modes,
		};
		enum system_update_physics : _enum
		{
			_system_update_physics_default,
			_system_update_physics_explosion,

			_system_update_physics,
		};
	};
	namespace TagGroups
	{
		struct s_particle_system_physics_constants
		{
			TAG_FIELD(real, k, "", "The meaning of this constant depends on the selected physics creation/update function.");
		}; BOOST_STATIC_ASSERT( sizeof(s_particle_system_physics_constants) == 0x4 ); // max count: 16

		struct s_particle_system_type_states
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(real_bounds, duration_bounds, "seconds", "Time in this state.");
			TAG_FIELD(real_bounds, transition_time_bounds, "seconds", "Time spent in transition to next state.");
			PAD32;
			TAG_FIELD(real, scale_multiplier, "", "This value will be multiplied into the scale computed by the particles' state.");
			TAG_FIELD(real, animation_rate_multiplier, "", "This value will be multiplied into the animation rate computed by the particles' state.");
			TAG_FIELD(real, rotation_rate_multiplier, "", "This value will be multiplied into the rotation rate computed by the particles' state.");
			TAG_FIELD(real_argb_color, color_multiplier, "", "This value will be multiplied into the color computed by the particles' state.");
			TAG_FIELD(real, radius_multiplier, "", "This value will be multiplied into the radius computed by the type.");
			TAG_FIELD(real, minimum_particle_count);
			TAG_FIELD(real, particle_creation_rate, "particles per second");
			TAG_PAD(int32, 21);
			TAG_ENUM(particle_creation_physics, Enums::particle_creation_physics, "This controls the placement of particles created during this state.");
			TAG_ENUM(particle_update_physics, Enums::particle_update_physics, "This controls the motion of particles during this state.");
			TAG_TBLOCK(physics_constants, s_particle_system_physics_constants);
		}; BOOST_STATIC_ASSERT( sizeof(s_particle_system_type_states) == 0xC0 ); // max count: 8

		struct s_particle_system_type_particle_states
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(real_bounds, duration_bounds, "seconds", "Time in this state.");
			TAG_FIELD(real_bounds, transition_time_bounds, "seconds", "Time spent in transition to next state.");
			TAG_FIELD(tag_reference, bitmaps, 'bitm');
			TAG_FIELD(int16, sequence_index);
			PAD16;
			PAD32;
			TAG_FIELD(real_bounds, scale, "world units per pixel", "Apparent size of the particles.");
			TAG_FIELD(real_bounds, animation_rate, "frames per second", "Rate of sprite animation.");
			TAG_FIELD(angle_bounds, rotation_rate, "degrees per second", "Rate of texture rotation.");
			TAG_FIELD(real_argb_color, color_1, "", "Particle will have a random color in the range determined by these two colors.");
			TAG_FIELD(real_argb_color, color_2, "", "Particle will have a random color in the range determined by these two colors.");
			TAG_FIELD(real, radius_multiplier, "", "This value will be multiplied into the radius computed by the type.");
			TAG_FIELD(tag_reference, point_physics, 'pphy');
			TAG_PAD(int32, 9);
			s_shader_effect shader_effect;
			TAG_TBLOCK(physics_constants, s_particle_system_physics_constants);
		}; BOOST_STATIC_ASSERT( sizeof(s_particle_system_type_particle_states) == 0x178 ); // max count: 8

		struct s_particle_system_types
		{
			struct __flags
			{
				TAG_FLAG(type_states_loop);
				TAG_FLAG(_forward_backward);
				TAG_FLAG(particle_states_loop);
				TAG_FLAG(_forward_backward_1);
				TAG_FLAG(particles_die_in_water);
				TAG_FLAG(particles_die_in_air);
				TAG_FLAG(particles_die_on_ground);
				TAG_FLAG(rotational_sprites_animate_sideways, "if the complex sprite mode is rotational and this flag is set, the sideways sequence is contains an animation rather than a set of permutations.");
				TAG_FLAG(disabled);
				TAG_FLAG(tint_by_effect_color);
				TAG_FLAG(initial_count_scales_with_effect);
				TAG_FLAG(minimum_count_scales_with_effect);
				TAG_FLAG(creation_rate_scales_with_effect);
				TAG_FLAG(scale_scales_with_effect);
				TAG_FLAG(animation_rate_scales_with_effect);
				TAG_FLAG(rotation_rate_scales_with_effect);
				TAG_FLAG(don_t_draw_in_first_person);
				TAG_FLAG(don_t_draw_in_third_person);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

			struct __physics_flags
			{
				TAG_FLAG(unused);
			}; BOOST_STATIC_ASSERT( sizeof(__physics_flags) == sizeof(long_flags) );

			TAG_FIELD(tag_string, name);
			TAG_FIELD(__flags, flags);
			TAG_FIELD(int16, initial_particle_count);
			PAD16;
			TAG_ENUM(complex_sprite_render_modes, Enums::complex_sprite_render_modes);
			PAD16;
			TAG_FIELD(real, radius, "world units");
			TAG_PAD(int32, 9);
			TAG_ENUM(particle_creation_physics, Enums::particle_creation_physics, "This controls the initial placement of particles.");
			PAD16;
			TAG_FIELD(__physics_flags, physics_flags);
			TAG_TBLOCK(physics_constants, s_particle_system_physics_constants);
			TAG_TBLOCK(states, s_particle_system_type_states);
			TAG_TBLOCK(particle_states, s_particle_system_type_particle_states);
		}; BOOST_STATIC_ASSERT( sizeof(s_particle_system_types) == 0x80 ); // max count: 4

		struct s_particle_system_definition
		{
			enum { k_group_tag = 'pctl' };

			struct __physics_flags
			{
				TAG_FLAG(unused);
			}; BOOST_STATIC_ASSERT( sizeof(__physics_flags) == sizeof(long_flags) );

			PAD32;
			TAG_PAD(int32, 13);

			////////////////////////////////////////////////////////////////
			// system behavior
			// These settings affect the behavior of the system's origin.
			TAG_FIELD(tag_reference, point_physics, 'pphy');
			TAG_ENUM(system_update_physics, Enums::system_update_physics);
			PAD16;
			TAG_FIELD(__physics_flags, physics_flags);
			TAG_TBLOCK(physics_constants, s_particle_system_physics_constants);
			TAG_TBLOCK(particle_types, s_particle_system_types);
		}; BOOST_STATIC_ASSERT( sizeof(s_particle_system_definition) == 0x68 ); // max count: 1
	};
};