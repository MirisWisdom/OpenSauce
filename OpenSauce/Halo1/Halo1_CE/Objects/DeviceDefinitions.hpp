/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include "Objects/ObjectDefinitions.hpp"

namespace Yelo
{
	namespace Flags
	{
		enum device_flags
		{

			// user_interation_flags

			_device_one_sided_flag = FLAG(0),
			_device_operates_automatically_flag = FLAG(1),
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
			s_device_machine_data machine;
		}; BOOST_STATIC_ASSERT( sizeof(s_device_machine_datum) == Enums::k_object_size_machine );

		struct s_device_control_datum : s_device_datum
		{
			s_device_control_data control;
		}; BOOST_STATIC_ASSERT( sizeof(s_device_control_datum) == Enums::k_object_size_control );

		struct s_device_lightfixture_datum : s_device_datum
		{
			s_device_lightfixture_data lightfixture;
		}; BOOST_STATIC_ASSERT( sizeof(s_device_lightfixture_datum) == Enums::k_object_size_light_fixture );
	};
};