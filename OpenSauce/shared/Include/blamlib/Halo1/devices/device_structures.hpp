/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/objects/object_structures.hpp>

namespace Yelo
{
	namespace Flags
	{
		enum device_flags
		{

			// user_interation_flags

			_device_one_sided_bit,
			_device_operates_automatically_bit,
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
			UNKNOWN_TYPE(int32);				// 0x218, looks like a timer used for door-type machines
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
			s_object_data object;
			s_device_data device;
		}; BOOST_STATIC_ASSERT( sizeof(s_device_datum) == Enums::k_object_size_device );

		struct s_device_machine_datum : s_device_datum
		{
			enum { k_object_type = Enums::_object_type_machine };

			s_device_machine_data machine;
		}; BOOST_STATIC_ASSERT( sizeof(s_device_machine_datum) == Enums::k_object_size_machine );

		struct s_device_control_datum : s_device_datum
		{
			enum { k_object_type = Enums::_object_type_control };

			s_device_control_data control;
		}; BOOST_STATIC_ASSERT( sizeof(s_device_control_datum) == Enums::k_object_size_control );

		struct s_device_lightfixture_datum : s_device_datum
		{
			enum { k_object_type = Enums::_object_type_light_fixture };

			s_device_lightfixture_data lightfixture;
		}; BOOST_STATIC_ASSERT( sizeof(s_device_lightfixture_datum) == Enums::k_object_size_light_fixture );
	};
};