/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/objects/object_definitions.hpp>

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct _item_definition
		{
			struct _item_definition_flags {
				TAG_FLAG(maintains_z_up);
				TAG_FLAG(destroyed_by_explosions);
				TAG_FLAG(unaffected_by_gravity);
			}flags; BOOST_STATIC_ASSERT( sizeof(_item_definition_flags) == sizeof(long_flags) );

			int16 message_index;
			int16 sort_order;
			real scale;
			int16 hud_message_value_scale;
			PAD16;

			TAG_PAD(int32, 4);
			_enum function_exports[Enums::k_number_of_incoming_object_functions];
			TAG_PAD(int32, 41);

			struct {
				TAG_FIELD(tag_reference, material_effects, 'foot');
				TAG_FIELD(tag_reference, collision_sound, 'snd!');
			}references;

			TAG_PAD(int32, 30);

			struct {
				real_bounds delay;
				TAG_FIELD(tag_reference, detonating_effect, 'effe');
				TAG_FIELD(tag_reference, effect, 'effe');
			}detonation;
		}; BOOST_STATIC_ASSERT( sizeof(_item_definition) == 0x18C );


		struct s_item_definition : s_object_definition
		{
			_item_definition item;
		}; BOOST_STATIC_ASSERT( sizeof(s_item_definition) == 0x308 );
	};
};