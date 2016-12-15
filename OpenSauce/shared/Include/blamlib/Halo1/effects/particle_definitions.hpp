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
		enum particle_orientation : _enum
		{
			_particle_orientation_screen_facing,
			_particle_orientation_parallel_to_direction,
			_particle_orientation_perpendicular_to_direction,

			_particle_orientation,
		};
	};
	namespace TagGroups
	{
		struct s_particle_definition
		{
			enum { k_group_tag = 'part' };

			struct __flags
			{
				TAG_FLAG(can_animate_backwards);
				TAG_FLAG(animation_stops_at_rest);
				TAG_FLAG(animation_starts_on_random_frame);
				TAG_FLAG(animate_once_per_frame);
				TAG_FLAG(dies_at_rest);
				TAG_FLAG(dies_on_contact_with_structure);
				TAG_FLAG(tint_from_diffuse_texture);
				TAG_FLAG(dies_on_contact_with_water);
				TAG_FLAG(dies_on_contact_with_air);
				TAG_FLAG(self_illuminated, "don't cast world-lights onto this particle");
				TAG_FLAG(random_horizontal_mirroring);
				TAG_FLAG(random_vertical_mirroring);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

			TAG_FIELD(__flags, flags);
			TAG_FIELD(tag_reference, bitmap, 'bitm');
			TAG_FIELD(tag_reference, physics, 'pphy');
			TAG_FIELD(tag_reference, marty_traded_his_kids_for_this, 'foot');
			PAD32;
			TAG_FIELD(real_bounds, lifespan, "seconds");
			TAG_FIELD(real, fade_in_time);
			TAG_FIELD(real, fade_out_time);
			TAG_FIELD(tag_reference, collision_effect, 'snd!', 'effe');
			TAG_FIELD(tag_reference, death_effect, 'snd!', 'effe');
			TAG_FIELD(real, minimum_size, "pixels", "in the distance, don't get any smaller than this size on the screen");
			TAG_PAD(int32, 2);
			TAG_FIELD(real_bounds, radius_animation, "", "when created, the radius is multiplied by the first number. at the end of the lifetime, the radius is multiplied by the second number.");
			PAD32;
			TAG_FIELD(real_bounds, animation_rate, "frames per second", "rate at which the particle animates");
			TAG_FIELD(real, contact_deterioration, "", "the decrease in the frame rate caused by a collision");
			TAG_FIELD(real, fade_start_size, "pixels", "begin fading the particle out when it's smaller than this size on the screen");
			TAG_FIELD(real, fade_end_size, "pixels", "kill the particle when it's smaller than this size on the screen");
			PAD32;
			TAG_FIELD(int16, first_sequence_index, "", "the index of the first sequence in the bitmap group used by this particle");
			TAG_FIELD(int16, initial_sequence_count);
			TAG_FIELD(int16, looping_sequence_count);
			TAG_FIELD(int16, final_sequence_count);
			TAG_PAD(int32, 3);
			TAG_ENUM(orientation, Enums::particle_orientation);
			PAD16;
			s_shader_effect shader_effect;
		}; BOOST_STATIC_ASSERT( sizeof(s_particle_definition) == 0x164 ); // max count: 1
	};
};