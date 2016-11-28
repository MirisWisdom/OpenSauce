/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/objects/object_structures.hpp>
#include <blamlib/Halo1/items/item_structures.hpp>

#include <blamlib/Halo1/items/weapon_definitions.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum weapon_state : byte_enum
		{
			_weapon_state_idle,

			_weapon_state_fire1,	_weapon_state_fire2,
			_weapon_state_chamber1,	_weapon_state_chamber2,
			_weapon_state_reload1,	_weapon_state_reload2,
			_weapon_state_charged1,	_weapon_state_charged2,

			_weapon_state_ready,
			_weapon_state_put_away,

			_weapon_state,
		};

		enum weapon_trigger_state : byte_enum
		{
			_weapon_trigger_state_idle,
			_weapon_trigger_state_unknown1, // alternate shot/overload related
			_weapon_trigger_state_charging,
			_weapon_trigger_state_charged,
			_weapon_trigger_state_unknown4, // locked related (tracking)
			_weapon_trigger_state_unknown5, // tracking related
			_weapon_trigger_state_spewing,
			_weapon_trigger_state_frozen_while_triggered,
			_weapon_trigger_state_frozen_timed, 

			_weapon_trigger_state,
		};

		enum weapon_magazine_state : _enum
		{
			_weapon_magazine_state_idle,
			_weapon_magazine_state_chambering_start,
			_weapon_magazine_state_chambering_finish,
			_weapon_magazine_state_chambering,
			_weapon_magazine_state,
		};
	};

	namespace Objects
	{
		struct s_weapon_datum_network_data
		{
			real_point3d position;
			real_vector3d transitional_velocity;
			PAD32; PAD32; PAD32; // not used in the update...probably a real_vector3d (angular_velocity?)
			int16 magazine_rounds_totals[Enums::k_maximum_number_of_magazines_per_weapon];
			real age;
		}; BOOST_STATIC_ASSERT( sizeof(s_weapon_datum_network_data) == 0x2C );

		struct s_weapon_data
		{
			struct s_trigger_state
			{
				sbyte idle_time;					// 0x0 used for determining when to fire next projectile (rounds per second)
				Enums::weapon_trigger_state state;
				int16 time;
				UNKNOWN_TYPE(long_flags);			// 0x4
				UNKNOWN_TYPE(int16);				// 0x8 firing effect related
				UNKNOWN_TYPE(int16);				// 0xA firing effect related
				UNKNOWN_TYPE(int16);				// 0xC firing effect related
				int16 rounds_since_last_tracer;
				real rate_of_fire;					// 0x10
				real ejection_port_recovery_time;	// 0x14
				real illumination_recovery_time;	// 0x18
				UNKNOWN_TYPE(real);					// 0x1C used in the calculation of projectile error angle
				datum_index charging_effect_index;	// 0x20
				sbyte network_delay_time;			// 0x24 hardedcoded to delay fire/reload by 10 frames in networked game
				PAD8;
				PAD16;
			}; BOOST_STATIC_ASSERT( sizeof(s_trigger_state) == 0x28 );
			struct s_magazine_state // '?' means IDK if its actually padding or there are values there. If there are, IDK their types (could be a boolean!)
			{
				Enums::weapon_magazine_state state;
				int16 reload_time_remaining;		// 0x2 in ticks
				int16 reload_time;					// 0x4 in ticks
				int16 rounds_unloaded;				// 0x6
				int16 rounds_loaded;				// 0x8
				int16 rounds_left_to_recharge;		// 0xA number of rounds left to apply to rounds_loaded (based on tag's rounds_recharged)
				UNKNOWN_TYPE(int16);				// 0xC I just know a WORD is here, may be an _enum
				PAD16; // ?
			}; BOOST_STATIC_ASSERT( sizeof(s_magazine_state) == 0x10 );
			struct s_start_reload_data
			{
				int16 starting_total_rounds[Enums::k_maximum_number_of_magazines_per_weapon];
				int16 starting_loaded_rounds[Enums::k_maximum_number_of_magazines_per_weapon];
			}; BOOST_STATIC_ASSERT( sizeof(s_start_reload_data) == 0x8 );

			// FLAG(3) - _weapon_must_be_readied_bit
			long_flags flags;						// 0x22C
			word_flags owner_unit_flags;			// 0x230
			PAD16;
			real primary_trigger;					// 0x234
			Enums::weapon_state weapon_state;		// 0x238
			PAD8;
			int16 ready_time;						// 0x23A in ticks
			real heat;								// 0x23C
			real age;								// 0x240
			real illumination_fraction;				// 0x244
			real integrated_light_power;			// 0x248
			PAD32;									// 0x24C unused
			datum_index tracked_object_index;		// 0x250
			PAD64;									// 0x254 unused
			int16 alt_shots_loaded;					// 0x25C
			PAD16;									// 0x25E
			s_trigger_state triggers[Enums::k_maximum_number_of_triggers_per_weapon];	// 0x260
			s_magazine_state magazines[Enums::k_maximum_number_of_magazines_per_weapon];// 0x2B0
			int32 last_trigger_fire_time;			// 0x2D0
			s_start_reload_data start_reload_update;// 0x2D4
			PAD32; // 0x2DC need to verify this is unused
			bool baseline_valid;					// 0x2E0
			sbyte baseline_index;					// 0x2E1
			sbyte message_index;					// 0x2E2
			PAD8;									// 0x2E3
			s_weapon_datum_network_data update_baseline;	// 0x2E4
			UNKNOWN_TYPE(bool);						// 0x310 probably delta_valid
			PAD24;									// 0x311
			s_weapon_datum_network_data update_delta;		// 0x314
		}; BOOST_STATIC_ASSERT( sizeof(s_weapon_data) == (Enums::k_object_size_weapon - Enums::k_object_size_item) );


		struct s_weapon_datum : s_item_datum
		{
			enum { k_object_types_mask = FLAG(Enums::_object_type_weapon) };

			s_weapon_data weapon;
		}; BOOST_STATIC_ASSERT( sizeof(s_weapon_datum) == Enums::k_object_size_weapon );
	};
};