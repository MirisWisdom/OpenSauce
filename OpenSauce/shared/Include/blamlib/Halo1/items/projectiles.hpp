/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/objects/object_structures.hpp>
#include <blamlib/Halo1/items/item_structures.hpp>

namespace Yelo
{
	namespace Flags
	{
		enum projectile_datum_flags
		{
			_projectile_datum_flags_attached_bit = 3,

			_projectile_datum_flags
		};
	};

	namespace Objects
	{
		struct s_projectile_datum_network_data
		{
			real_point3d position;
			real_vector3d transitional_velocity;
		}; BOOST_STATIC_ASSERT( sizeof(s_projectile_datum_network_data) == 0x18 );
		struct s_projectile_data
		{
			// FLAG(1) - tracer
			long_flags flags;									// 0x22C
			_enum action;										// 0x230
			UNKNOWN_TYPE(int16);								// 0x232, material index
			datum_index source_unit_index;						// 0x234
			datum_index target_object_index;					// 0x238
			int32 contrail_attachment_index;					// 0x23C, index for the proj's definition's object_attachment_block, index is relative to object.attachments.attachment_indices or NONE
			real time_remaining;								// 0x240, to target, or 0.0
			real arming_rate;									// 0x244, related to detonation coundown timer
			real unknown_real2;									// 0x248
			real unknown_real3;									// 0x24C, related to arming_time
			real range_traveled;								// 0x250
			real_vector3d transitional_velocity;				// 0x254
			UNKNOWN_TYPE(real);									// 0x260, set to water_damage_range's upper bound
			real_vector3d angular_velocity;						// 0x264
			UNKNOWN_TYPE(real_euler_angles2d);					// 0x270
			//////////////////////////////////////////////////////////////////////////
			// Added in HaloPC
			UNKNOWN_TYPE(bool);									// 0x278
			bool baseline_valid;								// 0x279
			sbyte baseline_index;								// 0x27A
			sbyte message_index;								// 0x27B
			s_projectile_datum_network_data update_baseline;	// 0x27C
			UNKNOWN_TYPE(bool);									// 0x294, delta_valid?
			PAD24;
			s_projectile_datum_network_data update_delta;		// 0x298
		}; BOOST_STATIC_ASSERT( sizeof(s_projectile_data) == (Enums::k_object_size_projectile - Enums::k_object_size_item) );


		struct s_projectile_datum
		{
			enum { k_object_types_mask = FLAG(Enums::_object_type_projectile) };

			s_object_data object;
			s_item_data unused;									// 0x1F4
			s_projectile_data projectile;
		}; BOOST_STATIC_ASSERT( sizeof(s_projectile_datum) == Enums::k_object_size_projectile );
	};
};