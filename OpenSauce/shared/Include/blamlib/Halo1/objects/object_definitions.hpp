/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/cache/predicted_resources.hpp>
#include <blamlib/Halo1/models/model_definitions.hpp>
#include <blamlib/Halo1/objects/object_types.hpp>

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_number_of_attachments_per_object = 8,
		};

		enum incoming_object_function {
			_incoming_object_function_a,
			_incoming_object_function_b,
			_incoming_object_function_c,
			_incoming_object_function_d,

			k_number_of_incoming_object_functions
		};
		enum outgoing_object_function {
			_outgoing_object_function_a,
			_outgoing_object_function_b,
			_outgoing_object_function_c,
			_outgoing_object_function_d,

			k_number_of_outgoing_object_functions
		};

		enum object_function_reference {
			_object_function_reference_none,
			_object_function_reference_a,
			_object_function_reference_b,
			_object_function_reference_c,
			_object_function_reference_d,

			k_number_of_object_function_references,
		};

		enum object_function_scalar {
			_object_function_scalar_none,

			_object_function_scalar_a_in,
			_object_function_scalar_b_in,
			_object_function_scalar_c_in,
			_object_function_scalar_d_in,

			_object_function_scalar_a_out,
			_object_function_scalar_b_out,
			_object_function_scalar_c_out,
			_object_function_scalar_d_out,

			k_number_of_object_function_scalars,
		};

		enum object_function_bounds_mode {
			_object_function_bounds_mode_clip,
			_object_function_bounds_mode_clip_and_normalize,
			_object_function_bounds_mode_scale_to_fit,

			k_number_of_object_function_bounds_modes,
		};

		enum object_change_color : _enum
		{
			_object_change_color_a,
			_object_change_color_b,
			_object_change_color_c,
			_object_change_color_d,

			k_number_of_object_change_colors,
		};
		enum object_change_color_reference : _enum
		{
			_object_change_color_reference_none,
			_object_change_color_reference_a,
			_object_change_color_reference_b,
			_object_change_color_reference_c,
			_object_change_color_reference_d,

			k_number_of_object_change_color_references,
		};
	};

	namespace Flags
	{
		enum object_definition_flags : word_flags
		{
			_object_definition_does_not_cast_shadow_bit,
			_object_definition_transparent_self_occlusion_bit,
			_object_definition_brighter_than_it_should_bit,
			_object_definition_not_pathfinding_obstacle_bit,

			k_number_of_object_definition_flags,

			_object_definition_cast_shadow_by_default_yelo_bit = k_number_of_object_definition_flags,

			k_number_of_object_definition_flags_yelo,
		};
	};

	namespace TagGroups
	{
		struct object_attachment_definition
		{
			tag_reference type;
			tag_string marker;
			struct {
				_enum primary_scale;
				_enum secondary_scale;
				_enum change_color;
				PAD16;
			}function_references; // Enums::object_function_reference
			TAG_PAD(int32, 4);
		};

		struct object_definition_widget
		{
			tag_reference reference;
			TAG_PAD(int32, 4);
		};

		struct s_object_function_definition
		{
			struct _object_function_flags {
				TAG_FLAG(invert);
				TAG_FLAG(additive);
				TAG_FLAG(always_active);
			}flags; BOOST_STATIC_ASSERT( sizeof(_object_function_flags) == sizeof(long_flags) );

			TAG_FIELD(real, period);
			TAG_ENUM(period_scale, Enums::object_function_scalar);

			TAG_ENUM(function, Enums::periodic_function);
			TAG_ENUM(function_scale, Enums::object_function_scalar);

			TAG_ENUM(wobble_function, Enums::periodic_function);
			TAG_FIELD(real, wobble_period);
			TAG_FIELD(real, wobble_magnitude);

			TAG_FIELD(real, square_wave_threshold);
			TAG_FIELD(int16, step_count);
			TAG_ENUM(map_to, Enums::transition_function);
			TAG_FIELD(int16, sawtooth_count);

			TAG_ENUM(add, Enums::object_function_scalar);
			TAG_ENUM(result_scale, Enums::object_function_scalar);

			TAG_ENUM(bounds_mode, Enums::object_function_bounds_mode);
			TAG_FIELD(real_fraction_bounds, bounds);
			PAD32;
			PAD16;
			TAG_FIELD(int16, turn_off_with, s_object_function_definition);
			TAG_FIELD(real, scale_by);
			TAG_PAD(byte, 252); // useless padding

			// postprocessed fields; labeled as PAD(16) in group definitions
			real_fraction bounds_normal;		// 1f / (max - min)
			real_fraction step_count_normal;	// 1f / (--step_count)
			real_fraction sawtooth_count_normal;// 1f / (--sawtooth_count)
			real_fraction period_normal;		// 1f / period

			TAG_FIELD(tag_string, usage);
		}; BOOST_STATIC_ASSERT( sizeof(s_object_function_definition) == 0x168 );

		struct _object_definition
		{
			TAG_ENUM(runtime_object_type, Enums::object_type);
			word_flags flags;

			real bounding_radius;
			real_point3d bounding_offset, origin_offset;
			real acceleration_scale;
			struct {
				TAG_FLAG(change_colors_scaled_by_function); // toggled when there is 1+ change colors that have their scale-by set to something other than NONE
			}runtime_flags;

			struct {
				TAG_FIELD(tag_reference, render_model,		'mod2');
				TAG_FIELD(tag_reference, animations,		'antr');
				TAG_PAD(int32, 10);
				TAG_FIELD(tag_reference, collision,			'coll');
				TAG_FIELD(tag_reference, physics,			'phys');
				TAG_FIELD(tag_reference, modifier_shader,	'shdr');
				TAG_FIELD(tag_reference, creation_effect,	'effe');
			}references;
			TAG_PAD(int32, 21);

			real render_bounding_radius;
			
			_enum function_exports[Enums::k_number_of_incoming_object_functions];
			TAG_PAD(int32, 11);
			int16 hud_text_message_index;
			int16 forced_shader_permutation;

			TAG_TBLOCK(attachments, object_attachment_definition);

			TAG_TBLOCK(widgets, object_definition_widget);

			TAG_PAD(tag_block, 1); // object_function_definition
			TAG_PAD(tag_block, 1); // object_change_color_definition
			TAG_TBLOCK_(predicted_resources, predicted_resource); // predicted resources
		}; BOOST_STATIC_ASSERT( sizeof(_object_definition) == 0x17C );


		struct s_object_definition
		{
			enum { k_group_tag = 'obje' };

			_object_definition object;
		};
	};
};