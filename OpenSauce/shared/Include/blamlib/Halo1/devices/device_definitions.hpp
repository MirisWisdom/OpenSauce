/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/objects/object_definitions.hpp>

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct _device_definition
		{
			struct _flags {
				TAG_FLAG(position_loops);
				TAG_FLAG(position_not_interpolated);
			}flags;

			struct _timing {
				TAG_FIELD(real, transition_time, "seconds");
				TAG_FIELD(real, acceleration_time, "seconds");
			}power_time, position_time, depower_time;

			_enum function_exports[Enums::k_number_of_incoming_object_functions]; // Enums::device_function_mode

			TAG_FIELD(tag_reference, open, 'snd!', 'effe');
			TAG_FIELD(tag_reference, close, 'snd!', 'effe');

			TAG_FIELD(tag_reference, opened, 'snd!', 'effe');
			TAG_FIELD(tag_reference, closed, 'snd!', 'effe');

			TAG_FIELD(tag_reference, depowered, 'snd!', 'effe');
			TAG_FIELD(tag_reference, repowered, 'snd!', 'effe');

			TAG_FIELD(real, delay_time, "seconds");
			TAG_PAD(real, 2); // 8

			TAG_FIELD(tag_reference, delay_effect, 'snd!', 'effe');
			TAG_FIELD(real, automatic_activation_radius, "world units");
			TAG_PAD(int32, 21);

			struct { // if([tag values] != 0) then * 30.0f, then 1.0f / result
				struct {
					real transition_time;
					real acceleration_time;
				}power_time, position_time, depower_time;

				real delay_time;
			}runtime;
		}; BOOST_STATIC_ASSERT( sizeof(_device_definition) == 0x114 );

		//////////////////////////////////////////////////////////////////////////

		struct _control_definition
		{
			TAG_ENUM(type, control_type);
			TAG_ENUM(triggers_when, control_trigger);
			TAG_FIELD(real, call_value);
			TAG_PAD(int32, 20); // 80

			TAG_FIELD(tag_reference, on, 'snd!', 'effe');
			TAG_FIELD(tag_reference, off, 'snd!', 'effe');
			TAG_FIELD(tag_reference, deny, 'snd!', 'effe');
		}; BOOST_STATIC_ASSERT( sizeof(_control_definition) == 0x88 );

		//////////////////////////////////////////////////////////////////////////

		struct _machine_definition
		{
			TAG_ENUM(type, machine_type);
			struct _Flags {
				TAG_FLAG16(pathfinding_obstacle);
				TAG_FLAG16(but_not_when_open);
				TAG_FLAG16(elevator, "lighting based on what's around, rather than what's below");
			}flags;
			TAG_FIELD(real, door_open_time, "seconds");
			TAG_PAD(int32, 20); // 80

			TAG_ENUM(collision_response, machine_collision_response);
			TAG_FIELD(short, elevator_node);
			TAG_PAD(int32, 13); // 52

			int32 runtime_door_open_time; // door_open_time * 30.0f
		}; BOOST_STATIC_ASSERT( sizeof(_machine_definition) == 0x94 );

		//////////////////////////////////////////////////////////////////////////


		struct s_device_definition : s_object_definition
		{
			enum { k_group_tag = 'devi' };

			_device_definition device;
		};

		struct s_control_definition : s_device_definition
		{
			enum { k_group_tag = 'ctrl' };

			_control_definition control;
		};

		struct s_machine_definition : s_device_definition
		{
			enum { k_group_tag = 'mach' };

			_machine_definition machine;
		};
	};
};