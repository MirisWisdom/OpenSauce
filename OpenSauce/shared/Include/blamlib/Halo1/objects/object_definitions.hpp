/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/cache/predicted_resources.hpp>
#include <blamlib/Halo1/models/model_definitions.hpp>

#include <blamlib/Halo1/tag_files/tag_groups.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_number_of_attachments_per_object = 8,
		};

		enum object_type
		{
			_object_type_object = -4,
			_object_type_device = -3,
			_object_type_item = -2,
			_object_type_unit = -1,

			_object_type_biped = 0,
			_object_type_vehicle,
			_object_type_weapon,
			_object_type_equipment,
			_object_type_garbage,
			_object_type_projectile,
			_object_type_scenery,
			_object_type_machine,
			_object_type_control,
			_object_type_light_fixture,
			_object_type_placeholder,
			_object_type_sound_scenery,

			_object_type_mask_device = 
				FLAG(_object_type_machine) |
				FLAG(_object_type_control) |
				FLAG(_object_type_light_fixture),
			_object_type_mask_item = 
				FLAG(_object_type_weapon) |
				FLAG(_object_type_equipment) |
				FLAG(_object_type_garbage),
			_object_type_mask_unit = 
				FLAG(_object_type_biped) |
				FLAG(_object_type_vehicle),
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
			}function_inputs; // _object_definition.function_exports
			TAG_PAD(int32, 4);
		};

		struct object_definition_widget
		{
			tag_reference reference;
			TAG_PAD(int32, 4);
		};

		struct _object_definition
		{
			TAG_ENUM(runtime_object_type, Enums::object_type);
			struct _object_definition_flags {
				TAG_FLAG16(does_not_cast_shadow);
				TAG_FLAG16(transparent_self_occlusion);
				TAG_FLAG16(brighter_than_it_should);
				TAG_FLAG16(not_pathfinding_obstacle);
			}flags; BOOST_STATIC_ASSERT( sizeof(_object_definition_flags) == sizeof(word_flags) );

			real bounding_radius;
			real_point3d bounding_offset, origin_offset;
			real acceleration_scale;
			TAG_PAD(int32, 1);

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
			
			_enum function_exports[4];
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