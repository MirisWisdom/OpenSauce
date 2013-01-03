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
		enum item_flags : long_flags {
			_item_in_unit_inventory_bit,

			_item_in_unit_inventory_flag = FLAG(_item_in_unit_inventory_bit),
		};
	};

	namespace Objects
	{
		struct s_item_data
		{
			long_flags flags;					// 0x1F4
			int16 detonation_countdown;			// 0x1F8
			UNKNOWN_TYPE(int16);				// 0x1FA collision surface index
			UNKNOWN_TYPE(int16);				// 0x1FC structure bsp index
			PAD16;								// 0x1FE
			UNKNOWN_TYPE(datum_index);			// 0x200 object index
			int32 last_update_time;				// 0x204
			UNKNOWN_TYPE(datum_index);			// 0x208 object index
			UNKNOWN_TYPE(real_point3d);			// 0x20C
			UNKNOWN_TYPE(real_vector3d);		// 0x218
			UNKNOWN_TYPE(real_euler_angles2d);	// 0x224
		}; BOOST_STATIC_ASSERT( sizeof(s_item_data) == (Enums::k_object_size_item - Enums::k_object_size_object) );


		struct s_garbage_data
		{
			int16 ticks_until_gc;
			PAD16;
			int32 _unused[5];
		}; BOOST_STATIC_ASSERT( sizeof(s_garbage_data) == (Enums::k_object_size_garbage - Enums::k_object_size_item) );



		struct s_item_datum
		{
			s_object_data object;
			s_item_data item;
		}; BOOST_STATIC_ASSERT( sizeof(s_item_datum) == Enums::k_object_size_item );

		struct s_garbage_datum : s_item_datum
		{
			s_garbage_data garbage;
		}; BOOST_STATIC_ASSERT( sizeof(s_garbage_datum) == Enums::k_object_size_garbage );
	};
};