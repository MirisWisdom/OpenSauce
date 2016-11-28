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
		enum contrail_render_type : _enum
		{
			_contrail_render_type_vertical_orientation,
			_contrail_render_type_horizontal_orientation,
			_contrail_render_type_media_mapped,
			_contrail_render_type_ground_mapped,
			_contrail_render_type_viewer_facing,
			_contrail_render_type_double_marker_linked,

			_contrail_render_type,
		};
	};

	namespace TagGroups
	{
		struct s_contrail_point_states
		{
			struct __scale_flags
			{
				TAG_FLAG(duration);
				TAG_FLAG(duration_delta);
				TAG_FLAG(transition_duration);
				TAG_FLAG(transition_duration_delta);
				TAG_FLAG(width);
				TAG_FLAG(color);
			}; BOOST_STATIC_ASSERT( sizeof(__scale_flags) == sizeof(long_flags) );

			////////////////////////////////////////////////////////////////
			// state timing
			TAG_FIELD(real_bounds, duration, "seconds:seconds", "the time a point spends in this state");
			TAG_FIELD(real_bounds, transition_duration, "seconds", "the time a point takes to transition to the next state");

			////////////////////////////////////////////////////////////////
			// point variables
			TAG_FIELD(tag_reference, physics, 'pphy');
			TAG_PAD(int32, 8);
			TAG_FIELD(real, width, "world units", "contrail width at this point");
			TAG_FIELD(real_argb_color, color_lower_bound, "", "contrail color at this point");
			TAG_FIELD(real_argb_color, color_upper_bound, "", "contrail color at this point");
			TAG_FIELD(__scale_flags, scale_flags, "these flags determine which fields are scaled by the contrail density");
		}; BOOST_STATIC_ASSERT( sizeof(s_contrail_point_states) == 0x68 ); // max count: 16

		struct s_contrail_definition
		{
			enum { k_group_tag = 'cont' };

			struct __flags
			{
				TAG_FLAG16(first_point_unfaded);
				TAG_FLAG16(last_point_unfaded);
				TAG_FLAG16(points_start_pinned_to_media);
				TAG_FLAG16(points_start_pinned_to_ground);
				TAG_FLAG16(points_always_pinned_to_media);
				TAG_FLAG16(points_always_pinned_to_ground);
				TAG_FLAG16(edge_effect_fades_slowly);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(word_flags) );

			struct __scale_flags
			{
				TAG_FLAG16(point_generation_rate);
				TAG_FLAG16(point_velocity);
				TAG_FLAG16(point_velocity_delta);
				TAG_FLAG16(point_velocity_cone_angle);
				TAG_FLAG16(inherited_velocity_fraction);
				TAG_FLAG16(sequence_animation_rate);
				TAG_FLAG16(texture_scale_u);
				TAG_FLAG16(texture_scale_v);
				TAG_FLAG16(texture_animation_u);
				TAG_FLAG16(texture_animation_v);
			}; BOOST_STATIC_ASSERT( sizeof(__scale_flags) == sizeof(word_flags) );

			TAG_FIELD(__flags, flags);
			TAG_FIELD(__scale_flags, scale_flags, "these flags determine which fields are scaled by the contrail density");

			////////////////////////////////////////////////////////////////
			// point creation
			TAG_FIELD(real, point_generation_rate, "points per second", "this many points are generated per second");
			TAG_FIELD(real_bounds, point_velocity, "world units per second", "velocity added to each point's initial velocity");
			TAG_FIELD(angle, point_velocity_cone_angle, "degrees", "initial velocity is inside the cone defined by the marker's forward vector and this angle");
			TAG_FIELD(real_fraction, inherited_velocity_fraction, "", "fraction of parent object's velocity that is inherited by contrail points.");

			////////////////////////////////////////////////////////////////
			// rendering
			TAG_ENUM(render_type, Enums::contrail_render_type, "this specifies how the contrail is oriented in space");
			PAD16;
			TAG_FIELD(real, texture_repeats_u, "", "texture repeats per contrail segment");
			TAG_FIELD(real, texture_repeats_v, "", "texture repeats across contrail width");
			TAG_FIELD(real, texture_animation_u, "repeats per second", "the texture along the contrail is animated by this value");
			TAG_FIELD(real, texture_animation_v, "repeats per second", "the texture across the contrail is animated by this value");
			TAG_FIELD(real, animation_rate, "frames per second");
			TAG_FIELD(tag_reference, bitmap, 'bitm');
			TAG_FIELD(int16, first_sequence_index);
			TAG_FIELD(int16, sequence_count);
			TAG_PAD(int32, 16);
			s_shader_effect shader_effect;
			TAG_TBLOCK(point_states, s_contrail_point_states);
		}; BOOST_STATIC_ASSERT( sizeof(s_contrail_definition) == 0x144 ); // max count: 1
	};
};
