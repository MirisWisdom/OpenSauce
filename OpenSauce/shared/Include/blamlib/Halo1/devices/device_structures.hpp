/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/game_configuration.hpp>
#include <blamlib/Halo1/objects/object_structures.hpp>

namespace Yelo
{
	namespace Flags
	{
		enum // device flags
		{
			_device_position_reversed_bit,
			_device_not_usable_from_any_side_bit,
		};
		enum // user_interation_flags
		{
			_device_one_sided_bit,
			_device_operates_automatically_bit,
		};

		// datum flags match the same order as the scenario datum's (perhaps move to device_machines.hpp?)
		enum {
			_device_machine_does_not_operate_automatically_bit,
			_device_machine_one_sided_bit,
			_device_machine_never_appears_locked_bit,
			_device_machine_opened_by_melee_attack_bit,
		};

		enum {
			_scenario_device_control_usable_from_both_sides_bit,
			_scenario_device_control_unused1_bit,
			_scenario_device_control_unused2_bit,
			_scenario_device_control_unused3_bit,
			_scenario_device_control_unused4_bit,
		};
		enum {
			_device_control_usable_from_both_sides_bit,
			_device_control_unused1_bit, // set when _scenario_device_control_unused4_bit
		};
	};

	namespace Objects
	{
		struct s_device_data
		{
			long_flags flags;					// 0x1F4

			struct {
				int16 device_group_index;
				PAD16;
				real value, change;
			}	power,							// 0x1F8
				position;						// 0x204

			long_flags user_interation_flags;	// 0x210
		}; BOOST_STATIC_ASSERT( sizeof(s_device_data) == Enums::k_object_size_device - Enums::k_object_size_object );


		struct s_device_machine_data
		{
			long_flags flags;					// 0x214
			game_ticks_t door_opening_ticks;	// 0x218, number of ticks passed since the door started opening
			real_point3d elevator_position;		// 0x21C
		}; BOOST_STATIC_ASSERT( sizeof(s_device_machine_data) == Enums::k_object_size_machine - Enums::k_object_size_device );

		struct s_device_control_data
		{
			long_flags flags;					// 0x214
			int16 custom_name_index;			// 0x218
			PAD16;
		}; BOOST_STATIC_ASSERT( sizeof(s_device_control_data) == Enums::k_object_size_control - Enums::k_object_size_device );

		struct s_device_lightfixture_data
		{
			real_rgb_color color;				// 0x214
			real intensity;
			real falloff_angle, cutoff_angle;
		}; BOOST_STATIC_ASSERT( sizeof(s_device_lightfixture_data) == Enums::k_object_size_light_fixture - Enums::k_object_size_device );



		struct s_device_datum
		{
			enum { k_object_types_mask = Enums::_object_type_mask_device };

			s_object_data object;
			s_device_data device;
		}; BOOST_STATIC_ASSERT( sizeof(s_device_datum) == Enums::k_object_size_device );

		struct s_device_machine_datum : s_device_datum
		{
			enum { k_object_types_mask = FLAG(Enums::_object_type_machine) };

			s_device_machine_data machine;
		}; BOOST_STATIC_ASSERT( sizeof(s_device_machine_datum) == Enums::k_object_size_machine );

		struct s_device_control_datum : s_device_datum
		{
			enum { k_object_types_mask = FLAG(Enums::_object_type_control) };

			s_device_control_data control;
		}; BOOST_STATIC_ASSERT( sizeof(s_device_control_datum) == Enums::k_object_size_control );

		struct s_device_lightfixture_datum : s_device_datum
		{
			enum { k_object_types_mask = FLAG(Enums::_object_type_light_fixture) };

			s_device_lightfixture_data lightfixture;
		}; BOOST_STATIC_ASSERT( sizeof(s_device_lightfixture_datum) == Enums::k_object_size_light_fixture );
	};
};