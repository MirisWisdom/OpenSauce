/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <blamlib/Halo1/shaders/shader_definitions.hpp>
#include <blamlib/Halo1/effects/particle_definitions.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum weather_particle_render_direction_source : _enum
		{
			_weather_particle_render_direction_source_from_velocity,
			_weather_particle_render_direction_source_from_acceleration,

			_weather_particle_render_direction_source,
		};
	};
	namespace TagGroups
	{
		struct s_weather_particle_type
		{
			struct __flags
			{
				TAG_FLAG(interpolate_colors_in_hsv);
				TAG_FLAG(_along_long_hue_path);
				TAG_FLAG(random_rotation);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

			TAG_FIELD(tag_string, name);
			TAG_FIELD(__flags, flags);
			TAG_FIELD(real, fade_in_start_distance, "world units", "Particles begin to fade into visibility beyond this distance");
			TAG_FIELD(real, fade_in_end_distance, "world units", "Particles become fully visible beyond this distance");
			TAG_FIELD(real, fade_out_start_distance, "world units", "Particles begin to fade out of visibility beyond this distance");
			TAG_FIELD(real, fade_out_end_distance, "world units", "Particles become fully invisible beyond this distance");
			TAG_FIELD(real, fade_in_start_height, "world units", "Particles begin to fade into visibility above this height");
			TAG_FIELD(real, fade_in_end_height, "world units", "Particles become fully visible above this height");
			TAG_FIELD(real, fade_out_start_height, "world units", "Particles begin to fade out of visibility above this height");
			TAG_FIELD(real, fade_out_end_height, "world units", "Particles become fully invisible above this height");
			TAG_PAD(int32, 24);
			TAG_FIELD(real_bounds, particle_count, "particles per cubic world unit");
			TAG_FIELD(tag_reference, physics, 'pphy');
			TAG_PAD(int32, 4);
			TAG_FIELD(real_bounds, acceleration_magnitude);
			TAG_FIELD(real_fraction, acceleration_turning_rate);
			TAG_FIELD(real, acceleration_change_rate);
			TAG_PAD(int32, 8);
			TAG_FIELD(real_bounds, particle_radius, "world units");
			TAG_FIELD(real_bounds, animation_rate, "frames per second");
			TAG_FIELD(angle_bounds, rotation_rate, "degrees per second");
			TAG_PAD(int32, 8);
			TAG_FIELD(real_argb_color, color_lower_bound);
			TAG_FIELD(real_argb_color, color_upper_bound);
			TAG_PAD(int32, 16);

			////////////////////////////////////////////////////////////////
			// SHADER
			TAG_FIELD(tag_reference, sprite_bitmap, 'bitm');
			TAG_ENUM(render_mode, Enums::particle_orientation);
			TAG_ENUM(render_direction_source, Enums::weather_particle_render_direction_source, "Render modes that depend on an direction will use this vector.");			
			s_shader_effect shader_effect;
		}; BOOST_STATIC_ASSERT( sizeof(s_weather_particle_type) == 0x25C ); // max count: 8

		struct s_weather_particle_system_definition
		{
			enum { k_group_tag = 'rain' };

			struct __flags
			{
				TAG_FLAG(unused);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

			TAG_FIELD(__flags, flags);
			TAG_PAD(int32, 8);
			TAG_TBLOCK(particle_types, s_weather_particle_type);
		}; BOOST_STATIC_ASSERT( sizeof(s_weather_particle_system_definition) == 0x30 ); // max count: 1
	};
};