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
			_item_hidden_in_unit_inventory_bit, // I think? this bit is either/or with the above bit. name taken from H2
			_item_rotating_bit, // no epsilon check on rotation (bug?), thus always set after it starts rotating
			_item_collided_with_bsp_bit,
			_item_collided_with_object_bit,
			_item_at_reset_bit, // see _object_at_reset_bit
		};
	};

	namespace Objects
	{
		struct s_item_data
		{
			long_flags flags;					// 0x1F4
			game_time_t detonation_countdown;	// 0x1F8
			struct {
				int16 surface_index;			// 0x1FA
				int16 bsp_reference_index;		// 0x1FC
			}bsp_collision;
			PAD16;								// 0x1FE
			datum_index dropped_by_unit_index;	// 0x200 set when the unit who had this item drops it
			game_ticks_t last_update_time;		// 0x204
			struct {
				datum_index object_index;		// 0x208
				real_point3d object_position;	// 0x20C
			}object_collision;
			UNKNOWN_TYPE(real_vector3d);		// 0x218
			UNKNOWN_TYPE(real_euler_angles2d);	// 0x224
		}; BOOST_STATIC_ASSERT( sizeof(s_item_data) == (Enums::k_object_size_item - Enums::k_object_size_object) );


		struct s_garbage_data
		{
			game_time_t ticks_until_gc;
			PAD16;
			int32 _unused[5];
		}; BOOST_STATIC_ASSERT( sizeof(s_garbage_data) == (Enums::k_object_size_garbage - Enums::k_object_size_item) );



		struct s_item_datum
		{
			enum { k_object_types_mask = Enums::_object_type_mask_item };

			s_object_data object;
			s_item_data item;
		}; BOOST_STATIC_ASSERT( sizeof(s_item_datum) == Enums::k_object_size_item );

		struct s_garbage_datum : s_item_datum
		{
			enum { k_object_types_mask = FLAG(Enums::_object_type_garbage) };

			s_garbage_data garbage;
		}; BOOST_STATIC_ASSERT( sizeof(s_garbage_datum) == Enums::k_object_size_garbage );
	};
};